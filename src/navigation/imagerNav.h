
#ifndef INAV_H
#define INAV_H

//#include <stream.h>
#include "include/types.h"
#include "block0/attitudeAngle.h"
#include "block0/biasStatistics.h"
#include "block0/calibration.h"
#include "block0/iscan.h"

/* pg 68 , goes i-m, oge specs */

class ImagerNav{ 
public:

  uint16 rel_scan_count; /*151 152*/
  uint16 abs_scan_count; /*153 154*/
  uint16 N_line_in_scan; /*155 156*/
  uint16 W_pix_of_frame; /*157 158*/
  uint16 E_pix_of_frame; /*159 160*/
  uint16 N_line_of_frame; /*161 162*/
  uint16 S_line_of_frame; /*163 164*/
  uint16 pix_of_zero_az; /*165 166*/
  uint16 line_of_zero_elev; /*167 168*/
  uint16 scan_of_zero_elev; /*169 170*/
  uint16 Line_Of_Subsat;    /*171 172*/
  uint16 Pix_Of_Subsat;     /*173 174*/
  SelFloat  Subsat_Lat; /*175 178*/
  SelFloat  Subsat_Lon; /*179 182*/
  uchar8 Czone; /*183*/
  uchar8 V_Detect_In_Block3; /*184*/
  uint16 G1cnt; /*185 186*/
  uint16 G2cnt; /*187 188*/
  uint16 EQ_Pix_Bias; /*189 190*/
  uint16 NS_Line_Bias; /*191 192*/

  SelFloat NW_lat_of_frame; /*231 234*/
  SelFloat NW_lon_of_frame; /*235 238*/
  SelFloat SE_Lat_Of_Frame; /*239 242*/
  SelFloat SE_lon_of_frame; /*243 246*/

  uchar8 Imc_identifier[4]; /* 279 282*/
  uchar8 w283_94[12]; /* 283 294*/

  SelFloat ReferenceLongitude; /* 295 298*/
  SelFloat ReferenceRadialDistance; /* 299 302*/
  SelFloat ReferenceLatitude; /* 303 306*/
  SelFloat ReferenceOrbitYaw; /* 307 310*/
  SelFloat ReferenceAttitudeRoll; /* 311 314*/
  SelFloat ReferenceAttitudePitch; /* 315 318*/
  SelFloat ReferenceAttitudeYaw; /* 319 322*/

  CdaTime EpochDate; /* 323 330*/

  SelFloat IMCenableFromEpoch; /* 331 334*/
  SelFloat CompensationRoll; /* 335 338*/
  SelFloat CompensationPitch; /* 339 342*/
  SelFloat CompensationYaw; /* 343 346*/
  SelFloat ChangeLongitude[13]; /* 347 398*/
  SelFloat ChangeRadialDistance[11]; /* 399 442*/
  SelFloat SineGeocentricLatitude[9]; /* 443 478*/
  SelFloat SineOrbitYaw[9]; /* 479 514*/
  SelFloat DailySolarRate; /* 515 518*/
  SelFloat ExponentialStartFromEpoch; /* 519 522*/

  AttitudeAngle RollAngle; /* 523 742*/
  AttitudeAngle PitchAngle; /* 743 962*/
  AttitudeAngle YawAngle; /* 963 1182*/
  AttitudeAngle RollMisalignment; /*1183 1402*/
  AttitudeAngle PitchMisalignment; /*1403 1662*/

  uchar8 w1623_625[3]; /*1623 1625*/
  uchar8 Parity_279_1625; /*1626 */


  uchar8 Coregisteration_table_id[4]; /*1627 1630*/
  uchar8 East_west_half_hourly_correction_terms[48]; /*1631 1678*/
  uchar8 Index_of_correction_term_currently_active; /*1679 */


public: 
  ImagerNav();

  int w_pix_of_frame(); // Western edge pixel
  int e_pix_of_frame(); // Eastern edge pixel
  int n_line_of_frame(); // Northern edge pixel
  int s_line_of_frame(); // Sothern edge pixel
  
  int imc(); 

};

inline int ImagerNav::w_pix_of_frame() { return W_pix_of_frame; } 
inline int ImagerNav::e_pix_of_frame() { return E_pix_of_frame; } 
inline int ImagerNav::n_line_of_frame() { return N_line_of_frame; } 
inline int ImagerNav::s_line_of_frame() { return S_line_of_frame; } 

#endif // INAV_H



