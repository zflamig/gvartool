#ifndef LINE_DOC_H
#define LINE_DOC_H

//#include <stream.h>
#include <iostream>
using std::istream;
using std::ostream;

#include "include/types.h"
#include "include/gvar.h"

#define IMAGER_LINES_MAX 1974
#define LINEDOC_BSZ 32



//For GOES-I through GOES-L, the GVAR does not order the channels 
//  according to increasing wavelength, but by the arbitrary system:
// 4, 5, 2, 3.  
//(This ordering was based on the old pre-GOES-I AAA data stream.)  
//
//Beginning with GOES-M (GVAR version  2), this is changed.  The data 
//in GVAR version 2 and beyond will be in order of increasing channel wavelength.  
//
//For example, for GOES-M through GOES-P the order will be 2, 3, 4, 6.  
//Block 1 contains IR data for channels 4 and 5 (GOES I-L) 
//                          or channels 2 and 3 (GOES-M and beyond); 
//
//Block 2 contains IR data for channels 2 and 3 (GOES I-L)
//                          or channels 4 and 6 (GOES-M and beyond). 
//
//The IR detector data position within the blocks 1 and 2 is as follows: 
// 
//For GOES I-L (== 8-11)
//Block 1:  Channel 4, Detector 1  
//          Channel 4, Detector 2  
//          Channel 5, Detector 1  
//          Channel 5, Detector 2  
//Block 2:  Channel 2, Detector 1  
//          Channel 2, Detector 2  
//          Channel 3, Detector 1  
// 
//For GOES M-N (== 12,13)
//Block 1:  Channel 2, Detector 1  
//          Channel 2, Detector 2  
//          Channel 3, Detector 1  
//          Channel 3, Detector 2  
//Block 2:  Channel 4, Detector 1  
//          Channel 4, Detector 2  
//          Channel 6, Detector 1 
// 
//For GOES O and beyond ( == 14+)
//Block 1:  Channel 2, Detector 1  
//          Channel 2, Detector 2  
//          Channel 3, Detector 1  
//          Channel 3, Detector 2  
//Block 2:  Channel 4, Detector 1  
//          Channel 4, Detector 2  
//          Channel 6, Detector 1  
//          Channel 6, Detector 2  
//
// logical detector id is 1-7 (or 8) and is in the order of
// channels as listed above. In yaw flip mode each detector pair 
// is flipped
//
// expected logical IR detector in lineDoc.detector_id
// indexed by [version: 0 = pre-GOES12, 1 = G12, G13, 2 = G14+]
//            [channel: 1 = channel 2, ... ]
//            [physical detector: 0 = north, 1 = south]
//            [mode: 0 = normal, 1 = yaw-flipped]




#ifdef _MAIN_

/*
[GVAR version] where 0 means G8-11, 1 means G12-13, 2 means 14-15
[Block]
[Flip]
[Index]
*/
int XLDet[3][10][2][4] =
{ // expected logical detector
  {{{1,2,3,4},{2,1,4,3}},  // pre-G12, block 1
   {{5,6,7,0},{6,5,7,0}},  // block 2
   {{5,0,0,0},{4,0,0,0}},  // block 3
   {{6,0,0,0},{3,0,0,0}},  // block 4
   {{7,0,0,0},{2,0,0,0}},  // block 5
   {{8,0,0,0},{1,0,0,0}},  // block 6
   {{1,0,0,0},{8,0,0,0}},  // block 7
   {{2,0,0,0},{7,0,0,0}},  // block 8
   {{3,0,0,0},{6,0,0,0}},  // block 9
   {{4,0,0,0},{5,0,0,0}}}, // block 10

  {{{1,2,3,4},{2,1,4,3}},  // G12-G13, block 1
   {{5,6,7,0},{6,5,7,0}},  // block 2
   {{5,0,0,0},{4,0,0,0}},  // block 3
   {{6,0,0,0},{3,0,0,0}},  // block 4
   {{7,0,0,0},{2,0,0,0}},  // block 5
   {{8,0,0,0},{1,0,0,0}},  // block 6
   {{1,0,0,0},{8,0,0,0}},  // block 7
   {{2,0,0,0},{7,0,0,0}},  // block 8
   {{3,0,0,0},{6,0,0,0}},  // block 9
   {{4,0,0,0},{5,0,0,0}}}, // block 10

  {{{1,2,3,4},{2,1,4,3}},  // G14-G15, block 1
   {{5,6,7,8},{6,5,8,7}},  // block 2
   {{5,0,0,0},{4,0,0,0}},  // block 3
   {{6,0,0,0},{3,0,0,0}},  // block 4
   {{7,0,0,0},{2,0,0,0}},  // block 5
   {{8,0,0,0},{1,0,0,0}},  // block 6
   {{1,0,0,0},{8,0,0,0}},  // block 7
   {{2,0,0,0},{7,0,0,0}},  // block 8
   {{3,0,0,0},{6,0,0,0}},  // block 9
   {{4,0,0,0},{5,0,0,0}}}, // block 10
};


#else
extern int XLDet[3][10][2][4];
#endif

class LineDoc{ 
public: 
  int expectedChannel(int version, int block, int index);

  uint16 Spcid; //1 
  uint16 ground_system_id; // 2
  uint16 side_id; // 3
  uint16 detector_id; // 4
  

  uint16 channel_id; // 5
  uint16 rel_scan_countMsb ; // 6 
  uint16 rel_scan_countLsb; // 7 

  uint16 L1scan;   //8
  uint16 L2scan;   //9

  uint16 LpixlsMsb; // 10 
  uint16 LpixlsLsb; // 11

  uint16 LwordsMsb; // 12 
  uint16 LwordsLsb; // 13

  uint16 Lzcor ;    // 14 
  uint16 is_lagged  ;    // 15 
  uint16 Lspar ;    // 16 

  unsigned short Data; 

public: 
  LineDoc() { } ; 
  istream & read( istream& in ) ; 
  int verify(ImagerDoc &b0, int block, int linendx); /*cmm*/

  int spcid() { return( Spcid ); }
  //  int spsid() { return( ground_system_id ); }
  //  int lside() { return( Lside ); }
  //  int lidet() { return( detector_id ); }
  //  int licha() { return( channel_id ); }
  uint16 rel_scan_count() {return(((uint16)rel_scan_countMsb<<10)+rel_scan_countLsb ); }


   //8   L1SCAN*   Imager scan status word 1: Bits 0-3 are not used. 
   //               Bits 4-9 are duplicates of bits 2- 7 of the ISCAN field and are defined in Table 3-6a.
   //9   L2SCAN*   Imager scan status word 2: Bits 0-1 are not used. 
   //               Bits 2-9 are duplicates of bits 8- 15 of the ISCAN field and are defined in Table 3-6a.
   //
   //Table 3-6a. Imager Documentation Block 0 (Words 0-6304) Format Definition
   //3-6  ISCAN   Imager Scan status is provided in four words (32 bits); 
   //   bit 0 is the Most Significant Bit (MSB) of word 3, and bit 31 is the Least Significant Bit (LSB) of word 6. 
   //   For each status bit that does not have a bit value of 0 listed below, a value of 0 represents a condition 
   //   that is the opposite, or negative of the condition associated with a bit value of 1. 
   //   For example, if bit 0 has a value of 1, a frame start occurred on this scan; 
   //                if bit 0 has a value of 0, no frame start occurred on this scan.
   //
   //Bit            Value   Condition				
   //2	=      1 	if frame break - line(s) lost 		
   //3	=      1 	if pixel(s) lost 			
   //4	=      1 	if priority 1 frame data 		
   //5	=      1 	if priority 2 frame data 		
   //6	=      0	if west-to-east scan 			
   //	         1 	if east-to-west scan			
   //7	=      0	if north-to-south frame 		
   //	        1	if south-to-north frame 		
   //8	=      1 	if IMC active 				
   //9	=      1 	if lost header block 			
   //10	=      1 	if lost trailer block 			
   //11	=      1 	if lost telemetry data 			
   //12	=      1 	if (star sense) time break 		
   //13	=      0	if side 1 (primary) active 		
   //	        1 	if side 2 (secondary) active 		
   //14	=      1 	if visible normalization active 	
   //15	=      1 	if IR calibration active                
 

  int l1scan() { return( L1scan ) ; }
  int l2scan() { return( L2scan ) ; }
  int n_pixels() { return( ((int)LpixlsMsb << 10) + LpixlsLsb ); } 
  int n_words() { return( ((int)LwordsMsb << 10) + LwordsLsb ); } 
  int lzcor() { return( Lzcor ); }

  int channel() { return (channel_id -1) ; }

  int detector(){ return( (channel_id == 1) ? (detector_id+3)&7 : (detector_id-1)&1); }
// channel	logical det	normal detector_id	flip detector_id
// visible: norm: det = (detector_id+3)&7  flip: det = (12-detector_id)&7
//	1	0		5		4
//	1	1		6		3
//	1	2		7		2
//	1	3		8		1
//	1	4		1		8
//	1	5		2		7
//	1	6		3		6
//	1	7		4		5
// infrared: norm: det = (detector_id-1)&1  flip: det = (chan 3: 0, others: detector_id&1)
//	2	0		5		6
//	2	1		6		5
//	3	0		7		7
//	4	0		1		2
//	4	1		2		1
//	5	0		3		4
//	5	1		4		3
// BUT: the flip bit (bit 16 of Iscan) is not included with linedoc data so
//      I modify detector_id in the case of flip mode so that the above function
//      will return the correct logical detector value.
// visible: detector_id = (det+4)&7+1 = (9-detector_id) where det is the block number - 3
// infrared: detector_id = (2*c+d)%8+1 where c=channel, d=expected detector
// see gvar.C, list.C for more details 
//
// new configuration for GOES-M (GOES-12) and beyond
// ch 3 now has 2 detectors, ch 6 has been added with one detector
// for GOES-O (GOES-14), ch 6 will also have 2 detectors
//	2	0		1		
//	2	1		2		
//	3	0		3		
//	3	1		4		
//	4	0		5		
//	4	1		6		
//	6	0		7		
//	6	1		8		
// GOES-P may go back to channel 5 thusly:
//	5	0		7		
//	5	1		8		

  int side() { return((side_id)?1:0); }
  
  unsigned short * data () { return  &Data; } 
  
  LineDoc* next() { return (LineDoc*) ( (uint16*) this + n_words() ) ; }

  int valid();  
  void print( ostream & ) ; 

  void lineDocText(ostream& );
  void printLineDoc(ostream& );
  void printDframe(ostream& );
  
  int bytes(); 

  float vis_line(ImagerDoc &b0); 
  float rel_vis_line(); 

}; 

inline int LineDoc::valid() {
  return (n_pixels() < n_words() ) ; }

inline int LineDoc::bytes() { return(n_words()*10/8); }  

#endif


