#ifdef __GNUG__
#pragma interface 
#endif
#ifndef FILE_TIFF_H
#define FILE_TIFF_H

#define XTILE 128
#define YTILE 128

#define TIFF_CHANNELS 20 

#include "fileBuf.h"
#include <tiffio.h>

class FileTiff: public FileBuf{ 
 private:
  TIFF* *Tiff;
  TiffFileMode Mode; 
  char* tileBuf; 
  char* getTile(int channel, unsigned int xtile);
 public: 

  virtual int Open(int channel);
  virtual int DiskWrite(int channel);
  virtual int Close(int channel);
  virtual char* FileName(int channel, char * Extn = NULL);

  FileTiff(TiffFileMode mode =line); 
  ~FileTiff();
};

#endif // FILE_TIFF_H

				
	



