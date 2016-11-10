#include "fileBuf.h"
#include "fileBin.h"
#include "fileTiff.h"
#include "fileJpeg.h"
#include "fileMcIDAS.h"
#include "genericFile.h"

FileBuf * GenericFile(FILE_TYPE fileType ){

  switch (fileType) { 
    
  case (noFile):
    return(new FileBuf);
  case (binFile): 
    return(new FileBin);
  case (tiffLine): 
    return(new FileTiff(line)); 
  case (tiffTile):
    return (new FileTiff(tile)) ;
  case (tiffStrip):
    return(new FileTiff(strip));
  case (jpegFile):
    return(new FileJpeg);
  case (mcidasFile):
    return(new FileMcIDAS);
  default:
    return(new FileBuf);
  }
  
}

