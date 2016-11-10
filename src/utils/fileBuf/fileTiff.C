#ifdef __GNUG__
#pragma implementation
#endif

#include "fileTiff.h"
//#include <strstream.h>
//#include <stream.h>
#include <sstream>
#include <stdlib.h>

FileTiff::FileTiff(TiffFileMode mode)
{ Mode = mode; 
  Tiff = NULL;
  tileBuf = NULL;
  if (Mode!=line) FileBuf::BufLines =  YTILE ;
}

FileTiff::~FileTiff() { 
  if (Tiff){
    for(int c=0; c< (int)Channels ; c++)
      if(Tiff[c])  { Close(c) ;} 
    delete Tiff;
    delete tileBuf;
  }
}


int FileTiff::Open(int channel) { 
  
  if (!Tiff) { 
    Tiff =  new TIFF* [Channels];
    for (int c=0 ; c<  (int)Channels; c++) Tiff[c] = NULL;
  }
  
  Tiff[channel] = TIFFOpen(tmpName(FileName(channel)), "w"); 
  if (!Tiff[channel])
    return -1;

  char tComment[4096];
  strcpy(tComment, GlobalComment);
  strcat(tComment, ChannelComment[channel]);
  TIFFSetField(Tiff[channel],
	       TIFFTAG_IMAGEDESCRIPTION,
	       tComment);


  TIFFSetField(Tiff[channel], 
	       TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(Tiff[channel],
	       TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

  TIFFSetField(Tiff[channel],
	       TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
  
  TIFFSetField(Tiff[channel],
	       TIFFTAG_BITSPERSAMPLE, 8*wordSize(channel));

  

  switch(Mode){
  case line:
    TIFFSetField(Tiff[channel],
		 TIFFTAG_IMAGEWIDTH, xSize(channel));
    TIFFSetField(Tiff[channel],
		 TIFFTAG_IMAGELENGTH, ySize(channel));
    break;
    
  case strip:
    TIFFSetField(Tiff[channel],
		 TIFFTAG_IMAGEWIDTH, 
		 xSize(channel));
    TIFFSetField(Tiff[channel],
		 TIFFTAG_IMAGELENGTH, 
		 (int)((ySize(channel)+YTILE)/YTILE)*YTILE);
    TIFFSetField(Tiff[channel],TIFFTAG_ROWSPERSTRIP, YTILE);
    break;

  case tile:
    TIFFSetField(Tiff[channel],TIFFTAG_TILEWIDTH, XTILE);
    TIFFSetField(Tiff[channel],TIFFTAG_TILELENGTH, YTILE);
    
    TIFFSetField(Tiff[channel],
		 TIFFTAG_IMAGEWIDTH, 
		 (int)((xSize(channel)+XTILE)/XTILE) *XTILE);
    TIFFSetField(Tiff[channel],
		 TIFFTAG_IMAGELENGTH, 
		 (int)((ySize(channel)+YTILE)/YTILE) * YTILE);
    break;
    
  default:
    break;
  }
  
  switch (ChannelWordType[channel]) { 
  case (Char):
    TIFFSetField(Tiff[channel],
		 TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT);
    break;
  case (Uchar):
    TIFFSetField(Tiff[channel],
		 TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
    break;
  case (Ushort):
    TIFFSetField(Tiff[channel],
		 TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
    break;
  case (Short):
    TIFFSetField(Tiff[channel],
		 TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT);
    break;
  case (Uint):
    TIFFSetField(Tiff[channel],
		 TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
    break;
  case (Int):
    TIFFSetField(Tiff[channel],
		 TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT);
    break;
  case (Float) :  
    TIFFSetField(Tiff[channel],
		 TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
    break;
  case (Double):
    TIFFSetField(Tiff[channel],
		 TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
    break;
  default:
    break;
  }

  if(Compress)
    TIFFSetField(Tiff[channel], TIFFTAG_COMPRESSION, Compress); 
  
  if (ChannelMin[channel]<ChannelMax[channel]) { 
    TIFFSetField(Tiff[channel],
		 TIFFTAG_MINSAMPLEVALUE, (short)ChannelMin[channel]);
    TIFFSetField(Tiff[channel],
		 TIFFTAG_MAXSAMPLEVALUE, (short)ChannelMax[channel]);
  } 
  else{
    TIFFSetField(Tiff[channel],
		 TIFFTAG_MINSAMPLEVALUE, (short)ChannelMax[channel]);
    TIFFSetField(Tiff[channel],
		 TIFFTAG_MAXSAMPLEVALUE, (short)ChannelMin[channel]);
  }

  return(0);
}

char * FileTiff::FileName(int channel, char * Extn){

  char type[6]; 
  switch(Mode){
  case line: 
    strcpy(type,".tif");
    break;
  case strip:
    strcpy(type,".stif");
    break;
  case tile:
    strcpy(type, ".ttif");
    break;
  default:
    strcpy(type, ".xxxx");
    break;
  }
  
  static char tmp[FILENAME_SIZE];
  std::stringstream fn; 
  fn << ChannelDirectory[channel];
  fn << FnameStub;
  fn.fill('0'); fn.width(2);
  fn << channel+1;
  fn << type;
  if(Extn) fn << Extn;
  fn << std::ends;
  fn.rdbuf()->sgetn(tmp, FILENAME_SIZE);
  return(tmp);
}

int FileTiff::DiskWrite(int channel){

  int ln = 0; 
  switch(Mode){
  case line:
    for (ln = 0;
	 ln<(int)BufLines &&
	 (imageLine(channel,ln)<(int)ySize(channel));
	 ln++)
      TIFFWriteScanline
	(Tiff[channel],
	 DataPtr(channel,ln,0),
	 imageLine(channel,ln),
	 0);
    break;

  case strip:
    TIFFWriteEncodedStrip
      (Tiff[channel], 
       TIFFComputeStrip(Tiff[channel],imageLine(channel),1),
       DataPtr(channel,0,0), 
       (long)(BufLines*xSize(channel)*wordSize(channel)));
    break;
    
  case tile:
    if (imageLine(channel) < (int)ySize(channel)){
      for (ln = 0; 
	   ln < (int)(xSize(channel)+XTILE)/XTILE ;
	   ln++)
	TIFFWriteTile
	  (Tiff[channel],
	   getTile(channel,ln),
	   ln * XTILE ,
	   imageLine(channel),
	   0,0) ;
    }
    break;
    
  default:
    break;
  }
  return(0);
}


int FileTiff::Close(int channel){
  if( Tiff[channel] ){
    bufFlush(channel);
    TIFFClose(Tiff[channel]);
    rename(tmpName(FileName(channel)), FileName(channel));
    if(linkLatest()) MakeLink(channel, ".tif");
    Tiff[channel] = NULL;
  }
  return(0);
}

char* FileTiff::getTile(int channel, unsigned int xtile)
{  
  if (!tileBuf) 
    tileBuf = new char [XTILE * YTILE * wordSize(channel)];
  
  int copyBytes = MIN(xSize(channel)-xtile*XTILE, XTILE) 
    *wordSize(channel);

  unsigned int zeroBytes = 
    (unsigned)(XTILE*wordSize(channel) - copyBytes);
    
  for(int y = 0; y < YTILE ; y++){
    if(imageLine(channel, y) < (int)ySize(channel)){
      memcpy(tileBuf + XTILE * y * wordSize(channel),
	     DataPtr(channel,y,xtile*XTILE),
	     copyBytes) ;
      if(zeroBytes)
	memset(tileBuf + XTILE* y* wordSize(channel) + copyBytes,
	       0, zeroBytes);
    }else  
      memset(tileBuf+ XTILE * y * wordSize(channel), 
	     0, XTILE * wordSize(channel));
  }
  return(tileBuf);
}
