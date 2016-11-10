#ifndef GVARHEADER_H
#define GVARHEADER_H

//#include <stream.h>
#include <iostream>
#include <string.h>
#include "include/types.h"


/* refer page 61, goes-i/m oge specs  */

#define N_HEADER_BYTES 30  

#define WORD_COUNT_MIN 8040
#define WORD_COUNT_MAX 42008
#define WORD_SIZE_MAX 11
#define BLOCK_ID_MAX 12

struct Header{ 
	uchar8 BlockId;  // 1 
	uchar8 WordSize;  // 2 
	uchar8 WordCount_msb; // 3	
	uchar8 WordCount_lsb; //  4 
	uchar8 ProductId_msb; // 5
	uchar8 ProductId_lsb; // 6	
	uchar8 RepeatFlag;  // 7
	uchar8 VersionNumber; // 8
	uchar8 DataValid;    // 9
	uchar8 AsciiBin;    // 10
	uchar8 SPS_id;          // 11 version 1+???
	uchar8 RangeWord;    // 12 
	uchar8 BlockCount_msb;   // 13
	uchar8 BlockCount_lsb; // 	14 
	uchar8 Spares_1[2];    // 15 16
	uchar8 SPS_time[8];    // 17 24
	uchar8 Spares_2[4];    // 25 28
        uchar8 Crc_msb;  //  29
	uchar8 Crc_lsb; // 30	
};
typedef struct Header Header; 

class GvarHeader{ 
public:
	Header Hdr; 
public:

	GvarHeader() { } ;
	   int isValidHeader() ; 
	   int crc_is_good() ; 
	  void copyHeader(char *hdr0);

	uint16 blockId();
	uint16 wordSize();
  	   int wordCount();
	uint16 productId();
	uint16 repeatFlag();
	uint16 versionNumber();
	uint16 dataValid();
	uint16 asciiBin();
	uint16 sps_id();
	uint16 rangeWord();
	uint16 blockCount();
        uchar8 sps_time();
	uint16 crc();

           int spcID();
	uint16 calc_crc();
	   int nBytes();

// istream & read(istream in );

	void print(std::ostream & out ) ; 
	void printHeader(std::ostream & out ) ; 
	void printDframe(std::ostream & out ) ; 
	void headerText(std::ostream & out );
};


//Block ID 
//An 8-bit binary number used to identify a GVAR block as follows: 
//   240  = GVAR Block 0 
//   1-11 = GVAR Block 1-11
//   15 = Equipment Idle Block 
inline uint16 GvarHeader::blockId()    { return ( ( Hdr.BlockId)==240 ? 0: ((uint16)Hdr.BlockId) ) ; }


// Word Count 
// A 16-bit binary number giving the number of words contained in the 
// subsequent information field plus two. The extra two represents the 
// two 8-bit words containing the block CRC. 
inline int GvarHeader::wordCount()  { return (   ((int)(Hdr.WordCount_msb) <<8) + (int)Hdr.WordCount_lsb     ); }
inline int GvarHeader::nBytes()     { return (    (int)((double)wordSize() * ((double)wordCount()-2) / 8  + 2)       ); }

//Product ID 
//A 16-bit binary number used to identify information field data.  Numbers 
//used are as follows: 
//   0 = No Data 
//   1 = AAA IR Data 
//   2 = AAA Visible Data 
//   3 = GVAR Imager Documentation 
//   4 = GVAR Imager IR Data 
//   5 = GVAR Imager Visible Data 
//   6 = GVAR Sounder Documentation 
//   7 = GVAR Sounder Scan Data 
//   8 = GVAR Compensation Data 
//   9 = GVAR Telemetry Statistics 
//   10 = Not Used (was GVAR AUX Text) 
//   11 = GIMTACS Text 
//   12 = SPS Text 
//   13 = AAA Sounding Products 
//   14 = GVAR ECAL Data 
//   15 = GVAR Spacelook Data 
//   16 = GVAR BB Data 
//   17 = GVAR Calibration Coefficients 
//   18 = GVAR Visible NLUTs 
//   19 = GVAR Star Sense Data 
//   20 = Imager Factory Coefficients 
//   21-1000 = Unassigned 
inline uint16 GvarHeader::productId()  { return ( ((uint16)(Hdr.ProductId_msb) <<8) +(uint16)Hdr.ProductId_lsb  ); }

//Block Count
//A 16-bit continuous count of GVAR blocks transmitted.  Values range 
//from 0 to 65,535, inclusive, rolling over to 0 following 65,535.  
//Count is incremented by 1 for each successive GVAR block transmitted.  
//Count does not increment for Equipment Idle Blocks. 
inline uint16 GvarHeader::blockCount() { return ( ((uint16)(Hdr.BlockCount_msb)<<8) +        Hdr.BlockCount_lsb ); }

//Error Check 
//A 16-bit error checking field used to validate transmission accuracy of the 
//header information.  This is derived in the same way as the information field 
//error check, CRC.  See Section 3.3.3. 
inline uint16 GvarHeader::crc()        { return ( ((uint16)(Hdr.Crc_msb       )<<8) +        Hdr.Crc_lsb        ); }


//Word Size
//An 8-bit binary number giving the word bit length of the subsequent 
//information field as follows: 
//    6 = 6-bit word size 
//    8 = 8-bit word size 
//   10 = 10-bit word size 
inline uint16 GvarHeader::wordSize()      { return (uint16)Hdr.WordSize;      } 

//Repeat Flag
//An 8-bit flag that indicates whether the data being transmitted is new data or 
//a repeat of data previously transmitted: 
//   0 = Data Repeat 
//   1 = New Data 
inline uint16 GvarHeader::repeatFlag()    { return (uint16)Hdr.RepeatFlag;    }

 
//Version Number
//An 8-bit binary number that indicates GVAR version.  See Section 3.1.1 for 
//explanation: 
//   0 = GOES I through L original specification 
//   1 = GOES I thru L MBCC information added 
//   2 = GOES M-N 
//   3 = GOES O-P 
inline uint16 GvarHeader::versionNumber() { return (uint16)Hdr.VersionNumber; }

//Data Valid 
//The following 8-bit flag which indicates whether the transmitted data is 
//usable or only filler: 
//   0 = Filler Data 
//   1 = Valid Data 
inline uint16 GvarHeader::dataValid()     { return (uint16)Hdr.DataValid;     }

//ASCII/Binary 
//The following 8-bit flag that indicates the format of the data in the data field 
//to be either ASCII or binary: 
//   0 = Binary Data 
//   1 = ASCII Data 
inline uint16 GvarHeader::asciiBin()  { return Hdr.AsciiBin;  }

//SPS ID
// A binary number identifying the source SPS, which formats the GVAR stream.  
//Values 1 to 9 denote SPS1 to SPS9, respectively.   
inline uint16 GvarHeader::sps_id()  { return Hdr.SPS_id;  }


//Range Word 
//An 8-bit word used for ranging functions.  The first 4 bits are a binary 
//number identifying the data source spacecraft.  The last four digits are 
//always zero except when a ranging function is in progress; then, they are set 
//to ones according to the type of ranging function.  The first 4 bits have a 
//value as follows: 
//    8 = GOES-I 
//    9 = GOES-J 
//   10 = GOES-K 
//   11 = GOES-L 
//   12 = GOES-M 
//   13 = GOES-N 
//   14 = GOES-O
//   15 = GOES-P 
//The last 4 bits have the following possible hexadecimal values: 
//   0 = Not a measurement block 
//   7 = Satellite ranging measurement block 
//   F = Ground path delay measurement block 
inline uint16 GvarHeader::rangeWord() { return Hdr.RangeWord; }
inline int GvarHeader::spcID()  { return (int)( (Hdr.RangeWord & 0xf0)>>4 ); }



//inline	istream & GvarHeader::read(istream in )  
//{ return (in.read((char * ) &Hdr, N_HEADER_BYTES) ); }

#endif








