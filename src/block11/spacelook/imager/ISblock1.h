#ifndef ISblock1_h
#define ISblock1_h

#include <string.h>

#include "utils/cdaTime/cdaTime.h"
#include "include/types.h"
#include "utils/selFloat/selFloat.h"
#include "block11/sadId.h"

class ISblock1{ 
 public:
	CdaTime T_spacelook_header; // 31 38
	CdaTime T_current_limits_set;   // 36 46
	uint16 PreClampLine; // 47 48 
	uint16 PreClampPixel;// 49 50 
	uint16 PostClampLine; // 51 52
	uint16 PostClampPixel; // 53 54 
	
	uint16 SpaceLookWarningFlags[15]; // 55 84
	uint16 PreClampWarningFlags[15]; // 85 114
	uint16 SpaceLookCriticalAlarmFlag; // 115 116 

	uchar8 w117 ; // 117  ??? check this!!!
	uchar8 w118 ; // 118 
	CdaTime T_preclamp_data; // 119 126
	CdaTime T_postclamp_data; // 127 134
	
	uchar8 w135[86]; // 135 220 
	uchar8 SideActive; // 221
	uchar8 Parity222; // 222

// Imager PreClamp Statistics
	uint16 PreClampTotalSize[15]; // 223 252
	uint16 PreClampFilteredSize[15]; // 253 282
	uint16 PreClampUnfilteredMin[15]; // 283 312
	uint16 PreClampFilteredMin[15]; // 313 342
	uint16 PreClampUnfilteredMax[15]; // 343 372
	uint16 PreClampFilteredMax[15]; // 373 402
	SelFloat PreClampUnfilteredMean[15]; // 403 462
	SelFloat PreClampFilteredMean[15]; // 463 522
	SelFloat PreClampUnfilteredStdDev[15]; // 523 582
	SelFloat PreClampFilteredSigma[15]; // 583 642
	SelFloat PreClampFilteredSigmaRadiance[7]; // 643 670
	SelFloat PreClampFilteredSigmaTemperature[7]; // 671 698
	uchar8 w699[3]; // 699 701
	uchar8 Parity702; // 702

// PostClamp Statistics
	uint16 PostClampTotalSize[15]; // 703 732
	uint16 PostClampFilteredSize[15]; // 733 762
	uint16 PostClampUnfilteredMin[15]; // 763 792
	uint16 PostClampFilteredMin[15]; // 793 822
	uint16 PostClampUnfilteredMax[15]; // 823 852
	uint16 PostClampFilteredMax[15]; // 853 882 
	SelFloat PostClampUnfilteredMean[15]; // 883 942 
	SelFloat PostClampFilteredMean[15]; // 943 1002
	SelFloat PostClampUnfilteredStdDev[15]; //1003 1062
	SelFloat PostClampFilteredSigma[15]; // 1063 1122
	SelFloat PostClampFilteredSigmaRadiance[7]; //    1123 1150
	SelFloat PostClampFilteredSigmaTemperature[7]; //1151 1178
	uchar8 w1179[3]; // 1179 1181
	uchar8 Parity1182; // 1182

// Imager Accured IR activity
	SelFloat AccuredPreClampFilteredMean[7][120];//1183 4542
	SelFloat AccuredPostClampFilteredMean[7][120];//4543 7902
	uchar8 w7903[3]; // 7903 7905
	uchar8 Parity7906; // 7902
	uchar8 w7907[138] ; // 7903 8040; 

public:
	ISblock1() { }; 
	void copy(SadId * sad); 
	void printHeader( ostream & out, int channel);
	void printStatistics( ostream & out, int channel);
#ifdef _LSBF_
	void ByteSwap(void);
#endif
};
	
inline void ISblock1::copy( SadId * sad ) 
{
  memcpy( this, sad->data() , sizeof(ISblock1) );
#ifdef _LSBF_
  ByteSwap();
#endif
}

#endif // ImagerSpacelookBlock0_h	
	
