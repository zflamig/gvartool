#ifdef __GNUG__
#pragma interface 
#endif

#ifndef FILE_JPEG_H
#define FILE_JPEG_H


#include "fileBuf.h"

extern "C" {
#include <jpeglib.h>
#include <jerror.h>
}


#include <setjmp.h>


class FileJpeg: public FileBuf{ 
 private:
  struct jpeg_compress_struct* *cinfo; 
  struct jpeg_error_mgr jerr;
  FILE* *Bin; 

 public:
  virtual int Open(int channel);
  virtual int DiskWrite(int channel);
  virtual int Close(int channel);
  virtual char* FileName(int channel, char * extn=NULL);
	
  FileJpeg() ; 
  ~FileJpeg(); 
	
};

#endif // FILE_JPEG_H
