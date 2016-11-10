#include "ISblock1.h"

void ISblock1::printHeader(ostream & out , int channel) { 

	out << "Imager Space Look Statistics for channel "
		<< channel + 1 << "\n" ;

	out << "Detector\t"; 
  out << "Time\t";
	out << "PreClampLine\t";
	out << "PreClampPixel\t";
	out << "PreClampTotalSize\t";
	out << "PreClampFilteredSize\t";
	out << "PreClampUnfilteredMin\t";
	out << "PreClampFilteredMin\t";
	out << "PreClampUnfilteredMax\t";
	out << "PreClampFilteredMax\t";
	out << "PreClampUnfilteredMean\t";
	out << "PreClampFilteredMean\t";
	out << "PreClampUnfilteredStdDev\t";
	out << "PreClampFilteredSigma(Counts)\t";
	out << "PreClampFilteredSigma(radiance)\t";
	out << "PreClampFilteredSigma(temperature)\t";
	out << "PostClampLine\t";
	out << "PostClampPixel\t";
	out << "PostClampTotalSize\t";
	out << "PostClampFilteredSize\t";
	out << "PostClampUnfilteredMin\t";
	out << "PostClampFilteredMin\t";
	out << "PostClampUnfilteredMax\t";
	out << "PostClampFilteredMax\t";
	out << "PostClampUnfilteredMean\t";
	out << "PostClampFilteredMean\t";
	out << "PostClampUnfilteredStdDev\t";
	out << "PostClampFilteredSigma(Counts)\t";
	if ( channel != 0 ) { 
		out << "PostClampFilteredSigma(radiance)\t";
		out << "PostClampFilteredSigma(temperature)\t";
	}
	out << "\n";
}
	
		
void ISblock1::printStatistics(ostream & out, int channel){ 

	static int D0[] = {7, 4, 6, 0, 2 };
	static int D1[] = {15,6, 7, 2, 4 };

	for ( int d = D0[channel]; d < D1[channel]; d++){
		out << (d - D0[channel]) ; 
		out << "\t" << T_current_limits_set;
		out << "\t" << PreClampLine; 
		out << "\t" << PreClampPixel;
		out << "\t" << PreClampTotalSize[d];
		out << "\t" << PreClampFilteredSize[d];
		out << "\t" << PreClampUnfilteredMin[d];
		out << "\t" << PreClampFilteredMin[d];
		out << "\t" << PreClampUnfilteredMax[d];
		out << "\t" << PreClampFilteredMax[d];
		out << "\t" << (float)PreClampUnfilteredMean[d];
		out << "\t" << (float)PreClampFilteredMean[d];
		out << "\t" << (float)PreClampUnfilteredStdDev[d];
		out << "\t" << (float)PreClampFilteredSigma[d];
		out << "\t" << (float)PreClampFilteredSigma[d];
		out << "\t" << (float)PreClampFilteredSigma[d];
		out << "\t" << PostClampLine;
		out << "\t" << PostClampPixel;
		out << "\t" << PostClampTotalSize[d];
		out << "\t" << PostClampFilteredSize[d];
		out << "\t" << PostClampUnfilteredMin[d];
		out << "\t" << PostClampFilteredMin[d];
		out << "\t" << PostClampUnfilteredMax[d];
		out << "\t" << PostClampFilteredMax[d];
		out << "\t" << (float)PostClampUnfilteredMean[d];
		out << "\t" << (float)PostClampFilteredMean[d];
		out << "\t" << (float)PostClampUnfilteredStdDev[d];
		out << "\t" << (float)PostClampFilteredSigma[d];
		if ( channel != 0 ) { 
			out << "\t" << (float)PostClampFilteredSigmaRadiance[d];
			out << "\t" << (float)PostClampFilteredSigmaTemperature[d];
		}
		out << "\n";
	}
}

#ifdef _LSBF_
void ISblock1::ByteSwap(void)
{
  int i,j,k,l;
  unsigned char *dp = (unsigned char *)&T_spacelook_header;
// CdaTime T_spacelook_header;                    //  31   38 B 0 7
// CdaTime T_current_limits_set;                  //  39   46 B 8 15

// uint16 PreClampLine;                           //  47   48 B 16 17
// uint16 PreClampPixel;                          //  49   50 B 18 19
// uint16 PostClampLine;                          //  51   52 B 20 21
// uint16 PostClampPixel;                         //  53   54 B 22 23
// uint16 SpaceLookWarningFlags[15];              //  55   84 B 24 53
// uint16 PreClampWarningFlags[15];               //  85  114 B 54 83
// uint16 SpaceLookCriticalAlarmFlag;             // 115  116 B 84 85
  i=16;
  while (i<=85)
  {
    l=i+1;
    dp[i] ^= dp[l];
    dp[l] ^= dp[i];
    dp[i] ^= dp[l];
    i+=2;
  }

// uchar8 w117 ;                                  // 117 B 86 ??? check this!!!
// uchar8 w118 ;                                  // 118      B 87
// CdaTime T_preclamp_data;                       // 119  126 B 88 95
// CdaTime T_postclamp_data;                      // 127  134 B 96 103
// uchar8 w135[86];                               // 135  220 B 104 189
// uchar8 SideActive;                             // 221 B 190
// uchar8 Parity222;                              // 222 B 191

// uint16 PreClampTotalSize[15];                  // 223  252 B 192 221
// uint16 PreClampFilteredSize[15];               // 253  282 B 222 251
// uint16 PreClampUnfilteredMin[15];              // 283  312 B 252 281
// uint16 PreClampFilteredMin[15];                // 313  342 B 282 311
// uint16 PreClampUnfilteredMax[15];              // 343  372 B 312 341
// uint16 PreClampFilteredMax[15];                // 373  402 B 342 371
  i=192;
  while (i<=371)
  {
    l=i+1;
    dp[i] ^= dp[l];
    dp[l] ^= dp[i];
    dp[i] ^= dp[l];
    i+=2;
  }

// SelFloat PreClampUnfilteredMean[15];           // 403  462 B 372 431
// SelFloat PreClampFilteredMean[15];             // 463  522 B 432 491
// SelFloat PreClampUnfilteredStdDev[15];         // 523  582 B 492 551
// SelFloat PreClampFilteredSigma[15];            // 583  642 B 552 611
// SelFloat PreClampFilteredSigmaRadiance[7];     // 643  670 B 612 639
// SelFloat PreClampFilteredSigmaTemperature[7];  // 671  698 B 640 667
  i=372;
  while (i<=667)
  {
    j=i+1; k=j+1; l=k+1;
    dp[i] ^= dp[l];
    dp[l] ^= dp[i];
    dp[i] ^= dp[l];
    dp[j] ^= dp[k];
    dp[k] ^= dp[j];
    dp[j] ^= dp[k];
    i+=4;
  }

// uchar8 w699[3];                                // 699  701 B 668 670
// uchar8 Parity702;                              // 702 B 671

// uint16 PostClampTotalSize[15];                 // 703  732 B 672 701
// uint16 PostClampFilteredSize[15];              // 733  762 B 702 731
// uint16 PostClampUnfilteredMin[15];             // 763  792 B 732 761
// uint16 PostClampFilteredMin[15];               // 793  822 B 762 791
// uint16 PostClampUnfilteredMax[15];             // 823  852 B 792 821
// uint16 PostClampFilteredMax[15];               // 853  882 B 822 851
  i=672;
  while (i<=851)
  {
    l=i+1;
    dp[i] ^= dp[l];
    dp[l] ^= dp[i];
    dp[i] ^= dp[l];
    i+=2;
  }

// SelFloat PostClampUnfilteredMean[15];          // 883  942 B 852 911
// SelFloat PostClampFilteredMean[15];            // 943 1002 B 912 971
// SelFloat PostClampUnfilteredStdDev[15];        //1003 1062 B 972 1031
// SelFloat PostClampFilteredSigma[15];           //1063 1122 B 1032 1091
// SelFloat PostClampFilteredSigmaRadiance[7];    //1123 1150 B 1092 1119
// SelFloat PostClampFilteredSigmaTemperature[7]; //1151 1178 B 1120 1147
  i=852;
  while (i<=1147)
  {
    j=i+1; k=j+1; l=k+1;
    dp[i] ^= dp[l];
    dp[l] ^= dp[i];
    dp[i] ^= dp[l];
    dp[j] ^= dp[k];
    dp[k] ^= dp[j];
    dp[j] ^= dp[k];
    i+=4;
  }

// uchar8 w1179[3];                               //1179 1181 B 1148 1150
// uchar8 Parity1182;                             //1182      B 1151

// SelFloat AccuredPreClampFilteredMean[7][120];  //1183 4542 B 1152 4511
// SelFloat AccuredPostClampFilteredMean[7][120]; //4543 7902 B 4512 7871
  i=1152;
  while (i<=7871)
  {
    j=i+1; k=j+1; l=k+1;
    dp[i] ^= dp[l];
    dp[l] ^= dp[i];
    dp[i] ^= dp[l];
    dp[j] ^= dp[k];
    dp[k] ^= dp[j];
    dp[j] ^= dp[k];
    i+=4;
  }

// uchar8 w7903[3];                               //7903 7905 B 7872 7874
// uchar8 Parity7906;                             //7902      B 7875
// uchar8 w7907[138] ;                            //7903 8040 B 7876 8013

};
#endif
