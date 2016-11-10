#ifdef __GNUG__
#pragma implementation
#endif

#include "fileJpeg.h"

#include <stdio.h>
//#include <strstream.h>
#include <sstream>



#define QUALITY 75 

FileJpeg::FileJpeg() {cinfo = NULL; Bin = NULL; }

FileJpeg::~FileJpeg(){ 
  if ( cinfo) {
    for( int c = 0; c < (int)Channels; c++) {
      if(cinfo[c])  { Close(c); delete cinfo[c]; cinfo[c] = NULL;}
    }
    delete cinfo;
    cinfo = NULL;
    delete Bin;
    Bin = NULL;
  }
}

char* FileJpeg::FileName(int channel, char * Extn){
  static char tmp[FILENAME_SIZE];
  std::stringstream fn; 
  fn << ChannelDirectory[channel];
  fn << FnameStub;
  fn.fill('0'); fn.width(2);
  fn << channel+1;
  fn << ".jpg";
  if(Extn) fn << Extn;
  fn << std::ends;
  fn.rdbuf()->sgetn(tmp, FILENAME_SIZE);
  return(tmp);
}

int FileJpeg::Open(int channel) { 
  if(!Bin) {
    Bin = new FILE * [Channels];
    cinfo = new jpeg_compress_struct * [Channels];
    for (int c = 0; c < (int)Channels ; c++){
      cinfo[c] = NULL;
      Bin[c] = NULL;
    }
  }

  Bin[channel] = fopen(tmpName(FileName(channel)),"w") ;

  cinfo[channel] = new jpeg_compress_struct;
  cinfo[channel]->err = jpeg_std_error(&jerr);
  jpeg_create_compress(cinfo[channel]);
  jpeg_stdio_dest(cinfo[channel], Bin[channel]);
  cinfo[channel]->image_width = xSize(channel);
  cinfo[channel]->image_height = ySize(channel); 
  cinfo[channel]->input_components = 1; 
  cinfo[channel]->in_color_space = JCS_GRAYSCALE;

  if(Compress)jpeg_set_quality(cinfo[channel], Compress, TRUE );
  else jpeg_set_quality(cinfo[channel], QUALITY, TRUE );

  jpeg_set_defaults(cinfo[channel]);	
  jpeg_start_compress(cinfo[channel], TRUE);

  char jComment[FILENAME_SIZE];
  strcpy(jComment, GlobalComment);
  if(ChannelComment[channel])strcat(jComment, ChannelComment[channel]);
  jpeg_write_marker
    (cinfo[channel], JPEG_COM, (JOCTET*)jComment,strlen(jComment)); 
  
  return (0);
}

int FileJpeg::Close(int channel) {  
  if (Bin[channel]) { 
    bufFlush(channel);
    jpeg_finish_compress(cinfo[channel]);
    jpeg_destroy_compress(cinfo[channel]);
    delete cinfo[channel];
    cinfo[channel] = NULL;
    fclose(Bin[channel]);
    rename(tmpName(FileName(channel)), FileName(channel));
    if(linkLatest()) MakeLink(channel, ".jpg");
    Bin[channel] = NULL;
  }
  return(0);
}

int FileJpeg::DiskWrite(int channel){  
  JSAMPROW row_pointer[1];
  
  for (int i = 0; 
       i < (int)BufLines && imageLine(channel,i) < (int)ySize(channel); 
       i++){
    row_pointer[0] = (JSAMPROW) DataPtr(channel,i,0);
    jpeg_write_scanlines(cinfo[channel],
			 row_pointer,
			 1);
  }
  return(0);
}






