#ifdef __GNUG__
#pragma interface 
#endif
#ifndef FILE_MCIDAS_H
#define FILE_MCIDAS_H

#include "fileBuf.h"
#include "block0/imagerDoc.h"
#include "McIDAS.h"
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <unistd.h>
//#include <strstream.h>
#include <stdlib.h>
#include "utils/string/stringUtils.h"

class FileMcIDAS:public FileBuf{ 
 private:
  McIDAS mcidas;
  fildes *Bin; 
  fildes *Doc;
  void InitMcIDAS(ImagerDoc & DOC, int channel);
 public:

  virtual int Open(int channel);
  virtual int DiskWrite(int channel);
  virtual int Close(int channel);
  virtual char* FileName(int channel, char * extn=NULL);
  
  FileMcIDAS() ; 
  ~FileMcIDAS(); 

};

#endif // FILE_MCIDAS_H






