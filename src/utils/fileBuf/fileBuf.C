#ifdef __GNUG__
#pragma implementation
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <unistd.h>
//#include <stream.h>
//#include <fstream.h>
#include <fstream>
using std::fstream;
//#include <iostream.h>
#include <iostream>
using std::ostream;
#include <stdlib.h>
#include "fileBuf.h"
#include "utils/string/stringUtils.h"

void FileBuf::setChannels(int channels) 
{ Channels = channels ;

  BufPageCounter = new int [channels];
  Buf = new char* [channels];
  BufLineStatus = new int* [channels];
  
  ChannelComment = new char* [channels];
  ChannelWordType = new WORD_TYPE [channels];
  ChannelYSize = new unsigned int [channels];
  ChannelXSize = new unsigned int [channels];
  ChannelYStart = new int [channels];
  ChannelXStart = new int [channels];
  ChannelXStride  = new double [channels];
  ChannelYStride  = new double [channels];
  ChannelMin = new double [channels];
  ChannelMax = new double [channels];
  ChannelDirectory = new char * [channels];

  // initilize
  for(int c = 0; c < channels; c++){
    ChannelComment[c]  = 0;
    ChannelYSize[c] = 0;
    ChannelXSize[c] = 0;
    ChannelYStart[c] = 0;
    ChannelXStart[c] = 0;
    ChannelYStride[c] = 1.0;
    ChannelXStride[c] = 1.0;
    setChannelMinMax(c);
    ChannelDirectory[c] = NULL;

    Buf[c] = NULL;
    BufPageCounter[c] = 0;
  
    BufLineStatus[c] = new int [BufLines];
    for (int y = 0; y < (int)BufLines; y++)
      setBufLineStatus(c,y,DIRTY);
  }
}

void FileBuf::bufMakeSpace(int channel){ 
  if(!Buf) setChannels(Channels);
  Buf[channel] = new char 
    [BufLines*xSize(channel)*wordSize(channel)];
  BufPageCounter[channel] = 0;
  bufZeroClean(channel); 
  //Open(channel);
  if ((isOpen = (Open(channel)==0))!=0)
    std::cout << "Opening: " << FileName(channel) << '\n' << std::flush; 
  else
    std::cout << "Failed to open: " << FileName(channel) << '\n' << std::flush;
}

void FileBuf::bufFlush(int channel){
  if(Buf[channel]){
    long empty = 0; 
    writeData(channel, 
	      yStart(channel)+ySize(channel)-1,
	      xStart(channel)+xSize(channel)-1,
	      &empty, 1 );
    bufWrite(channel);
  }
}

void FileBuf::bufWrite(int channel){
  bufZeroDirty(channel); 
  if (isOpen)
    DiskWrite(channel); 
 (BufPageCounter[channel])++;
}

FileBuf::~FileBuf(){

  delete GlobalComment;
  delete FnameStub; 

  delete BufPageCounter;
  delete ChannelMax;
  delete ChannelMin;
  delete ChannelYStart;
  delete ChannelXStart;
  delete ChannelYSize;
  delete ChannelXSize;
  delete ChannelXStride;
  delete ChannelYStride;
  delete ChannelWordType;

  for(int c=0; c<(int)Channels; c++){
    if (Buf) delete Buf[c];
    if (BufLineStatus) delete BufLineStatus[c];
    if (ChannelComment) delete ChannelComment[c];
    if (ChannelDirectory) delete ChannelDirectory[c];
  }
  delete Buf;
  delete BufLineStatus;
  delete ChannelComment;
  delete ChannelDirectory;
}

void FileBuf::bufZeroClean(int channel){ 
  for (int bufln=0; bufln<(int)BufLines; bufln++) {
    if(bufLineStatus(channel,bufln) != CLEAN)
      memset(DataPtr(channel,bufln,0), 0,
	     xSize(channel)*wordSize(channel));
    setBufLineStatus(channel,bufln,CLEAN); 
  }
}

void FileBuf::bufZeroDirty(int channel){
  for (int bufln=0; bufln<(int)BufLines; bufln++) 
    if(bufLineStatus(channel,bufln) == DIRTY){
      memset(DataPtr(channel,bufln,0), 0,
	     xSize(channel)*wordSize(channel));
      setBufLineStatus(channel,bufln,CLEAN);
    } else 
      setBufLineStatus(channel,bufln,DIRTY);
}

void FileBuf::writeData
(int channel, int y, int x, void * data, int xs)
{
  
	   



//    OutFile[f][i]->writeData(c,
//                            (int)(wline),
//                            xoffset,
//                            converted, 
//		              pixels);

  if (inBounds(channel,y,x,xs) ){ 


    if(!Buf[channel]) bufMakeSpace(channel); 
    
    int bufPg = (y-yStart(channel))/BufLines;
    int bufLn = (y-yStart(channel))%BufLines;
   
    while(bufPg > BufPageCounter[channel]){
      bufWrite(channel);
      if(xs == 1) bufZeroClean(channel);
    }
    
    setBufLineStatus(channel,bufLn,KEEP); 
    
    if(xs == 1 ) 
      memcpy(DataPtr(channel, bufLn, x-xStart(channel)),
	     data,
	     wordSize(channel) );
    else{ 
      
      unsigned int LeftFill=
	MIN(MAX(0,x-xStart(channel)), (int)xSize(channel)) ;
      
      unsigned int XS = 
	MIN((int)(xSize(channel)-LeftFill),
	    x+xs-xStart(channel) );

      
      XS = MIN((int)XS, xs);
      
      void * data0 = (char*) data + 
	MAX(0,xStart(channel)-x)*wordSize(channel);
      
      unsigned int RightFill = 
	xSize(channel) - XS - LeftFill ;
      
      if (LeftFill)
	memset(DataPtr(channel,bufLn,0),
	       0,LeftFill*wordSize(channel));
      
      if(XS)
	memcpy(DataPtr(channel,bufLn,LeftFill),
	       data0, XS*wordSize(channel));

      if(RightFill)
	memset(DataPtr(channel,bufLn,LeftFill+XS),0,
	       RightFill*wordSize(channel)); 
    }
  }
}

char * FileBuf::tmpName(char * fname){
  static char tmp[FILENAME_SIZE] ;
  strcpy(tmp,fname);
  strcpy(strrchr(tmp, '.'), ".tmp");

  return(tmp);
}

void FileBuf::setFnameStub(char * text) {
  if (!isOpen ) { 
    FnameStub = new char [strlen(text)+1];
    strcpy(FnameStub, text);
  }
}

void FileBuf::setChannelDirectory(int channel, char * dir){
  char linkName[FILENAME_SIZE];
  realpath(dir,linkName);
  strcat(linkName,"/");
  ChannelDirectory[channel] = new char [strlen(linkName)+1];
  strcpy(ChannelDirectory[channel],linkName);
}

void FileBuf::setComment(char * text, int channel){
  if(channel==-1) {
    GlobalComment = new char [strlen(text)+1];
    strcpy(GlobalComment,text);
  }else {
    ChannelComment[channel] = new char [strlen(text)+1];
    strcpy(ChannelComment[channel],text);
  }
}

void FileBuf::MakeLink(int channel, char * Extn ){ 

  fildes txt;
  char lnk[FILENAME_SIZE];
  char lnktxt[FILENAME_SIZE];
  char fname[FILENAME_SIZE];

  strcpy(lnk,ChannelDirectory[channel]);
  strcat(lnk, "latest");
  if(Extn) strcat(lnk, Extn);

  strcpy(fname,"./");
  strcat(fname,baseName(FileName(channel)));

  unlink(lnk);
  symlink(fname, lnk);

  strcpy(lnktxt, ChannelDirectory[channel]);
  strcat(lnktxt, "latest.info");
  

  txt = open(lnktxt,
	     O_RDWR | O_CREAT ,
	     S_IROTH| S_IRUSR |S_IWUSR |S_IRGRP  );

  if (GlobalComment)
    write(txt,GlobalComment,strlen(GlobalComment));
  if (ChannelComment[channel]) 
    write(txt,ChannelComment[channel],strlen(ChannelComment[channel]));
  close(txt);

}

int FileBuf::lineClean (int channel, int y, int x, int xs)
{
  return(inBounds(channel, y, x, xs) &&
         (bufLineStatus(channel, y) != KEEP ||
          y/(int)BufLines != (int)BufPageCounter[channel]));
}


// virtual Functions
int FileBuf::Open(int /*channel*/) { return 0;}

int FileBuf::Close(int /*channel*/) { return 0;} 

int FileBuf::DiskWrite(int /*channel*/) { return 0;}  

char * FileBuf::FileName(int/*channel*/,char* /*Extn*/){return(NULL);}


