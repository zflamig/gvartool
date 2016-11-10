#include <string.h>
#include "sounderDoc.h"
#include "utils/crcCheck/crcCheck.h"
#include "utils/options/options.h"

extern Options Opt;

SounderDoc::SounderDoc()
{ memset(this, 0, SOUNDERDOC_BSZ); }

void SounderDoc::copy(void * in )
{
  unsigned char *c = (unsigned char*) in;
  if(parityCheck(in, 305-1+1 ) == *(c+305)){
    memcpy(&Spcid, in,  30);
    memcpy(&Sscan, (char*)in+30, SOUNDERDOC_BSZ -30 );
#ifdef _LSBF_
    ByteSwap();
#endif
  }
    else if(Opt.debug())
      std::cout << "\nBlock11 word 305 parity check fail";
}

/*
    char *c = ((char*) in + 30);

    if(parityCheck(c+307-1,1717-307+1) == *(c+1717) )
      memcpy(&Sscan+307, c+307, 1717-307+1 );

    if(parityCheck(c+3005-1,8039-3005+1) == *(c+8039) )
      memcpy(&Sscan+3005, c+3005, 8039-3005+1);
    else if(Opt.debug())
      std::cout << "\nBlock11 word 8039 parity check fail";
*/


int SounderDoc::imc()
{ return Sscan.imcStatus();}

void SounderDoc::ByteSwap(void)
{
  int i,j,k,l;
  unsigned char *dp = (unsigned char *)&Spcid;
/* char Spcid;			B 0 */
/* char ground_system_id;	B 1 */
/* char DataId;			B 2 */
/* char Fbf;			B 3 */

/* char Lbf;			B 4 */
/* unsigned BlockCount: 24;	B 5 7*/
  i=5; l=i+2;
  dp[i] ^= dp[l];
  dp[l] ^= dp[i];
  dp[i] ^= dp[l];

/* char Nrec;			B 8 */
/* char spare0[21];		B 9 29 */
/* char junk[2];  for alignment */

/* SSCAN Sscan;        B 30 31*/
  i=32; l=i+1;
  dp[i] ^= dp[l];
  dp[l] ^= dp[i];
  dp[i] ^= dp[l];

/* uchar8 Stdta[10];   B 32 41*/

/* uint16 Srbct;       B 42 43*/
/* uint16 Sgbct;       B 44 45*/
/* uint16 Sloct;       B 46 47*/
/* uint16 Ssbk;        B 48 49*/
/* uint16 Scbrk;       B 50 51*/
  i=44;
  while (i<=53)
  {
    l=i+1;
    dp[i] ^= dp[l];
    dp[l] ^= dp[i];
    dp[i] ^= dp[l];
    i+=2;
  }

/* uchar8 w53_58[6];   B 52 57*/
/* CdaTime T_sps_current; B 58 65*/
/* CdaTime T_scanline_start; B 66 73*/
/* CdaTime T_scanline_end; B 74 81*/
/* CdaTime T_cal_set_start; B 82 89*/
/* CdaTime T_cal_set_end; B 90 97*/
/* CdaTime T_priority_framestart; B 98 105*/
/* CdaTime T_normal_framestart; B 106 113*/
/* CdaTime Tsspc; B 114 121*/
/* CdaTime Tsecl; B 122 129*/
/* CdaTime Tsbbc; B 130 137*/
/* CdaTime Tsstr; B 138 145*/
/* CdaTime Tlran; B 146 153*/
/* CdaTime Tsvit; B 154 161*/
/* CdaTime Tclmt; B 162 169*/
/* CdaTime Tsona; B 170 177*/
/* uchar8 w179_80[2]; B 178 179*/

/* uint16 Rssct; B 180 181*/
/* uint16 Assct; B 182 183*/
/* uint16 N_Line_In_Scan; B 184 185*/
/* uint16 W_pix_of_frame; B 186 187*/
/* uint16 E_pix_of_frame; B 188 189*/
/* uint16 N_line_of_frame; B 190 191*/
/* uint16 S_line_of_frame; B 192 193*/
/* uint16 pix_of_zero_az;     B 194 195*/
/* uint16 line_of_zero_elev;  B 196 197*/
/* uint16 scan_of_zero_elev;  B 198 199*/
/* uint16 Line_Of_Subsat;     B 200 201*/
/* uint16 Pix_Of_Subsat;      B 202 203*/
  i=182;
  while (i<=205)
  {
    l=i+1;
    dp[i] ^= dp[l];
    dp[l] ^= dp[i];
    dp[i] ^= dp[l];
    i+=2;
  }

/* char w205_206[2]; B 204 205*/

/* SelFloat Subsat_Lat; B 206 209*/
/* SelFloat Subsat_Lon; B 210 213*/
  i=208;
  while (i<=215)
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

/* uchar8 w215_226[12]; B214 225*/

/* SelFloat Sdber;  B 226 229*/
/* SelFloat Range;  B 230 233*/
/* SelFloat Gpath;  B 234 237*/
/* SelFloat Xmsne;  B 238 241*/
  i=228;
  while (i<=243)
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

/* CdaTime Tgpat;  B 242 249*/
/* CdaTime Txmsn;  B 250 257*/
/* uchar8 w259_260[2]; B 258 259*/
/* uchar8 frame_number;  B 260*/
/* uchar8 Smode;  B 261*/

/* SelFloat NW_lat_of_frame;  B 262 265*/
/* SelFloat NW_lon_of_frame;  B 266 269*/
/* SelFloat SE_lat_of_frame;  B 270 273*/
/* SelFloat SE_lon_of_frame;  B 274 277*/
  i=264;
  while (i<=279)
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

/* uchar8 w279_305[27]; B 278 304*/
/* uchar8 Parity_31_305; B 305*/
/* uchar8 ImcId[4]; B 306 309*/
/* uchar8 w311_322[12]; B 310 321*/

/* SelFloat ReferenceLongitude;        B 322 325*/
/* SelFloat ReferenceRadialDistance;   B 326 329*/
/* SelFloat ReferenceLatitude;         B 330 333*/
/* SelFloat ReferenceOrbitYaw;         B 334 337*/
/* SelFloat ReferenceAttitudeRoll;     B 338 341*/
/* SelFloat ReferenceAttitudePitch;    B 342 345*/
/* SelFloat ReferenceAttitudeYaw;      B 346 349*/
  i=324;
  while (i<=351)
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

/* CdaTime  EpochDate;   B 350 357*/

/* SelFloat IMCenableFromEpoch;        B 358 361*/
/* SelFloat CompensationRoll;          B 362 365*/
/* SelFloat CompensationYaw;           B 366 369*/
/* SelFloat CompensationPitch;         B 370 373*/
/* SelFloat ChangeLongitude[13];       B 374 425*/
/* SelFloat ChangeRadialDistance[11];  B 426 469*/
/* SelFloat SineGeocentricLatitude[9]; B 470 505*/
/* SelFloat SineOrbitYaw[9];           B 506 541*/
/* SelFloat DailySolarRate;            B 542 545*/
/* SelFloat ExponentialStartFromEpoch; B 546 549*/
  i=360;
  while (i<=551)
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

/* AttitudeAngle RollAngle;         B 550 769*/
   RollAngle.ByteSwap();
/* AttitudeAngle PitchAngle;        B 770 989*/
   PitchAngle.ByteSwap();
/* AttitudeAngle YawAngle;          B 990 1209*/
   YawAngle.ByteSwap();
/* AttitudeAngle RollMisalignment;  B 1210 1429*/
   RollMisalignment.ByteSwap();
/* AttitudeAngle PitchMisalignment; B 1430 1649*/
   PitchMisalignment.ByteSwap();

/* uchar8 w1651_717[67]; B 1650 1716*/
/* uchar8 Parity_307_1717; B 1717*/
/* uchar8 w1719_3004[1286]; B 1718 3003*/
/* uchar8 Sofnc; B 3004*/
/* uchar8 Sofec; B 3005*/

/* uint16 Sofni;                B 3006 3007*/
/* uint16 Sofei;                B 3008 3009*/
/* sint16 XoffsetVis[4];        B 3010 3017*/
/* sint16 XoffsetLongwaveIR[4]; B 3018 3025*/
/* sint16 XoffsetMidwaveIR[4];  B 3026 3033*/
/* sint16 XoffsetShortwave[4];  B 3034 3041*/
/* sint16 YoffsetVis[4];        B 3042 3049*/
/* sint16 YoffsetLongwaveIR[4]; B 3050 3057*/
/* sint16 YoffsetMidwaveIR[4];  B 3058 3065*/
/* sint16 YoffsetShortwave[4];  B 3066 3073*/
  i=3008;
  while (i<=3075)
  {
    l=i+1;
    dp[i] ^= dp[l];
    dp[l] ^= dp[i];
    dp[i] ^= dp[l];
    i+=2;
  }

/* SelFloat Svcrb[4];        B 3074 3089*/
/* SelFloat Svcr1[4];        B 3090 3105*/
/* SelFloat Svcr2[4];        B 3106 3121*/
/* SelFloat Svral;           B 3122 3125*/
/* SelFloat Sicrb[18][4];    B 3126 3415*/
/* SelFloat Sicr1[18][4];    B 3414 3701*/
/* SelFloat Sicr2[18][4];    B 3702 3989*/
/* SelFloat Sisfb[18][4];    B 3990 4277*/
/* SelFloat Sisf1[18][4];    B 4278 4565*/
/* SelFloat Sg2it[18][4][4]; B 4566 5717*/
/* SelFloat Sg2bp[4];        B 5718 5733*/
/* SelFloat Sbbtr[18][4][4]; B 5734 6885*/
/* SelFloat Sprng[3][2];     B 6886 6909*/
  i=3076;
  while (i<=6911)
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

/* uchar8 w6911_7366[456]; B 6910 7365*/

/* Calibration Sounder; B 7366 8013*/
   Sounder.ByteSwap();

/* SelFloat FilterWheelHeaterGain;   B 8014 8017*/
/* SelFloat FilterWheelHeaterBias;   B 8018 8021*/
/* SelFloat PatchControlVoltageGain; B 8022 8025*/
/* SelFloat PatchControlVoltageBias; B 8026 8029*/
/* SelFloat InstrumentCurrentGain;   B 8030 8033*/
/* SelFloat InstrumentCurrentBias;   B 8034 8037*/
  i=8016;
  while (i<=8039)
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

/* uchar8 w8039; B 8038*/
/* uchar8 Parity_3005_8039; B 8039*/

/* uint16 CRC; B 8040 8041*/
  i=8042; l=i+1;
  dp[i] ^= dp[l];
  dp[l] ^= dp[i];
  dp[i] ^= dp[l];

} /* end SounderDoc::ByteSwap */




void SounderDoc::Ddump_all(ostream & out ){

  int i=0;

   out << " \n";									     					   
   out << " \n";									     					   

   out << "  uchar8 Spcid  /* 1 */ 			        "  <<  (int)Spcid                                          << "\n";
   out << "  uchar8 ground_system_id  /* 2 */ 			"  <<  (int)ground_system_id                               << "\n";


   out << "   char DataId ;         /* 3 */                     "  <<  (int)DataId                               << "\n";
   out << "   char Fbf;             /* 4 */                     "  <<    (int)Fbf                                        << "\n";
   out << "   char Lbf ;            /* 5 */                     "  <<    (int)Lbf                                        << "\n";
   out << "   unsigned BlockCount: 24;   /* 6 7 8 */            "  <<    (int)BlockCount                               << "\n";
   out << "   char Nrec ;         /* 9 */                       "  <<    (int)Nrec                                       << "\n";
   out << "   SSCAN Sscan;        // 31 32                      "  <<                                        "\n";

   out << "   uchar8 Stdta[10];   // 33 42                      "  <<    (int)Stdta[i]                                << "\n";
   out << "   uint16 Srbct;       // 43 44                      "  <<   Srbct                                    << "\n";
   out << "   uint16 Sgbct;       // 45 46                      "  <<   Sgbct                                    << "\n";
   out << "   uint16 Sloct;       // 47 48                      "  <<   Sloct                                    << "\n";
   out << "   uint16 Ssbk;        // 49 50                      "  <<   Ssbk                                     << "\n";
   out << "   uint16 Scbrk;       // 51 52                      "  <<   Scbrk                                    << "\n";

   out << "   CdaTime T_sps_current; // 59 66                   "  <<      T_sps_current                         << "\n";
   out << "   CdaTime T_scanline_start; // 67 74                "  <<      T_scanline_start                         << "\n";
   out << "   CdaTime T_scanline_end; // 75 82                  "  <<      T_scanline_end                         << "\n";
   out << "   CdaTime T_cal_set_start; // 83 90                 "  <<      T_cal_set_start                         << "\n";
   out << "   CdaTime T_cal_set_end; // 91 98                   "  <<      T_cal_set_end                         << "\n";

   out << "   CdaTime T_priority_framestart; // 99 106          "  <<      T_priority_framestart                          << "\n";
   out << "   CdaTime T_normal_framestart; // 107 114           "  <<      T_normal_framestart                           << "\n";

   out << "   CdaTime Tsspc; // 115 122                           "  <<      Tsspc                           << "\n";
   out << "   CdaTime Tsecl; // 123 130                           "  <<      Tsecl                           << "\n";
   out << "   CdaTime Tsbbc; // 131 138                           "  <<      Tsbbc                           << "\n";
		           						          
   out << "   CdaTime Tsstr; // 139 146                           "  <<      Tsstr                           << "\n";
   out << "   CdaTime Tlran; // 147 154                           "  <<      Tlran                           << "\n";
   out << "   CdaTime Tsvit; // 155 162                           "  <<      Tsvit                           << "\n";
   out << "   CdaTime Tclmt; // 163 170                           "  <<      Tclmt                           << "\n";
   out << "   CdaTime Tsona; // 171 178                           "  <<      Tsona                           << "\n";

   out << "   uint16 rel_scan_count;  // 181 182                  "  <<     rel_scan_count                              << "\n";
   out << "   uint16 abs_scan_count;  // 183 184                  "  <<     abs_scan_count                              << "\n";
   out << "   uint16 N_Line_In_Scan;           // 185 186                  "  <<     N_Line_In_Scan                                       << "\n";
		                      					                     
   out << "   uint16 W_pix_of_frame;  // 187 188                  "  <<     W_pix_of_frame                              << "\n";
   out << "   uint16 E_pix_of_frame;  // 189 190                  "  <<     E_pix_of_frame                              << "\n";
   out << "   uint16 N_line_of_frame; // 191 192                  "  <<     N_line_of_frame                             << "\n";
   out << "   uint16 S_line_of_frame; // 193 194                  "  <<     S_line_of_frame                             << "\n";
		                      					                     
   out << "   uint16 pix_of_zero_az;               // 195 196                 "  <<      pix_of_zero_az                                          << "\n";

   out << "   uint16 line_of_zero_elev;            // 197 198                 "  <<      line_of_zero_elev                                       << "\n";
   out << "   uint16 scan_of_zero_elev;            // 199 200                 "  <<      scan_of_zero_elev                                       << "\n";

   out << "   uint16 Line_Of_Subsat;               // 201 202                 "  <<      Line_Of_Subsat                                          << "\n";
   out << "   uint16 Pix_Of_Subsat;                // 203 204                 "  <<      Pix_Of_Subsat                                           << "\n";

   out << "   SelFloat Subsat_Lat;    // 207 210                 "  <<     Subsat_Lat                              << "\n";
   out << "   SelFloat Subsat_Lon;    // 211 214                 "  <<     Subsat_Lon                              << "\n";
		                    					                
   out << "   SelFloat Sdber;         // 227 230                 "  <<     Sdber                                   << "\n";
   out << "   SelFloat Range;         // 231 234                 "  <<     Range                                   << "\n";
   out << "   SelFloat Gpath;         // 235 238                 "  <<     Gpath                                   << "\n";
   out << "   SelFloat Xmsne;         // 239 242                 "  <<     Xmsne                                   << "\n";
		       	                   				   	            
   out << "   CdaTime  Tgpat;          // 243 250                "  <<     Tgpat                                   << "\n";
   out << "   CdaTime  Txmsn;          // 251 258                "  <<     Txmsn                                   << "\n";

   out << "   uchar8 frame_number;  // 261                       "  <<     (int)frame_number                            << "\n";
   out << "   uchar8 Smode;  // 262                              "  <<     (int)Smode                            << "\n";

   out << "   SelFloat NW_lat_of_frame;  // 263 266              "  <<     NW_lat_of_frame                            << "\n";
   out << "   SelFloat NW_lon_of_frame;  // 267 270              "  <<     NW_lon_of_frame                            << "\n";
   out << "   SelFloat SE_lat_of_frame;  // 271 274              "  <<     SE_lat_of_frame                            << "\n";
   out << "   SelFloat SE_lon_of_frame;  // 275 278              "  <<     SE_lon_of_frame                            << "\n";

   out << "   uchar8 Sofnc; // 3005                              "  <<      (int)Sofnc                           << "\n";
   out << "   uchar8 Sofec; // 3006                              "  <<      (int)Sofec                           << "\n";
   out << "   uint16 Sofni; // 3007 3008                         "  <<      Sofni                           << "\n";
   out << "   uint16 Sofei; // 3009 3010                         "  <<      Sofei                           << "\n";

   out << " \n";
   out << " \n";
   out << " \n";


}


