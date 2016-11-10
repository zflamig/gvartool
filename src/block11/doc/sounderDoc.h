#ifndef SOUNDERDOC_H
#define SOUNDERDOC_H

#include "include/types.h"

#include "block0/attitudeAngle.h"
#include "block0/calibration.h"
#include "utils/cdaTime/cdaTime.h"
#include "block11/sscan.h"
#define SOUNDERDOC_BSZ  8042

//     Partition                   Word Range
//SAD ID (see Section 3.3.7.1) 	      1-  30 
//Instrument and Scan Status 	     31- 306 
//Sounder O&A parameters 	    307-1718 
//Factory Parameters		   3005-8040 







class SounderDoc{ 
public:

  char Spcid ;			/* 1 */
  char ground_system_id ;	/* 2 */
  char DataId ;			/* 3 */ 
  char Fbf;			/* 4 */

  char Lbf ;			/* 5 */
  unsigned BlockCount: 24;	/* 6 7 8 */

  char Nrec ;			/* 9 */ 


  char textmessages[11];	/* 10 20 */
  char yaw_flip_flag ;	     	/* 21 */ 

  char spare0[9];		/* 22 30 */

  char junk[2]; // for alignment 



//  start section 2   Instrument and Scan Status, words 31- 306 

  SSCAN Sscan;        // 31 32 
  uchar8 Stdta[10];   // 33 42
  uint16 Srbct;       // 43 44

  uint16 Sgbct;       // 45 46
  uint16 Sloct;       // 47 48

  uint16 Ssbk;        // 49 50
  uint16 Scbrk;       // 51 52

  uchar8 sscan_parity1;   // 53
  uchar8 srelon;          // 54
  uint16 snboost;   // 55 56
  uchar8 yarfl;   // 57

  uchar8 w58_spare;   // 58

  
  CdaTime T_sps_current; // 59 66

  CdaTime T_scanline_start; // 67 74
  CdaTime T_scanline_end; // 75 82

  CdaTime T_cal_set_start; // 83 90
  CdaTime T_cal_set_end; // 91 98

  CdaTime T_priority_framestart; // 99 106
  CdaTime T_normal_framestart; // 107 114

  CdaTime Tsspc; // 115 122
  CdaTime Tsecl; // 123 130
  CdaTime Tsbbc; // 131 138


  CdaTime Tsstr; // 139 146
  CdaTime Tlran; // 147 154
  CdaTime Tsvit; // 155 162
  CdaTime Tclmt; // 163 170
  CdaTime Tsona; // 171 178

  uchar8 w179_180_spare[2]; // 179 180

  uint16 rel_scan_count; // 181 182

  uint16 abs_scan_count; // 183 184

  uint16 N_Line_In_Scan; // 185 186

  uint16 W_pix_of_frame; // 187 188
  uint16 E_pix_of_frame; // 189 190
  uint16 N_line_of_frame; // 191 192
  uint16 S_line_of_frame; // 193 194

  uint16 pix_of_zero_az;    // 195 196

  uint16 line_of_zero_elev; // 197 198
  uint16 scan_of_zero_elev; // 199 200

  uint16 Line_Of_Subsat;    // 201 202
  uint16 Pix_Of_Subsat;     // 203 204

  char w205_206_spare[2]; // 205 206

  SelFloat Subsat_Lat; // 207 210
  SelFloat Subsat_Lon; // 211 214

  uchar8 w215_226_spare[12]; //215 226 

  SelFloat Sdber;  // 227 230
  SelFloat Range;  // 231 234
  SelFloat Gpath;  // 235 238
  SelFloat Xmsne;  // 239 242

  CdaTime Tgpat;  // 243 250
  CdaTime Txmsn;  // 251 258

  uchar8 w259_260_spare[2]; // 259 260

  uchar8 frame_number;  // 261
  uchar8 Smode;  // 262

  SelFloat NW_lat_of_frame;  // 263 266
  SelFloat NW_lon_of_frame;  // 267 270
  SelFloat SE_lat_of_frame;  // 271 274
  SelFloat SE_lon_of_frame;  // 275 278

  uchar8 sg2tn;           // 279
  uchar8 sscan_parity2;   // 280
  SSCAN Sscan_copy2;        // 281 282 

  CdaTime T_current_framestart; // 283-290 version 1+ ???

  uchar8 w290_305_spare[15]; // 291 305


  uchar8 Parity_31_305; // 306 


// end section 2 : Instrument and Scan Status, words 31- 306

// start section 3 : Sounder O&A parameters, words 307-1718 


  /* Orbit and attitude parameters */

  uchar8 ImcId[4]; // 307 310
  uchar8 w311_322_spare[12]; // 311 322

  SelFloat ReferenceLongitude; // 323 326
  SelFloat ReferenceRadialDistance; // 327 330
  SelFloat ReferenceLatitude;  // 331 334
  SelFloat ReferenceOrbitYaw;  // 335 338
  SelFloat ReferenceAttitudeRoll; // 339 342
  SelFloat ReferenceAttitudePitch; // 343 346
  SelFloat ReferenceAttitudeYaw;  // 347 350

  CdaTime  EpochDate;   // 351 358

  SelFloat IMCenableFromEpoch; // 359 362
  SelFloat CompensationRoll; // 363 366
  SelFloat CompensationPitch; // 367 370
  SelFloat CompensationYaw; // 371 374 

  SelFloat ChangeLongitude[13] ; // 375 426
  SelFloat ChangeRadialDistance[11]; // 427 470
  SelFloat SineGeocentricLatitude[9]; // 471 506
  SelFloat SineOrbitYaw[9]; // 507 542
  SelFloat DailySolarRate;  // 543 546
  SelFloat ExponentialStartFromEpoch; // 547 550

  AttitudeAngle RollAngle; // 551 770
  AttitudeAngle PitchAngle; // 771 990
  AttitudeAngle YawAngle;   // 991 1210
  AttitudeAngle RollMisalignment; // 1211 1430 
  AttitudeAngle PitchMisalignment; // 1431 1650

  SSCAN Sscan_copy3;        // 1651 1652 
  uchar8 sscan_parity3;   // 1653


  uchar8 w1651_1717_spare[64]; // 1654 1717
  uchar8 Parity_307_1717; // 1718 


// end section 3 : Sounder O&A parameters, words 307-1718 


  uchar8 w1719_2994_spare[1276]; // 1719 3004
  CdaTime T_spssattim; //2995-3002  Satellite database modified time (BCD time)
  uint16 spssatver; //3003-3004  Satellite database version number



// start section 4 : Factory Parameters, words 3005-8040 

  uchar8 Sofnc; // 3005
  uchar8 Sofec; // 3006
  uint16 Sofni; // 3007 3008
  uint16 Sofei; // 3009 3010

  sint16 XoffsetVis[4];        // 3011 3018
  sint16 XoffsetLongwaveIR[4]; // 3019 3026
  sint16 XoffsetMidwaveIR[4];  // 3027 3034
  sint16 XoffsetShortwave[4];  // 3035 3042

  sint16 YoffsetVis[4];        // 3043
  sint16 YoffsetLongwaveIR[4]; // 
  sint16 YoffsetMidwaveIR[4];  // 
  sint16 YoffsetShortwave[4];  //       3074

  SelFloat Svcrb[4]; // 3075 3090
  SelFloat Svcr1[4]; // 3091 3106
  SelFloat Svcr2[4]; // 3107 3122
  SelFloat Svral; // 3123 3126

  SelFloat Sicrb[18][4]; // 3127 3414
  SelFloat Sicr1[18][4]; // 3415 3702
  SelFloat Sicr2[18][4]; // 3703 3990
  SelFloat Sisfb[18][4]; // 3991 4278
  SelFloat Sisf1[18][4]; // 4279 4556

  SelFloat Sg2it[18][4][4]; // 4567 5718
  SelFloat Sg2bp[4];        // 5719 5734
  SelFloat Sbbtr[18][4][4]; // 5753 6886
  SelFloat Sprng[3][2]; //  6887 - 6910 

  uint16 sadcp; //6911-6912
  uint16 sadcn; //6913-6914 

  uchar8 w6915_7342[428]; // 6915- 7342

  SelFloat Schta_f[6];        // 7343-7366

  Calibration Sounder; /* 7367 8014 */ 

  SelFloat FilterWheelHeaterGain; /* 8015 8018 */
  SelFloat FilterWheelHeaterBias; /* 8019 8022 */

  SelFloat PatchControlVoltageGain; /* 8023 8026 */
  SelFloat PatchControlVoltageBias; /* 8027 8030 */

  SelFloat InstrumentCurrentGain; /* 8031 8034 */
  SelFloat InstrumentCurrentBias; /* 8035 8038 */

  uchar8 w8039_spare ; 
  uchar8 Parity_3005_8039 ; 

// end section 4 : Factory Parameters, words 3005-8040 

  uint16 CRC; 

public:
  SounderDoc() ;

  void copy(void * in );  
  
  uint16 crc() ; 
  char parity_31_305(); 
  char parity_307_1717();
  char parity_3005_8039(); 

  int spcid(); 
  int imc(); 

  int instrument();

  float nlon(); 
  float slon();
  float elat(); 
  float wlat();

  void Ddump_all(ostream & out); 
  void ByteSwap();
};

inline uint16 SounderDoc::crc() 
{ return CRC; }

inline int SounderDoc::spcid() { return Spcid; }

inline int SounderDoc::instrument() { return sounder; }



#endif // SOUNDERDOC_H
