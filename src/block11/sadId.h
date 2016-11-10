#ifndef SADID_H
#define SADID_H

//#include <stream.h>
#include <iostream>
using std::ostream;
#include "include/types.h"

#include "utils/cdaTime/cdaTime.h"

class SadId{ 
public:

	char Spcid ;             /* 1     Spacecraft Identity  a binary number identifying the source satellite */
	char ground_system_id ;  /* 2     SPS ID               a binary number identifying the source SPS  */
	char DataId ;            /* 3     Data Identity        a binary number denoting the SAD block type */ 
	char Fbf;                /* 4     First Block Flag     a 6-bit flag set to 63 (x '3F') if the SAD Block is the first of a series.*/
	char Lbf ;               /* 5     Last Block Flag      a 6-bit flag set to 63 (x '3F') if the SAD Block 11 is the last of a series.*/
	unsigned BlockCount: 24; /* 6 7 8 Block Count          an 18-bit binary number representing the number of blocks within a sequence of blocks.*/
	char Nrec ;              /* 9     RECORD COUNT         a 6-bit binary count of records in block; 0 to 63 corresponding to 1 to 64 records.*/ 

	char    text_source_id;           /* 10     Used to support text messages.  */
	uint16  text_word_count;          /* 11-12  Used to support text messages.  */
        CdaTime text_T_time_queued;       /* 13-20  Used to support text messages.  */

        char yaw_flip;           /* 21    YAW-FLIP FLAG        a 6-bit flag set to 63 (x '3F') if the satellite is currently yaw- flipped. */
	char spare1[9];          /* 22 - 30  spares */

	char Data;  

public:
	
	SadId() { };
	
	char spcid() ; 
	uint16 dataId();
	uint16 nrec();
	uint32 blockCount(); 
	int flip();

	void print( ostream & out ); 
	void printHeader( ostream & out);

	void * data();
};

inline uint16 SadId::dataId()
{ return DataId; }

inline uint16 SadId::nrec() 
{ return Nrec; }

inline uint32 SadId::blockCount() 
{
#ifdef _LSBF_
  return (((BlockCount&0xff0000)>>16)|
           (BlockCount&0x00ff00)|
          ((BlockCount&0x0000ff)<<16));
#else
  return BlockCount;
#endif
}

inline char SadId::spcid()
{ return Spcid; }

inline	void * SadId::data()
{ return (void*) (& Data) ; }

inline int SadId::flip()
{ return yaw_flip?1:0; }

#endif // SADID_H

