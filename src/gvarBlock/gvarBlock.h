#ifndef GVARBLOCK_H
#define GVARBLOCK_H

#include <sys/types.h>
#include <sys/stat.h>
//#include <fstream.h>
#include <fstream>
#include <iostream>

#include "include/gvar.h"
#include "include/types.h"
#include "gvarHeader/gvarHeader.h"
#include "utils/crcCheck/crcCheck.h"

class GvarBlock: public GvarHeader{ 
 private:

  std::ifstream Ifs; // input file stream

  //char* InBuf; // pointer to buffer space for input file stream

  void* Data;  // pointer to buffer for gvar data stream in memory
  uint32 Trash;  // trash counter
  int BytesRead;// bytes read
  int BlockCounter; // block counter
  int fileSize; // size of input files in bytes

  std::istream & better_FindHeader(); 
  std::istream & FindHeader(); 
  std::istream & GetData();

 public:
  GvarBlock(void/*int bufSiz = GVARBUFSIZ*/); 
  ~GvarBlock();
  uint16 getBlock();//Find block of data and copy contents to Data
  int crcBlock();//do crc check on contents to Data
  void* data(); // return pointer to Data

  void open(char* fileName);
  int getTrashBytes();
  int getBytesRead(); 
  float percentToDo(); // how much more of the file to process 
  int trash(int); // update trash counter
  void close(); // close file 
  int blockCounter(); // 
  uint16 crc() ; // CRC code
};
#if 0


inline GvarBlock::~GvarBlock()  
{ /*delete InBuf;*/ delete (char *)Data; }

inline void * GvarBlock::data()
{ return Data ; } 

inline void GvarBlock::close() 
{ BytesRead = (int)Ifs.tellg(); Ifs.close(); }

inline int GvarBlock::trash(int n =0 )
{ Trash += n; return((int)Trash);}

inline 	int GvarBlock::getTrashBytes() 
{ return (int)Trash ; }

inline 	int GvarBlock:: getBytesRead()  
{ return MAX(BytesRead,(int)Ifs.tellg()); }

inline 	float GvarBlock::percentToDo() 
{ return ( 100 - 100.0 * getBytesRead()/fileSize ) ; }

inline int GvarBlock::blockCounter()
{ return BlockCounter; }

inline uint16 GvarBlock::crc(){ 
#ifdef _MSBF_
  uint16* CRC = (uint16*)data() + (nBytes()-1)/2;  
  return( *CRC);
#else
  uint16 CRC = *((uint16*)data()+(nBytes()-1)/2);
  CRC = ((CRC&0xff00)>>8)|((CRC&0x00ff)<<8);
  return CRC;
#endif
}
#endif

#endif // GVARBLOCK_H


