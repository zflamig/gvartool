#ifdef __GNUG__
#pragma interface 
#endif
#ifndef FILE_BIN_H
#define FILE_BIN_H

#include "fileBuf.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <unistd.h>
//#include <stream.h>
#include <iostream>

//#include <strstream.h>
#include <sstream>

//#include <fstream.h>
#include <fstream>

#include <stdlib.h>
#include "utils/string/stringUtils.h"

class FileBin: public FileBuf{ 
 private:
  fildes *Bin; 
  fildes *Doc;
 public:

  virtual int Open(int channel);
  virtual int DiskWrite(int channel);
  virtual int Close(int channel);
  virtual char* FileName(int channel, char * extn=NULL);

  
  FileBin() ; 
  ~FileBin(); 

};

#endif // FILE_BIN_H

