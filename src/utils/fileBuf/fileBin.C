#ifdef __GNUG__
#pragma implementation
#endif

#include "fileBin.h"

FileBin::FileBin() { Bin = NULL ; Doc = NULL; }

FileBin::~FileBin(){
  for(int c = 0; c < (int)Channels; c++)
    if ( Bin && Bin[c]) { Close(c);}
  delete Bin;
  delete Doc;
}

int FileBin::Open(int channel){
  if (!Bin) { 
    Bin = new fildes [Channels];
    Doc = new fildes [Channels];
    for ( int c = 0; c < (int)Channels; c++) { 
      Bin[c] = 0;
      Doc[c] = 0;
    }
  }
  Bin[channel] = open(tmpName(FileName(channel)), 
		      O_RDWR | O_CREAT, 
		      S_IROTH| S_IRUSR |S_IWUSR |S_IRGRP  );

  Doc[channel] = open(FileName(channel,"info"),
		      O_RDWR | O_CREAT ,
		      S_IROTH| S_IRUSR |S_IWUSR |S_IRGRP  );

  return(0);
}


int FileBin::Close(int channel){
  if (Bin[channel]) { 
    bufFlush(channel);
    
    if(GlobalComment)
    write(Doc[channel],GlobalComment,strlen(GlobalComment));

    if(ChannelComment[channel])
      write(Doc[channel],ChannelComment[channel],strlen(ChannelComment[channel]));

    close(Bin[channel]);
    close(Doc[channel]);
    Bin[channel] = 0;

    rename(tmpName(FileName(channel)), FileName(channel));
    if(linkLatest()) MakeLink(channel);
  }
  return(0);
}

int FileBin::DiskWrite(int channel){  
  for(int y =0 ; 
      y < (int)BufLines && imageLine(channel,y) < (int)ySize(channel);
      y++)
    write(Bin[channel],DataPtr(channel,y,0), 
	  xSize(channel)*wordSize(channel));
  
  return(0);
}


char* FileBin::FileName(int channel, char * Extn){
  static char tmp[FILENAME_SIZE];
  std::stringstream fn; 
  fn << ChannelDirectory[channel];
  fn << FnameStub;
  fn.fill('0'); fn.width(2);
  fn << channel+1;
  fn << ".x";
  fn.fill('0'); fn.width(5);
  fn<< xSize(channel);
  fn<< ".y";
  fn.fill('0'); fn.width(5);
  fn << ySize(channel);
  if (Extn) fn << Extn;
  fn << std::ends;
  fn.rdbuf()->sgetn(tmp, FILENAME_SIZE);
  return(tmp);
}
  
  
  
