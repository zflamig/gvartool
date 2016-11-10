#include <string.h>
#include <iomanip>
#include "imagerDoc.h"
#include "utils/crcCheck/crcCheck.h"
#include "utils/options/options.h"
#include "gvarBlock/gvarBlock.h"

extern Options Opt; 
extern GvarBlock gvar;

int ImagerDoc::imc()
{ return Iscan.imcStatus();}

ImagerDoc::ImagerDoc(){
 memset(this, 0, BLOCK0_BSZ) ; }




int ImagerDoc::check_copy(void * in )
{

   int block_status   = 0;
   int section_status = 0;
   int sub_status = 0;

   block_status = (crcCheck(in, BLOCK0_BSZ) == MAGIC_CRC);
   // 0 is fail
   if (block_status == 0) {

     //std::cout << "block id = " << gvar.blockId() << ", count = " << gvar.blockCount();
     //std::cout << ": Crc Check failed (in imagerdoc copy subroutine)\n" << std::flush;

      section_status = parity_check(in); // try to recover
      
      // fail ==1, pass=0
      //for (int j=0; j<6; j++) {
      //  sub_status += ( (section_status & (2 << j)) ==  (2 << j));
      //}
      //std::cout << sub_status << "  sections of block 0 failed parity check\n";


      if ((section_status & 2) == 2) {
	//std::cout << "  section 1 of block 0, count = " << gvar.blockCount() << " failed parity check\n";
	 // check some key values
      } else {
        // for now, if section 1 passes parity assume it's values are ok
        // most of the code only uses values from section 1
	block_status = 1;
      }


   }

   return(block_status);
   
}




int ImagerDoc::copy(void * in)
{
      memcpy(&Spcid, in, BLOCK0_BSZ );

#ifdef _LSBF_
    ByteSwap();
#endif

      return(TRUE);
}

// same as copy but minus the byteswap
int ImagerDoc::duplicate(void * in)
{
      memcpy(&Spcid, in, BLOCK0_BSZ );

      return(TRUE);
}






//The GVAR Block 0 information field is 64,320 bits in length.  
//It consists of 8040 8-bit words 
//divided into the following sections which each have their own parity words: 
// 
//Words Description 
//   0- 277 Instrument and Scan Status 
// 278-1625 Instrument Orbit and Attitude (O&A) Data 
//1626-2305 Scan Reference Data 
//2306-5385 Grid Data 
//5386-6303 Scan Reference and Calibration Data 
//6304-8039 Factory Parameters/8th IR detector Calibration Data 

int ImagerDoc::parity_check(void * in ){

  char *c = (char*) in;
  int j=0;
  int a[6] = {    0,  278, 1626, 2306, 5386, 6304};
  int b[6] = {  277, 1625, 2305, 5385, 6303, 8039};

  // default everything is bad
  int iss_status = 126; //bits 1-7 on

  for (j=0; j<6; j++) {
    if (  parityCheck(c+a[j], b[j]-a[j]) == *(c+b[j])  )  {
       iss_status -= (2 << j); // set bit flag for this section to 0 to indicate it's good
       //memcpy(&Spcid+a[j], c+a[j], b[j]-a[j]+1);
    }
  }

  return(iss_status);
}



void ImagerDoc::NavigationParameters(ostream & out){
  
  out.setf(std::ios::left | std::ios::showpoint | std::ios::fixed);

  out << std::setw(19)  << Iscan.imcStatus()               << "imc        " << "\tIMC status (1 == on)\n";
  out << std::setw(19)  << W_pix_of_frame                  << "x1         " << "\tWestern-most visible pixel in current frame\n";
  out << std::setw(19)  << E_pix_of_frame                  << "x2         " << "\tEastern-most visible pixel in current frame\n"; 
  out << std::setw(19)  << N_line_of_frame                 << "y1         " << "\tNorthern-most visible pixel in current frame\n";  
  out << std::setw(19)  << S_line_of_frame                 << "y2         " << "\tSouthern-most visible pixel in current frame\n";  
  out << std::setw(19)  << pix_of_zero_az                  << "x_zero     " << "\tpixel of zero azimuth\n"; 
  out << std::setw(19)  << line_of_zero_elev               << "y_zero     " << "\tline of zero elevation\n";  
  out << std::setw(19)  << (double)ReferenceLongitude      << "ref_lon    " << "\tReference Longitude (positive east, radians) \n";
  out << std::setw(19)  << (double)ReferenceRadialDistance << "ref_dist   " << "\tReference Radial Distance from nominal (kilometers [km]) \n";
  out << std::setw(19)  << (double)ReferenceLatitude       << "ref_lat    " << "\tReference Latitude (positive north, radians)\n";
  out << std::setw(19)  << (double)ReferenceOrbitYaw       << "ref_yaw    " << "\tReference Orbit Yaw (radians) \n";
  out << std::setw(19)  << (double)ReferenceAttitudeRoll   << "att_roll   " << "\tReference Attitude Roll (radians) \n";
  out << std::setw(19)  << (double)ReferenceAttitudePitch  << "att_pitch  " << "\tReference Attitude Pitch (radians) \n";
  out << std::setw(19)  << (double)ReferenceAttitudeYaw    << "att_yaw    " << "\tReference Attitude Yaw (radians) \n";

  out << "\n\n";
}



void ImagerDoc::NavigationFiles(ostream & out1, ostream & out2){



  out1.setf(std::ios::left | std::ios::showpoint | std::ios::fixed);

  out1 << std::setw(19) <<                         NW_lat_of_frame             << "DLAT       INPUT LATITUDE\n\n";
  out1 << std::setw(19) <<                         NW_lon_of_frame             << "DLON       INPUT LONGITUDE\n\n";
  out1 << std::setw(19) << std::setprecision(2) << float(N_line_of_frame)      << "L(1)       INPUT LINE FOR IMAGER\n\n";
  out1 << std::setw(19) << std::setprecision(2) << float(477)        << "L(2)       INPUT LINE FOR SOUNDER\n\n";
  out1 << std::setw(19) << std::setprecision(4) << float(W_pix_of_frame)      << "IP(1)      INPUT PIXEL FOR IMAGER\n\n";
  out1 << std::setw(19) << std::setprecision(4) << float(910)        << "IP(2)      INPUT PIXEL FOR SOUNDER\n\n";
  out1 << std::setw(19) <<                         2                 << "KEY        1-LAT/LONG-->LINE/PIX  2-LINE/PIX-->LAT/LONG\n\n";
  out1 << std::setw(19) <<                         Iscan.imcStatus() << "IMC        0-ON, 1-OFF(COMPUTE CHANGES IN ORBIT)\n\n";
  out1 << std::setw(19) <<                         1                 << "INSTR      1-IMAGER, 2-SOUNDER\n\n";

  out1.unsetf(std::ios::left);
  out1 << std::setfill('0');

  out1 << std::setw(4)  <<                          EpochDate.year()                                << "               NYE        EPOCH YEAR\n\n";
  out1 << std::setw(3)  <<                          EpochDate.day()                                 << "                NDE        EPOCH DAY\n\n";
  out1 << std::setw(2)  <<                          EpochDate.hrs()                                 << "                 NHE        EPOCH HOUR\n\n";
  out1 << std::setw(2)  <<                          EpochDate.min()                                 << "                 NME        EPOCH MINUTES\n\n";
  out1 << std::setw(6)  << std::setprecision(3) << (float)EpochDate.sec() + EpochDate.msec()/1000.0 << "             SE         EPOCH SECONDS\n\n";
  out1 << std::setw(4)  <<                          T_current_header.year()                                    << "               NY         CURRENT YEAR\n\n";
  out1 << std::setw(3)  <<                          T_current_header.day()                                     << "                ND         CURRENT DAY\n\n";
  out1 << std::setw(2)  <<                          T_current_header.hrs()                                     << "                 NH         CURRENT HOUR\n\n";
  out1 << std::setw(2)  <<                          T_current_header.min()                                     << "                 NM         CURRENT MINUTES\n\n";
  out1 << std::setw(6)  << std::setprecision(3) << (float)T_current_header.sec() + T_current_header.msec()/1000.0         << "             S          CURRENT SECONDS\n\n";

  out1.setf(std::ios::left | std::ios::showpoint | std::ios::fixed);
  out1 << std::setfill(' ');


  out1 << std::setw(19) << (uint)ew_cycles   << "IEW_NAD_CY  IMAGER  EW NADIR POSITION (CYCLES PORTION)\n\n";
  out1 << std::setw(19) << ew_incr         << "IEW_NAD_INC IMAGER  EW NADIR POSITION (INCREMENTS PORTION)\n\n";
  out1 << std::setw(19) << (uint)ns_cycles   << "INS_NAD_CY  IMAGER  NS NADIR POSITION (CYCLES PORTION)\n\n";
  out1 << std::setw(19) << ns_incr         << "INS_NAD_INC IMAGER  NS NADIR POSITION (INCREMENTS PORTION)\n\n";

  out1 << std::setw(19) << 2             << "SEW_NAD_CY  SOUNDER EW NADIR POSITION (CYCLES PORTION)\n\n";
  out1 << std::setw(19) << 1293          << "SEW_NAD_INC SOUNDER EW NADIR POSITION (INCREMENTS PORTION)\n\n";
  out1 << std::setw(19) << 4             << "SNS_NAD_CY  SOUNDER NS NADIR POSITION (CYCLES PORTION)\n\n";
  out1 << std::setw(19) << 868           << "SNS_NAD_INC SOUNDER NS NADIR POSITION (INCREMENTS PORTION)\n\n";                                  




  EpochDate.print(out2);
  out2 << "\tEpoch Time\n";
  T_current_header.print(out2);
  out2 << "\tMost Recent Header Time\n";
  T_sps_current.print(out2);
  out2 << "\tCurrent SPS Time\n";


  out2.setf(std::ios::left | std::ios::showpoint | std::ios::fixed);
  out2.precision(14);
  out2 << std::setfill(' ');

  out2 << std::setw(24) << (uint)instrument() << "Instrument\n";


  out2 << std::setw(24) << (double)ReferenceLongitude            << "Reference Longitude\n";
  out2 << std::setw(24) << (double)ReferenceRadialDistance       << "Reference Radial Distance\n";
  out2 << std::setw(24) << (double)ReferenceLatitude             << "Reference Radial Latitude\n";
  out2 << std::setw(24) << (double)ReferenceOrbitYaw             << "Reference Orbit Yaw\n";

  out2 << std::setw(24) << (double)ReferenceAttitudeRoll         << "ReferenceAttitudeRoll\n";
  out2 << std::setw(24) << (double)ReferenceAttitudePitch        << "Reference Attitude Pitch\n";
  out2 << std::setw(24) << (double)ReferenceAttitudeYaw          << "ReferenceAttitudeYaw\n";

  out2 << std::setw(24) << EpochDate.ieeea()                     << "Epoch Date\n";
  out2 << std::setw(24) << EpochDate.ieeeb()                     << "Epoch Date\n";

  out2 << std::setw(24) << (double)IMCenableFromEpoch            << "IMC enable from epoch\n";
  out2 << std::setw(24) << (double)CompensationRoll              << "CompensationRoll\n";
  out2 << std::setw(24) << (double)CompensationPitch             << "Compensation Pitch\n";
  out2 << std::setw(24) << (double)CompensationYaw               << "Compensation Yaw\n";

  for(int i = 0; i < 13; i++){
    out2 << std::setw(24) << (double)ChangeLongitude[i]          << "Change Longitude "<< i << "\n";
  }
  for (int j=0; j < 11 ; j++){
    out2 << std::setw(24) << (double)ChangeRadialDistance[j]     << "Change Radial Distance " << j << "\n";
  }
  for(int k=0; k < 9; k++){
    out2 << std::setw(24) << (double)SineGeocentricLatitude[k]   << "Change Geocentric Latitude " << k << "\n";
  }
  for(int k=0; k < 9; k++){
    out2 << std::setw(24) << (double)SineOrbitYaw[k]             << "Sine Orbit Yaw " << k << "\n";
  }
  out2 << std::setw(24) << (double)DailySolarRate                << "Daily Solar Rate\n";
  out2 << std::setw(24) << (double)ExponentialStartFromEpoch     << "Exponential Start From Epoch\n";
  
  //  out2 << "\nRoll Angle\n";
  out2 << RollAngle;

  //out2 <<"\nPitch Angle\n";
  out2 << PitchAngle;
  
  //out2 << "\nYaw Angle\n";
  out2 << YawAngle;
  
  //out2 << "\nRoll Misalignment\n";
  out2 << RollMisalignment;
  
  //out2 << "\nPitch Misalignment\n";
  out2 << PitchMisalignment;

  // these aren't actually needed for gimloc
  out2 << std::setw(24) << N_Line_In_Scan  << "Northern-most visible detector scan line in current scan\n";
  out2 << std::setw(24) << W_pix_of_frame  << "Western-most visible pixel in current frame\n";
  out2 << std::setw(24) << E_pix_of_frame  << "Eastern-most visible pixel in current frame\n";
  out2 << std::setw(24) << N_line_of_frame  <<"Northern-most visible pixel in current frame\n";
  out2 << std::setw(24) << S_line_of_frame  <<"Southern-most visible pixel in current frame\n";  
    
  for ( int l = 0; l < 4; l++)
    out2 << Imc_identifier[l];
  out2 << "                    IMC identifier \n";


  out2 << "\n\n";
}


void ImagerDoc::block0time(ostream & out ){
  out <<rel_scan_count<<"\t"<<T_sps_current<<"\t"<<T_current_header<<"\t"<<T_current_trailer<<"\t"<<T_lagged_header<<"\t"
    <<T_lagged_trailer<<"\t"<<T_priority_framestart<<"\t"<<T_normal_framestart<<"\t"<<Tispc<<"\t"
      <<Tiecl<<"\t"<<Tibbc<<"\t"<<Tistr<<"\t"<<Tlran<<"\t"
	<<Tiirt<<"\t"<<Tivit<<"\t"<<Tclmt<<"\t"<<Tiona;
}

void ImagerDoc::Dframe(ostream & out ){
  out << "\t" << rel_scan_count << "\t";
  out << abs_scan_count << "\t";
  out << N_Line_In_Scan << "\t";
}


void ImagerDoc::block0timeText(ostream & out){
  /*		out << "Gvar Imager Documentation Block0 Format Definition";
		out << "OGE Interface Spec DRL 504-02-1 Rev C; table 6.3 pp 67 \n";
		*/
  out<<"rel_scan_count\t";
  out<<"T_sps_current-y\tT_sps_current-d\tT_sps_current-h\tT_sps_current-m\tT_sps_current-s\tT_sps_current-ms\tT_sps_current-f\t";
  out<<"T_current_header-y\tT_current_header-d\tT_current_header-h\tT_current_header-m\tT_current_header-s\tT_current_header-ms\tT_current_header-f\t";
  out<<"T_current_trailer-y\tT_current_trailer-d\tT_current_trailer-h\tT_current_trailer-m\tT_current_trailer-s\tT_current_trailer-ms\tT_current_trailer-f\t";
  out<<"T_lagged_header-y\tT_lagged_header-d\tT_lagged_header-h\tT_lagged_header-m\tT_lagged_header-s\tT_lagged_header-ms\tT_lagged_header-f\t";

  out<<"T_lagged_trailer-y\tT_lagged_trailer-d\tT_lagged_trailer-h\tT_lagged_trailer-m\tT_lagged_trailer-s\tT_lagged_trailer-ms\tT_lagged_trailer-f\t";
  out<<"T_priority_framestart-y\tT_priority_framestart-d\tT_priority_framestart-h\tT_priority_framestart-m\tT_priority_framestart-s\tT_priority_framestart-ms\tT_priority_framestart-f\t";
  out<<"T_normal_framestart-y\tT_normal_framestart-d\tT_normal_framestart-h\tT_normal_framestart-m\tT_normal_framestart-s\tT_normal_framestart-ms\tT_normal_framestart-f\t";
  out<<"Tispc-y\tTispc-d\tTispc-h\tTispc-m\tTispc-s\tTispc-ms\tTispc-f\t";

  out<<"Tiecl-y\tTiecl-d\tTiecl-h\tTiecl-m\tTiecl-s\tTiecl-ms\tTiecl-f\t";
  out<<"Tibbc-y\tTibbc-d\tTibbc-h\tTibbc-m\tTibbc-s\tTibbc-ms\tTibbc-f\t";
  out<<"Tistr-y\tTistr-d\tTistr-h\tTistr-m\tTistr-s\tTistr-ms\tTistr-f\t";
  out<<"Tlran-y\tTlran-d\tTlran-h\tTlran-m\tTlran-s\tTlran-ms\tTlran-f\t";

  out<<"Tiirt-y\tTiirt-d\tTiirt-h\tTiirt-m\tTiirt-s\tTiirt-ms\tTiirt-f\t";
  out<<"Tivit-y\tTivit-d\tTivit-h\tTivit-m\tTivit-s\tTivit-ms\tTivit-f\t";
  out<<"Tclmt-y\tTclmt-d\tTclmt-h\tTclmt-m\tTclmt-s\tTclmt-ms\tTclmt-f\t";
  out<<"Tiona-y\tTiona-d\tTiona-h\tTiona-m\tTiona-s\tTiona-ms\tTiona-f";

}


int ImagerDoc::line_to_scan(int this_line) {

  int this_scan;
  int curr_diff;

  curr_diff = abs_scan_count*8 - N_Line_In_Scan;

  this_scan = (this_line + curr_diff  )/8.;

  return this_scan;
}

// line is northernmost visible line in scan
int ImagerDoc::scan_to_line(int this_scan) {

  int this_line;
  int curr_diff;

  curr_diff = abs_scan_count*8 - N_Line_In_Scan;


  this_line = this_scan*8.0 - curr_diff;

  return this_line;
}


int ImagerDoc::pix_to_aligned_pix(int this_pix) {

  int aligned_pix;

  aligned_pix = this_pix  - (  (this_pix - W_pix_of_frame  ) % 4);

  return aligned_pix;
}


int ImagerDoc::line_to_aligned_line(int this_line) {

  int aligned_line;

  aligned_line = this_line  - (  (this_line - N_line_of_frame  ) % 8);

  return aligned_line;
}


void ImagerDoc::ByteSwap(void)
{
  int i,j,k,l;
  unsigned char *dp = (unsigned char *)&Spcid;

/* ISCAN Iscan B 2-5 */
  i = 2;
  j = 3;
  k = 4;
  l = 5;

  dp[i] ^= dp[l];
  dp[l] ^= dp[i];
  dp[i] ^= dp[l];

  dp[j] ^= dp[k];
  dp[k] ^= dp[j];
  dp[j] ^= dp[k];

/* uint16 rel_scan_count; B 150 151*/
/* uint16 abs_scan_count; B 152 153*/
/* uint16 N_Line_In_Scan; B 154 155*/
/* uint16 W_pix_of_frame; B 156 157*/
/* uint16 E_pix_of_frame; B 158 159*/
/* uint16 N_line_of_frame; B 160 161*/
/* uint16 S_line_of_frame; B 162 163*/
/* uint16 pix_of_zero_az;    b 164 165*/
/* uint16 line_of_zero_elev; b 166 167*/
/* uint16 scan_of_zero_elev; b 168 169*/
/* uint16 Line_Of_Subsat;    B 170 171*/
/* uint16 Pix_Of_Subsat;     B 172 173*/
  i = 150;
  while (i<=173)
  {
    l=i+1;
    dp[i] ^= dp[l];
    dp[l] ^= dp[i];
    dp[i] ^= dp[l];
    i+=2;
  }

/* SelFloat Subsat_Lat; B 174 177*/
/* SelFloat Subsat_Lon; B 178 181*/
  i=174;
  while (i<=181)
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

/* uint16 G1cnt; B 184 185*/
/* uint16 G2cnt; B 186 187*/
/* uint16 EW_Pix_Bias; B 188 189*/
/* uint16 NS_Line_Bias; B 190 191*/
  i = 184;
  while (i<=191)
  {
    l=i+1;
    dp[i] ^= dp[l];
    dp[l] ^= dp[i];
    dp[i] ^= dp[l];
    i+=2;
  }

/* SelFloat Idber; B 194 197*/
/* SelFloat Range; B 198 201*/
/* SelFloat Gpath; B 202 205*/
/* SelFloat Xmsne; B 206 209*/
  i=194;
  while (i<=209)
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

/* uint16 Istim; B 226 227*/
  i = 226; l=i+1;
  dp[i] ^= dp[l];
  dp[l] ^= dp[i];
  dp[i] ^= dp[l];

/* SelFloat NW_lat_of_frame; B 230 233*/
/* SelFloat NW_lon_of_frame; B 234 237*/
/* SelFloat SE_lat_of_frame; B 238 241*/
/* SelFloat SE_lon_of_frame; B 242 245*/
  i=230;
  while (i<=245)
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

/* SelFloat ReferenceLongitude;        B 294 297*/
/* SelFloat ReferenceRadialDistance;   B 298 301*/
/* SelFloat ReferenceLatitude;         B 302 305*/
/* SelFloat ReferenceOrbitYaw;         B 306 309*/
/* SelFloat ReferenceAttitudeRoll;     B 310 313*/
/* SelFloat ReferenceAttitudePitch;    B 314 317*/
/* SelFloat ReferenceAttitudeYaw;      B 318 321*/
  i=294;
  while (i<=321)
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

/* SelFloat IMCenableFromEpoch;        B 330 333*/
/* SelFloat CompensationRoll;          B 334 337*/
/* SelFloat CompensationPitch;         B 338 341*/
/* SelFloat CompensationYaw;           B 342 345*/
/* SelFloat ChangeLongitude[13];       B 346 397*/
/* SelFloat ChangeRadialDistance[11];  B 398 441*/
/* SelFloat SineGeocentricLatitude[9]; B 442 477*/
/* SelFloat SineOrbitYaw[9];           B 478 513*/
/* SelFloat DailySolarRate;            B 514 517*/
/* SelFloat ExponentialStartFromEpoch; B 518 521*/
  i=330;
  while (i<=521)
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

/* AttitudeAngle RollAngle;         B 522 741*/
  RollAngle.ByteSwap();
/* AttitudeAngle PitchAngle;        B 742 961*/
  PitchAngle.ByteSwap();
/* AttitudeAngle YawAngle;          B 962 1181*/
  YawAngle.ByteSwap();
/* AttitudeAngle RollMisalignment;  B 1182 1401*/
  RollMisalignment.ByteSwap();
/* AttitudeAngle PitchMisalignment; B 1402 1661*/
  PitchMisalignment.ByteSwap();

/* uint16 w1691_782[46];  B 1690 1781*/
/* uint16 w1783_874[46];  B 1782 1873*/
/* uint16 w1875_966[46];  B 1874 1965*/
/* uint16 w1967_2058[46]; B 1966 2057*/
  i = 1690;
  while (i<=2057)
  {
    l=i+1;
    dp[i] ^= dp[l];
    dp[l] ^= dp[i];
    dp[i] ^= dp[l];
    i+=2;
  }

/* uint16 Block_telemetry_word[2][39]; B 2146 2301*/
  i = 2146;
  while (i<=2301)
  {
    l=i+1;
    dp[i] ^= dp[l];
    dp[l] ^= dp[i];
    dp[i] ^= dp[l];
    i+=2;
  }

/* uint16 Grid1Pixel[512];  B 3330 4353*/
/* uint16 Grid2Pixel[512];  B 4354 5377*/
/* uint16 Gridset1RevLevel; B 5378 5379*/
/* uint16 Gridset2RevLevel; B 5380 5381*/
  i = 3330;
  while (i<=5381)
  {
    l=i+1;
    dp[i] ^= dp[l];
    dp[l] ^= dp[i];
    dp[i] ^= dp[l];
    i+=2;
  }

/* uint16 w5387_478[46]; B 5386 5477*/
/* uint16 w5479_570[46]; B 5478 5569*/
  i = 5386;
  while (i<=5569)
  {
    l=i+1;
    dp[i] ^= dp[l];
    dp[l] ^= dp[i];
    dp[i] ^= dp[l];
    i+=2;
  }

/* SelFloat Iwbias[7]; B 5586 5613*/
/* SelFloat Igain1[7]; B 5614 5641*/
/* SelFloat Igain2[7]; B 5642 5669*/
/* SelFloat Ibrate[7]; B 5670 5697*/
  i=5586;
  while (i<=5697)
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

/* BiasStatistics ImagerIRclamped ; B 5706 5969*/
  ImagerIRclamped.ByteSwap();
/* BiasStatistics ImagerIRdrift ;   B 5970 6233*/
  ImagerIRdrift.ByteSwap();

/* uint16 ns_incr;                    B 6306 6307*/
/* uint16 ew_incr;                    B 6308 6309*/
/* uint16 Vis_detector_x_offset[8]; B 6310 6325*/
/* uint16 Ir_detector_x_offset[14]; B 6326 6353*/
/* uint16 Vis_detector_y_offset[8]; B 6354 6369*/
/* uint16 Ir_detector_y_offset[14]; B 6370 6397*/
  i = 6306;
  while (i<=6397)
  {
    l=i+1;
    dp[i] ^= dp[l];
    dp[l] ^= dp[i];
    dp[i] ^= dp[l];
    i+=2;
  }

/* SelFloat Ivcrb[8];       B 6398 6429*/
/* SelFloat Ivcr1[8];       B 6430 6461*/
/* SelFloat Ivcr2[8];       B 6462 6493*/
/* SelFloat Ivral;          B 6494 6497*/
/* SelFloat Iicrb[2][7];    B 6498 6553*/
/* SelFloat Iicr1[2][7];    B 6554 6609*/
/* SelFloat Iicr2[2][7];    B 6610 6665*/
/* SelFloat Iisfb[2][7];    B 6666 6721*/
/* SelFloat Iisf1[2][7];    B 6722 6777*/
/* SelFloat Ig2it[2][7][4]; B 6778 7001*/
/* SelFloat Ig2bp[4];       B 7002 7017*/
/* SelFloat Ibbtr[2][7][4]; B 7018 7241*/
  i=6398;
  while (i<=7241)
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

/* Calibration Imager; B 7366 8013*/
  Imager.ByteSwap();

/* SelFloat Patch_control_voltage_gain; B 8014 8017*/
/* SelFloat Patch_control_voltage_bias; B 8018 8021*/
/* SelFloat Instrument_current_gain ;   B 8022 8025*/
/* SelFloat Instrument_current_bias;    B 8026 8029*/
  i=8014;
  while (i<=8029)
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

/* uint16 CRC; B 8040 8041 */
  i = 8040; l=i+1;
  dp[i] ^= dp[l];
  dp[l] ^= dp[i];
  dp[i] ^= dp[l];

} /* end ImagerDoc::ByteSwap */



void ImagerDoc::Ddump_all(ostream & out ){

  int i=0;

   out << " \n";									     					   
   out << " \n";									     					   
   out << "  //   0- 277 Instrument and Scan Status \n";
   out << " \n";									     					   
   out << "  uchar8 Spcid  /* 1 */ 			        "  <<  (int)Spcid                                          << "\n";
   out << "  uchar8 ground_system_id  /* 2 */ 			"  <<  (int)ground_system_id                               << "\n";
   out << "  ISCAN Iscan  /* 3 6 */ 			        \n"	 ;  Iscan.print(out);                                       out   << "\n";
   out << "  uchar8 Idsub   /* 7 */ 			        "  <<  (int)Idsub                                          << "\n";
   out << "  uchar8 IdsubIR[7]   /* 8 14 */ 			"  <<  (int)IdsubIR[0]<<"\t"<<(int)IdsubIR[1]<<"\t"<<(int)IdsubIR[2]<<"\t"<<(int)IdsubIR[3]<<"\t"<<(int)IdsubIR[4]<<"\t"<<(int)IdsubIR[5]<<"\t"<<(int)IdsubIR[6] << "\n";
   out << "  uchar8 IdsubVis[8]  /* 15 22 */ 			"  <<  (int)IdsubVis[0]<<"\t"<<(int)IdsubVis[1]<<"\t"<<(int)IdsubVis[2]<<"\t"<<(int)IdsubVis[3]<<"\t"<<(int)IdsubVis[4]<<"\t"<<(int)IdsubVis[5]<<"\t"<<(int)IdsubVis[6]<<"\t"<<(int)IdsubVis[7] << "\n";
   out << " \n";						      					   
   out << "  CdaTime T_sps_current    /* 23 30 */ 		"  <<   T_sps_current                                  << "\n";
   out << "  CdaTime T_current_header    /* 31 38 */ 		"  <<   T_current_header                               << "\n";
   out << "  CdaTime T_current_trailer    /* 39 46 */ 		"  <<   T_current_trailer                              << "\n";
   out << "  CdaTime T_lagged_header    /* 47 54 */ 		"  <<   T_lagged_header                                << "\n";
   out << "  CdaTime T_lagged_trailer    /* 55 62 */ 		"  <<   T_lagged_trailer                               << "\n";
   out << "  CdaTime T_priority_framestart    /* 63 70 */ 	"  <<   T_priority_framestart                          << "\n";
   out << "  CdaTime T_normal_framestart    /* 71 78 */ 	"  <<   T_normal_framestart                            << "\n";
   out << "  CdaTime Tispc    /* 79 86 */ 			"  <<   Tispc                                          << "\n";
   out << "  CdaTime Tiecl    /* 87 94 */ 			"  <<   Tiecl                                          << "\n";
   out << "  CdaTime Tibbc    /* 95 102*/ 			"  <<   Tibbc                                          << "\n";
   out << "  CdaTime Tistr    /*103 110*/ 			"  <<   Tistr                                          << "\n";
   out << "  CdaTime Tlran    /*111 118*/ 			"  <<   Tlran                                          << "\n";
   out << "  CdaTime Tiirt    /*119 126*/ 			"  <<   Tiirt                                          << "\n";
   out << "  CdaTime Tivit    /*127 134*/ 			"  <<   Tivit                                          << "\n";
   out << "  CdaTime Tclmt    /*135 142*/ 			"  <<   Tclmt                                          << "\n";
   out << "  CdaTime Tiona    /*143 150*/ 			"  <<   Tiona                                          << "\n";
   out << " \n";									     					   
   out << "  uint16 rel_scan_count   /*151 152*/ 		"	<<   rel_scan_count                            << "\n";
   out << "  uint16 abs_scan_count   /*153 154*/ 		"	<<   abs_scan_count                            << "\n";
   out << "  uint16 N_Line_In_Scan   /*155 156*/ 		"	<<   N_Line_In_Scan                            << "\n";
   out << "  uint16 W_pix_of_frame   /*157 158*/ 		"	<<   W_pix_of_frame                            << "\n";
   out << "  uint16 E_pix_of_frame   /*159 160*/ 		"	<<   E_pix_of_frame                            << "\n";
   out << "  uint16 N_line_of_frame   /*161 162*/ 		"	<<   N_line_of_frame                           << "\n";
   out << "  uint16 S_line_of_frame   /*163 164*/ 		"	<<   S_line_of_frame                           << "\n";
   out << "  uint16 pix_of_zero_az   /*165 166*/ 		"	<<   pix_of_zero_az                            << "\n";
   out << "  uint16 line_of_zero_elev   /*167 168*/ 		"	<<   line_of_zero_elev                         << "\n";
   out << "  uint16 scan_of_zero_elev   /*169 170*/ 		"	<<   scan_of_zero_elev                         << "\n";
   out << "  uint16 Line_Of_Subsat      /*171 172*/ 		"	<<   Line_Of_Subsat                            << "\n";
   out << "  uint16 Pix_Of_Subsat       /*173 174*/ 		"	<<   Pix_Of_Subsat                             << "\n";
   out << "  SelFloat  Subsat_Lat   /*175 178*/ 		"	<<   Subsat_Lat                                << "\n";
   out << "  SelFloat  Subsat_Lon   /*179 182*/ 		"	<<   Subsat_Lon                                << "\n";
   out << " \n";									     			       	   
   out << "  uchar8 Czone   /*183*/ 			        "	<<   (int)Czone                                << "\n";
   out << "  uchar8 V_Detect_In_Block3   /*184*/ 		"	<<   (int)V_Detect_In_Block3                   << "\n";
   out << "  uint16 G1cnt   /*185 186*/ 			"	<<   G1cnt                                     << "\n";
   out << "  uint16 G2cnt   /*187 188*/ 			"	<<   G2cnt                                     << "\n";
   out << "  uint16 EQ_Pix_Bias   /*189 190*/ 			"	<<   EQ_Pix_Bias                               << "\n";
   out << "  uint16 NS_Line_Bias   /*191 192*/ 			"	<<   NS_Line_Bias                              << "\n";
   out << "  uchar8 iscan_parity   /*193 */ 			"	<<   (int)iscan_parity                         << "\n";
   out << "  uchar8 w194_spare   /* 194*/ 			"	<<   (int)w194_spare                           << "\n";
   out << "  SelFloat Idber   /*195 198*/ 			"	<<   Idber                                     << "\n";
   out << "  SelFloat Range   /*199 202*/ 			"	<<   Range                                     << "\n";
   out << "  SelFloat Gpath   /*203 206*/ 			"	<<   Gpath                                     << "\n";
   out << "  SelFloat Xmsne   /*207 210*/ 			"	<<   Xmsne                                     << "\n";
   out << "  CdaTime Tgpat   /*211 218*/ 			"	<<   Tgpat                                     << "\n";
   out << "  CdaTime Txmsn   /*219 226*/ 			"	<<   Txmsn                                     << "\n";
   out << "  uint16 Istim   /*227 228*/ 			"	<<   Istim                                     << "\n";
   out << "  uchar8 frame_number   /*229*/ 			"	<<   (int)frame_number                         << "\n";
   out << "  uchar8 Imode   /*230*/ 			        "	<<   (int)Imode                                << "\n";
   out << " \n";								     				       
   out << "  SelFloat NW_lat_of_frame   /*231 234*/ 		"	<<   NW_lat_of_frame                           << "\n";
   out << "  SelFloat NW_lon_of_frame   /*235 238*/ 		"	<<   NW_lon_of_frame                           << "\n";
   out << "  SelFloat SE_lat_of_frame   /*239 242*/ 		"	<<   SE_lat_of_frame                           << "\n";
   out << "  SelFloat SE_lon_of_frame   /*243 246*/ 		"	<<   SE_lon_of_frame                           << "\n";
   out << " \n";									     			       	   


   out << "  uchar8 gain_table_index      /* 247 IG2TN  */	"	<<   gain_table_index                          << "\n";
   out << "  uchar8 iscan_parity_copy2    /* 248 ISCP2  */	"	<<   iscan_parity_copy2                        << "\n";
   out << "  uchar8 iscan_3_4[2]          /* 249-250    */	"	<<   (int)iscan_3_4[0]                         << "\n";
   out << "  CdaTime T_current_framestart /* 251-258 version 1+ */ "	<<   T_current_framestart                      << "\n";


   out << "  uchar8 w259_277[31]   /*259 277*/ 			"	<<   (int)w259_277[0]                          << "\n";
   out << "  uchar8 Parity_1_277   /*278*/ 			"	<<   (int)Parity_1_277                         << "\n";
   out << " \n";									     					   
   out << " \n";									     					   
   out << " \n";									     					   






   out << "   * Imager Documentation, Block 0 Format Definition \n";
   out << "   * Orbit and Attitude Parameters \n";
   out << " \n";									     					   
   out << "  uchar8 Imc_identifier[4]   /* 279 282*/ 			"	<<   (int)Imc_identifier[0]                                 << "\n";
   out << "  uchar8 w283_94[12]   /* 283 294*/ 			        "	<<   (int)w283_94[0]                                        << "\n";
   out << " \n";									     					   
   out << "  SelFloat ReferenceLongitude   /* 295 298*/ 		"	<<   ReferenceLongitude                             << "\n";
   out << "  SelFloat ReferenceRadialDistance   /* 299 302*/ 		"	<<   ReferenceRadialDistance                        << "\n";
   out << "  SelFloat ReferenceLatitude   /* 303 306*/ 			"	<<   ReferenceLatitude                              << "\n";
   out << "  SelFloat ReferenceOrbitYaw   /* 307 310*/ 			"	<<   ReferenceOrbitYaw                              << "\n";
   out << "  SelFloat ReferenceAttitudeRoll   /* 311 314*/ 		"	<<   ReferenceAttitudeRoll                          << "\n";
   out << "  SelFloat ReferenceAttitudePitch   /* 315 318*/ 		"	<<   ReferenceAttitudePitch                         << "\n";
   out << "  SelFloat ReferenceAttitudeYaw   /* 319 322*/ 		"	<<   ReferenceAttitudeYaw                           << "\n";
   out << " \n";									     					   
   out << "  CdaTime EpochDate   /* 323 330*/ 			        "	<<   EpochDate                                      << "\n";
   out << " \n";									     					   
   out << "  SelFloat IMCenableFromEpoch   /* 331 334*/ 		"	<<   IMCenableFromEpoch                             << "\n";
   out << "  SelFloat CompensationRoll   /* 335 338*/ 			"	<<   CompensationRoll                               << "\n";
   out << "  SelFloat CompensationPitch   /* 339 342*/ 			"	<<   CompensationPitch                              << "\n";
   out << "  SelFloat CompensationYaw   /* 343 346*/ 			"	<<   CompensationYaw                                << "\n";
   out << "  SelFloat ChangeLongitude[13]   /* 347 398*/ 		"	<<   ChangeLongitude[0]                                << "\n";
   out << "  SelFloat ChangeRadialDistance[11]   /* 399 442*/ 		"	<<   ChangeRadialDistance[0]                           << "\n";
   out << "  SelFloat SineGeocentricLatitude[9]   /* 443 478*/ 		"       <<   SineGeocentricLatitude[0]                         << "\n";
   out << "  SelFloat SineOrbitYaw[9]   /* 479 514*/ 			"	<<   SineOrbitYaw[0]                                   << "\n";
   out << "  SelFloat DailySolarRate   /* 515 518*/ 			"	<<   DailySolarRate                                 << "\n";
   out << "  SelFloat ExponentialStartFromEpoch   /* 519 522*/ 		"       <<   ExponentialStartFromEpoch                      << "\n";
   out << " \n";									     					   
   out << "  AttitudeAngle RollAngle   /* 523 742*/ 			"	<<   RollAngle                                      << "\n";
   out << "  AttitudeAngle PitchAngle   /* 743 962*/ 			"	<<   PitchAngle                                     << "\n";
   out << "  AttitudeAngle YawAngle   /* 963 1182*/ 			"	<<   YawAngle                                       << "\n";
   out << "  AttitudeAngle RollMisalignment   /*1183 1402*/ 		"	<<   RollMisalignment                               << "\n";
   out << "  AttitudeAngle PitchMisalignment   /*1403 1662*/ 		"	<<   PitchMisalignment                              << "\n";
   out << " \n";									     					   
   out << "  uchar8 w1623_625[3]   /*1623 1625*/ 			"	<<   (int)w1623_625[0]                                      << "\n";
   out << "  uchar8 Parity_279_1625   /*1626 */ 			"	<<   (int)Parity_279_1625                                << "\n";
   out << " \n";									     					   
   out << " \n";									     					   
   out << " \n";									     					   
   out << " \n";									     					   
   out << " \n";







   out << "  //1627-2306 Scan Reference Data \n";
   out << " \n";									     					   
   out << "  uchar8 Coregisteration_table_id[4]   /*1627 1630*/ 		 "  <<   (int)Coregisteration_table_id[0]                       << "\n";
   out << "  uchar8 East_west_half_hourly_correction_terms[48]   /*1631 1678*/   "  <<   (int)East_west_half_hourly_correction_terms[0]         << "\n";
   out << "  uchar8 Index_of_correction_term_currently_active   /*1679 */        "  <<   (int)Index_of_correction_term_currently_active        << "\n";
   out << "  uchar8 w1680_1690[11]   /*1680 1690*/ 			         "  <<   (int)w1680_1690[0]                                    << "\n";
   out << " \n";									     					   
   out << "  uint16 w1691_782[46]   		 "					<<   w1691_782[0]                                      << "\n";
   out << "  uint16 w1783_874[46]   		 "					<<   w1783_874[0]                                      << "\n";
   out << "  uint16 w1875_966[46]   		 "					<<   w1875_966[0]                                      << "\n";
   out << "  uint16 w1967_2058[46]  		 "					<<   w1967_2058[0]                                     << "\n";
   out << " \n";		    							     					   
   out << "  uchar8 w2059_80[22]    		 "					<<   (int)w2059_80[0]                                       << "\n";
   out << "  uchar8 w2081_102[22]   		 "					<<   (int)w2081_102[0]                                      << "\n";
   out << "  uchar8 w2103_24[22]    		 "					<<   (int)w2103_24[0]                                       << "\n";
   out << "  uchar8 w2125_46[22]    		 "					<<   (int)w2125_46[0]                                       << "\n";
   out << "  uint16 Block_telemetry_word[2][39]    /* 2147 2302 */ 	"               <<   Block_telemetry_word[0][0]                           << "\n";
   out << "  uchar8 w2303_5[3]   /* 2305 2305 */ 			"		<<   (int)w2303_5[0]                                        << "\n";
   out << "  uchar8 Parity_1691_2305   /* 2306 */ 			"		<<   (int)Parity_1691_2305                               << "\n";
   out << " \n";									     					   
   out << " \n";									     					   
   out << " \n";									     					   
   out << " \n";									     					   





   out << "  //2307-5386 Grid Data \n";						     					   
   out << " \n";									     					   
   out << "  uchar8 Grid1Detector[512]   /*2307 2818*/ 			"	<<   (int)Grid1Detector[0]                                  << "\n";
   out << "  uchar8 Grid2Detector[512]   /*2819 3330*/ 			"	<<   (int)Grid2Detector[0]                                  << "\n";
   out << "  uint16 Grid1Pixel[512]   /*3331 4353*/ 			"	<<   Grid1Pixel[0]                                     << "\n";
   out << "  uint16 Grid2Pixel[512]   /*4355 5378*/ 			"	<<   Grid2Pixel[0]                                     << "\n";
   out << " \n";								       					   
   out << "  uint16 Gridset1RevLevel   /*5379 5380*/ 			"	<<   Gridset1RevLevel                               << "\n";
   out << "  uint16 Gridset2RevLevel   /*5381 5382*/ 			"	<<   Gridset2RevLevel                               << "\n";
   out << " \n";								     					   
   out << "  uchar8 w5383_85[3]   /*5383 5385*/ 			"	<<   (int)w5383_85[0]                                       << "\n";
   out << "  uchar8 Parity_2307_5385   /*5386 */ 			"	<<   (int)Parity_2307_5385                               << "\n";
   out << " \n";								       					   
   out << "  uint16 w5387_478[46]                                       "	<<   w5387_478[0]                                      << "\n";
   out << "  uint16 w5479_570[46]                                       "	<<   w5479_570[0]                                      << "\n";
   out << " \n";									     					   
   out << "  CdaTime Tophed   /* 5571 5578 */ 		             	"	<<   Tophed                                         << "\n";
   out << "  CdaTime Totrl /* 2279 5586 */                              "	<<   Totrl                                          << "\n";
   out << " \n";									     					   
   out << " \n";									     					   
   out << " \n";									     					   
   out << " \n";									     					   





   out << "  //5387-6304 Scan Reference and Calibration Data \n";
   out << " \n";									     					   
   out << "  SelFloat  Iwbias[7]   /* 5587 5614*/ 			" <<Iwbias[0]<<"\t"<<Iwbias[1]<<"\t"<<Iwbias[2]<<"\t"<<Iwbias[3]<<"\t"<<Iwbias[4]<<"\t"<<Iwbias[5]<<"\t"<<Iwbias[6]  << "\n";
   out << "  SelFloat  Igain1[7]   /* 5615 5642*/ 			" <<Igain1[0]<<"\t"<<Igain1[1]<<"\t"<<Igain1[2]<<"\t"<<Igain1[3]<<"\t"<<Igain1[4]<<"\t"<<Igain1[5]<<"\t"<<Igain1[6]  << "\n";
   out << "  SelFloat  Igain2[7]   /* 5643 5670*/ 			" <<Igain2[0]<<"\t"<<Igain2[1]<<"\t"<<Igain2[2]<<"\t"<<Igain2[3]<<"\t"<<Igain2[4]<<"\t"<<Igain2[5]<<"\t"<<Igain2[6]  << "\n";
   out << "  SelFloat  Ibrate[7]   /* 5671 5698*/ 			" <<Ibrate[0]<<"\t"<<Ibrate[1]<<"\t"<<Ibrate[2]<<"\t"<<Ibrate[3]<<"\t"<<Ibrate[4]<<"\t"<<Ibrate[5]<<"\t"<<Ibrate[6]  << "\n";
   out << " \n";									       					   
   out << "  CdaTime Tod_of_westernmost_ir_pixel   /* 5699 5706 */ 	"  <<   Tod_of_westernmost_ir_pixel                    << "\n";
   out << " \n";									       					   
   //   out << "  BiasStatistics ImagerIRclamped    /* 5707 5970 */  "	<<   ImagerIRclamped                                << "\n";
   //   out << "  BiasStatistics ImagerIRdrift    /* 5971 6234 */    "	<<   ImagerIRdrift                                  << "\n";
   out << " \n";									     					   
   out << "  uchar8 NS_half_hourly_correction_terms[48]    /* 6235 6282 */ "		<<   (int)NS_half_hourly_correction_terms[0]                << "\n";
   out << "  uchar8 Scan_clamp_e_w_clipping_edge_limb_offset[4]   /* 6283 6286 */ "	<<   (int)Scan_clamp_e_w_clipping_edge_limb_offset[0]       << "\n";
   out << "  uchar8 w6287_303[17]    /* 6287 6203 */ 			"		<<   (int)w6287_303[0]                                      << "\n";
   out << "  uchar8 Parity_5387_6303    /* 6304 */ 			"		<<   (int)Parity_5387_6303                               << "\n";
   out << " \n";									       					   
   out << " \n";									     					   
   out << " \n";									     					   





   out << "  //6305-8040 Factory Parameters/8th IR detector Calibration Data \n";
   out << " \n";									     					   
   out << "  uchar8 ns_cycles   /* 6305 */ 			"			<<   (int)ns_cycles                                   << "\n";
   out << "  uchar8 ew_cycles    /* 6306 */ 			"			<<   (int)ew_cycles                                   << "\n";
   out << "  uint16 ns_incr    /* 6307 6308 */ 			"			<<   ns_incr                                          << "\n";
   out << "  uint16 ew_incr   /* 6309 6310 */ 			"			<<   ew_incr                                          << "\n";
   out << " \n";									       					   
   out << "  uint16 Vis_detector_x_offset[8]   /* 6311 6325 */ 			";  for (i=0;i<8;i++) out<<Vis_detector_x_offset[i]<<"\t"; out  << "\n";
   out << "  uint16 Ir_detector_x_offset[14]   /* 6326 6354 */ 			";  for (i=0;i<14;i++) out<<Ir_detector_x_offset[i]<<"\t";  out  << "\n";
   out << "  uint16 Vis_detector_y_offset[8]   /* 6355 6369 */ 			";  for (i=0;i<8;i++) out<<Vis_detector_y_offset[i]<<"\t"; out  << "\n";
   out << "  uint16 Ir_detector_y_offset[14]   /* 6370 6398 */ 			";  for (i=0;i<14;i++) out<<Ir_detector_y_offset[i]<<"\t";  out  << "\n";
   out << " \n";									     					   
   out << "  SelFloat Ivcrb[8]   /* 6399 6430 */                   "      ;  for (i=0;i<8;i++) out<<Ivcrb[i]<<"\t"; out << "\n";
   out << "  SelFloat Ivcr1[8]   /* 6431 6462 */                   "      ;  for (i=0;i<8;i++) out<<Ivcr1[i]<<"\t"; out << "\n";
   out << "  SelFloat Ivcr2[8]   /* 6463 6494 */                   "      ;  for (i=0;i<8;i++) out<<Ivcr2[i]<<"\t"; out << "\n";
   out << "  SelFloat Ivral      /* 6495 6498 */                   "            <<   Ivral                                          << "\n";
   out << " \n";                                                    
   out << " \n";                                                                                              
   out << "  SelFloat Iicrb[2][7]    /* 6499 6554 */               "      ;  for (i=0;i<7;i++) out<<Iicrb[0][i]<<"\t"; out   << "\n";
   out << "                                                        "      ;  for (i=0;i<7;i++) out<<Iicrb[1][i]<<"\t"; out   << "\n";

   out << "  SelFloat Iicr1[2][7]    /* 6555 6610 */                "      ;  for (i=0;i<7;i++) out<<Iicr1[0][i]<<"\t"; out   << "\n";
   out << "                                                         "      ;  for (i=0;i<7;i++) out<<Iicr1[1][i]<<"\t"; out   << "\n";

   out << "  SelFloat Iicr2[2][7]    /* 6611 6666 */                "      ;  for (i=0;i<7;i++) out<<Iicr2[0][i]<<"\t"; out   << "\n";
   out << "                                                         "      ;  for (i=0;i<7;i++) out<<Iicr2[1][i]<<"\t"; out   << "\n";



   out << " \n";                                                                                              
   out << "  SelFloat Iisfb[2][7]    /* 6667 6722 */                "      ;  for (i=0;i<7;i++) out<<Iisfb[0][i]<<"\t"; out   << "\n";
   out << "                                                         "      ;  for (i=0;i<7;i++) out<<Iisfb[1][i]<<"\t"; out   << "\n";

   out << "  SelFloat Iisf1[2][7]    /* 6723 6778 */                "      ;  for (i=0;i<7;i++) out<<Iisf1[0][i]<<"\t"; out   << "\n";
   out << "                                                         "      ;  for (i=0;i<7;i++) out<<Iisf1[1][i]<<"\t"; out   << "\n";
   out << " \n";                                                                                              
   out << "  SelFloat Ig2it[2][7][4]    /* 6779 7002 */             "      ;  for (i=0;i<7;i++) out<<Ig2it[0][i][0]<<"\t"; out   << "\n";
   out << "                                                         "      ;  for (i=0;i<7;i++) out<<Ig2it[0][i][1]<<"\t"; out   << "\n";
   out << "                                                         "      ;  for (i=0;i<7;i++) out<<Ig2it[0][i][2]<<"\t"; out   << "\n";
   out << "                                                         "      ;  for (i=0;i<7;i++) out<<Ig2it[0][i][3]<<"\t"; out   << "\n";

   out << " \n";                                                    
   out << " \n";                                                    


                                          
   out << "  SelFloat Ig2bp[4]    /* 7003 7018 */                   "      ;  for (i=0;i<4;i++) out<<Ig2bp[i]<<"\t"; out   << "\n";
   out << "  SelFloat Ibbtr[2][7][4]    /* 7019 7242 */                   "      <<   Ibbtr                                          << "\n";
   out << " \n";                                                                                              
   out << "  uchar8 w7234_366[124]   /* 7243 7336 */                   "            <<   (int)w7234_366[0]                                      << "\n";
   out << " \n";                                                                                            
   out << "  Calibration Imager   /* 7367 8014 */                   "            ;   Imager.print( out);      //                                   << "\n";
   out << " \n";                                                                                              
   out << "  SelFloat Patch_control_voltage_gain   /* 8015 8018 */      "                   <<   Patch_control_voltage_gain                     << "\n";
   out << "  SelFloat Patch_control_voltage_bias   /* 8019 8022 */       "  <<   Patch_control_voltage_bias                     << "\n";
   out << "  SelFloat Instrument_current_gain      /* 8023 8026 */       "  <<   Instrument_current_gain                        << "\n";
   out << "  SelFloat Instrument_current_bias      /* 8027 8030 */       "  <<   Instrument_current_bias                        << "\n";
   out << " \n";                                                                                              
   out << "  uchar8 w8031_39[9]    /* 8031 8039 */                   "            <<   (int)w8031_39[0]                                       << "\n";
   out << "  uchar8 Parity_6305_8039   /* 8040 */                   "            <<   (int)Parity_6305_8039                               << "\n";
   out << "  uint16 CRC   /* 8041 8042 */                           "                  <<   CRC                                            << "\n";
   out << " \n";
   out << " \n";
   out << " \n";


}


