#include "gimloc3.h"
#include "math.h"

extern ImagerDoc imagerDoc ; 
extern SounderDoc sounderDoc ; 

extern nav_constants i_nav;
extern nav_constants s_nav;

void timex(int ny, 
           int nd, 
           int nh, 
           int nm, 
  	   double  s, 
  	   double & out_time) {

  int j;

   /*     here we convert integer year and day of year to number of */
   /*     days from 0 hour ut, 1950 jan. 1.0 */
   /*     this convertion is based on an algorithm by fliegel and van */
   /*     flandern, comm. of acm, vol.11, no. 10, oct. 1968 (p.657) */
   j = nd + (ny + 4799) * 1461 / 4 - (ny + 4899) / 100 * 3 / 4 - 2465022;


   /*  convert days to minutes and add hour, minute and seconds */
   out_time = j * 1440. + nh * 60. + nm + s / 60.;

}

void gimlocInit(){

  static int rel_scan_count = -1;
  static int frame_number = -1; 
  static double REC[336];

  //	std::cout <<"\nin gimloc init\n";

  if(rel_scan_count != imagerDoc.rel_scan_count || frame_number != imagerDoc.frame_number) { 

    rel_scan_count = imagerDoc.rel_scan_count;
    frame_number = imagerDoc.frame_number;

    REC[0] = 0; 
    REC[1] = 0;
    REC[2] = 0;
    REC[3] = 0;
    REC[4] = (double)imagerDoc.ReferenceLongitude; /* 295 298*/
    REC[5] = (double)imagerDoc.ReferenceRadialDistance; /* 299 302*/
    REC[6] = (double)imagerDoc.ReferenceLatitude; /* 303 306*/
    REC[7] = (double)imagerDoc.ReferenceOrbitYaw; /* 307 310*/
    REC[8] = (double)imagerDoc.ReferenceAttitudeRoll; /* 311 314*/
    REC[9] = (double)imagerDoc.ReferenceAttitudePitch; /* 315 318*/
    REC[10] = (double)imagerDoc.ReferenceAttitudeYaw; /* 319 322*/

    REC[11] = (imagerDoc.EpochDate.year()* 10000 +
	       imagerDoc.EpochDate.day() * 10  +
	       (imagerDoc.EpochDate.hrs())/10 );
	    
    REC[12] = ( (imagerDoc.EpochDate.hrs()) % 10 * 10000000 +  
	       imagerDoc.EpochDate.min() * 100000 + 
	       imagerDoc.EpochDate.sec() * 10 +
	       imagerDoc.EpochDate.msec() );
  

    REC[13] = (double)imagerDoc.IMCenableFromEpoch; /* 331 334*/
    REC[14] = (double)imagerDoc.CompensationRoll; /* 335 338*/
    REC[15] = (double)imagerDoc.CompensationPitch; /* 339 342*/
    REC[16] = (double)imagerDoc.CompensationYaw; /* 343 346*/
    int i ;
    for(i=0; i<13; i++) 
      REC[i+17] = (double)imagerDoc.ChangeLongitude[i];/* 347 398*/

    for(i = 0; i<11; i++) 
      REC[i+30] = (double)imagerDoc.ChangeRadialDistance[i]; /* 399 442*/

    for(i = 0; i<9 ; i++) 
      REC[41+i] = (double)imagerDoc.SineGeocentricLatitude[i]; /* 443 478*/
    
    for(i = 0; i<9; i++) 
      REC[50+i] = (double)imagerDoc.SineOrbitYaw[i]; /* 479 514*/
  
    REC[59] = (double)imagerDoc.DailySolarRate; /* 515 518*/
    
    REC[60] = (double)imagerDoc.ExponentialStartFromEpoch; /* 519 522*/

    imagerDoc.RollAngle.toNoaaNavigation(REC+61);
    imagerDoc.PitchAngle.toNoaaNavigation(REC+116); /* 743 962*/
    imagerDoc.YawAngle.toNoaaNavigation(REC+171); /* 963 1182*/
    imagerDoc.RollMisalignment.toNoaaNavigation(REC+226); /*1183 1402*/
    imagerDoc.PitchMisalignment.toNoaaNavigation(REC+281); /*1403 1662*/
  
  
    
    double T, TU;
    int imc = 0; // imagerDoc.iscan.imcStatus() ; 
    double rlat, rlon;
    double sec;  

    sec = imagerDoc.EpochDate.sec() + imagerDoc.EpochDate.msec()/1000.0;

    timex(imagerDoc.EpochDate.year(),
          imagerDoc.EpochDate.day(),
          imagerDoc.EpochDate.hrs(),
          imagerDoc.EpochDate.min(),
          sec,
          TU  );
  
    sec = imagerDoc.T_sps_current.sec() + imagerDoc.T_sps_current.msec()/1000.0;

    timex(imagerDoc.T_sps_current.year(),
          imagerDoc.T_sps_current.day(),
          imagerDoc.T_sps_current.hrs(),
          imagerDoc.T_sps_current.min(),
          sec,
          T  );


    int INSTRUMENT = 1;
    int NS_NAD_CY =  imagerDoc.ns_cycles;
    int EW_NAD_CY =  imagerDoc.ew_cycles;

    int NS_NAD_INC = imagerDoc.ns_incr;
    int EW_NAD_INC = imagerDoc.ew_incr;

    setcon_(&INSTRUMENT,
	    &NS_NAD_CY,
	    &NS_NAD_INC,
	    &EW_NAD_CY,
	    &EW_NAD_INC);

    double pi = 4 * atan(1.0);

    /* ????? lmodel calculates rlat and rlon ????? */
    rlat = (double) imagerDoc.Subsat_Lat* pi/180.0;
    rlon = (double) imagerDoc.Subsat_Lon* pi/180.0;

    lmodel_(&T,
	    &TU,
	    &REC,
	    &imc, 
	    &rlat, 
	    &rlon);
  }
}



void lineXpixel2latXlon(double  line, 
			double  pixel,
			double & latitude, 
			double & longitude ){
  
  double pi = 4 * atan(1.0);

  double elevationAngle, scanAngle;
  double Lat, Lon;
  double rLine = line ;
  double rPixel = pixel ;
  latitude = 9999;
  longitude = 9999; 

  gimlocInit();

  int INSTRUMENT = 0;
  int ier;
  set_nav_constants(  INSTRUMENT  );
 
  xy_to_scanelev(INSTRUMENT, &rPixel, &rLine, &scanAngle, &elevationAngle);

 // printf("pixel %f, line %f, angles %f, %f\n", rPixel, rLine, scanAngle, elevationAngle);

  lpoint_(&elevationAngle,
	  &scanAngle,
	  &Lat, 
	  &Lon,
	  &ier);
  
  if ( !ier ) { 
    latitude  = Lat * 180.0/pi;
    longitude = Lon* 180.0/pi;
  }

}         

void latXlon2lineXpixel(double latitude, 
			double longitude,
			double & line, 
			double & pixel){

  double pi = 4 * atan(1.0);

  int ier;
  double elevation, scan;
  double rLine  = -1; 
  double rPixel = -1;
  double rLat = latitude * pi/180.0;
  double rLon = longitude * pi/180.0;

  line = -1; 
  pixel = -1; 

  if ( latitude == 999999 || longitude == 999999 ) return; 

  gimlocInit();



  int INSTRUMENT = 0;
  set_nav_constants(  INSTRUMENT  );
  print_nav_constants(  &i_nav  );

  
  // convert lon,lat to instrument scan angle,elevation
  gpoint_(&rLat, &rLon, &elevation, &scan, &ier);
  

  // if it doesn't point off earth
  // convert scan angle,elevation to x,y
  if(!ier) { 
    scanelev_to_xy(INSTRUMENT, &scan, &elevation, &rPixel, &rLine);
    line = rLine; 
    pixel = rPixel;
  }

}







void set_nav_constants( int instr )
{


 //  src/include/types.h:  imager=0,  sounder=1;
   nav_constants *g;

   double ns_nad_cy;
   double ew_nad_cy;
   double ns_nad_inc;
   double ew_nad_inc;

   double incr_per_cycle, elevation_per_incr, scan_per_incr;

   double deg_to_rad = 3.141592653589793/180;

   // set up a bunch of constants for the following transforms,
   //  which are all purely scaling transformations...
   //
   //  1 - file x,y to GOES pixel,line
   //  2 - GOES pixel,line to GOES ew,ns cycle/increment 
   //  3 - GOES ew,ns cycle/increment to GOES ew,ns angular deflection from nadir
   // actually below we only set up 2&3, 1 is set up on a per output file basis
   //
   // Once we have GOES ew,ns angular deflection from nadir, 
   // navigation is a straightforward geometry calculation using
   // sat position and attitude and an accurate Earth model to find
   // intercept of poitning vector and Earth's surface

   if (instr == imager) {

     g = &i_nav;

     g->imc = imagerDoc.imc();

     incr_per_cycle = 6136;

     // 2.8125 = 360/128 or 128 cycles for one full 360°  shaft revolution
     // 5.6250 ... same thing except 64 cycles per revolution
     elevation_per_incr =  (2.8125/incr_per_cycle) * deg_to_rad;
     scan_per_incr      =  (5.6250/incr_per_cycle) * deg_to_rad;

     g->elevation_per_y    =  elevation_per_incr * 28 / 8;
     g->scan_per_x         =  scan_per_incr;

     // factory constants for instrument nadir .... these values show up in
     // in ???? Earth location users guide, are they also in the gvar file itself? 
     ns_nad_cy = 4;
     ew_nad_cy = 2;
     ns_nad_inc = 3068;
     ew_nad_inc = 3068;

     //     g->elevation_ul = (ns_nad_cy*incr_per_cycle + ns_nad_inc) * elevation_per_incr;
     //     g->scan_ul      = (ew_nad_cy*incr_per_cycle + ew_nad_inc) * scan_per_incr;

     g->elevation_ul =  imagerDoc.line_of_zero_elev * g->elevation_per_y;
     g->scan_ul      = -imagerDoc.pix_of_zero_az    * g->scan_per_x;

     g->line_constant = 4.5;

     g->subsat_lat = imagerDoc.Subsat_Lat;
     g->subsat_lon = imagerDoc.Subsat_Lon;
   }
   else 
   {
     g = &s_nav;

     g->imc = sounderDoc.imc();

     incr_per_cycle = 2805; 

     elevation_per_incr =  (2.8125/incr_per_cycle) * deg_to_rad;
     scan_per_incr      =  (5.6250/incr_per_cycle) * deg_to_rad;

     g->elevation_per_y  = elevation_per_incr * 64 / 4;
     g->scan_per_x       = scan_per_incr      * 8;

     // if cycles look valid, use increments from gvar data stream
     // otherwise use factory constants for instrument nadir ....
     ns_nad_cy = 4;
     ew_nad_cy = 2;
     if ( (sounderDoc.Sofnc == 4) && (sounderDoc.Sofec == 2) ) {
        ns_nad_inc = sounderDoc.Sofni;
        ew_nad_inc = sounderDoc.Sofei;
     } else {
        ns_nad_inc = 1402;
        ew_nad_inc = 1402;
     }

     g->elevation_ul = ((9-ns_nad_cy)*incr_per_cycle - ns_nad_inc) * elevation_per_incr;
     g->scan_ul      = (   ew_nad_cy *incr_per_cycle + ew_nad_inc) * scan_per_incr;

     g->line_constant = 2.5;

     g->subsat_lat = sounderDoc.Subsat_Lat;
     g->subsat_lon = sounderDoc.Subsat_Lon;

   }


   // these should be adjusted on a per file basis
   g->x_scale = 1;
   g->x_start = 0;
   g->y_scale = 1;
   g->y_start = 0;


   // start is center of pixel, adjust for pixel size
   g->x_start = g->x_start + 0.5*(1/g->x_scale);
   g->y_start = g->y_start + 0.5*(1/g->y_scale);

   g->scale[0] = g->scan_per_x      / g->x_scale;
   g->scale[1] = g->elevation_per_y / g->y_scale;

   g->tiepoints[0] = 0.5;
   g->tiepoints[1] = 0.5;
   g->tiepoints[3] =  g->scan_ul      + g->scan_per_x      * g->x_start ;
   g->tiepoints[4] =  g->elevation_ul - g->elevation_per_y * g->y_start ;

//	printf("x_start %f, x_scale %f, y_start %f, y_sale %f\n", g->x_start, g->scale[0], g->y_start, g->scale[1]);

   //------------------------------------------------------------------
   // ellipsoid constants
   //------------------------------------------------------------------
   //status = j_match_ellipsoid(name='WGS 84',ellipsoid=e)

   //g->e = e
   g->nomorb = 42164.17478    * 1000; //per NOAA ELUG rev 2, jul 2005 orig value was 42164.365d    
   g->a      = 6378137.0 / 1000.0 ;
   g->b      = 6356752.3 / 1000.0 ;
   g->flat   = 1.0  / 298.25722;
   g->e_sqr  = 1.0 - pow((1.0  - g->flat),2);


} 



void print_nav_constants( nav_constants *g )
{

  std::cout <<  "  double scale[3]        " << g->scale[0]<<"   "<< g->scale[1]<<"   "<< g->scale[2]   << "\n" << std::flush;     
   std::cout <<  "  double tiepoints[6]    " << g->tiepoints[0]<<"   "<< g->tiepoints[1]<<"   "<< g->tiepoints[2] ;
   std::cout <<                        "   " << g->tiepoints[3]<<"   "<< g->tiepoints[4]<<"   "<< g->tiepoints[5]     << "\n" << std::flush; 
   std::cout <<  "  double elevation_ul     " << g->elevation_ul       << "\n" << std::flush;          //   bounds in elevation (radians)
   std::cout <<  "  double scan_ul          " << g->scan_ul            << "\n" << std::flush;          //   bounds in scan angle (radians)
   std::cout <<  "  double elevation_per_y  " << g->elevation_per_y    << "\n" << std::flush;           //   elevation angle per detector line (radians)
   std::cout <<  "  double scan_per_x       " << g->scan_per_x         << "\n" << std::flush;           //   scan angle per pixel (radians)

   std::cout <<  "  double line_constant   " << g->line_constant     << "\n" << std::flush;
   std::cout <<  "  double x_scale         " << g->x_scale           << "\n" << std::flush;
   std::cout <<  "  double x_start         " << g->x_start           << "\n" << std::flush;
   std::cout <<  "  double y_scale         " << g->y_scale           << "\n" << std::flush;
   std::cout <<  "  double y_start         " << g-> y_start          << "\n" << std::flush;
   std::cout <<  "\n" << std::flush;

   std::cout <<  "  double nomorb          " << g->nomorb            << "\n" << std::flush;          //   nominal radial distance of satellite (km)
   std::cout <<  "  double a               " << g->a                 << "\n" << std::flush;          //   earth semi major axis (km)
   std::cout <<  "  double b               " << g->b                 << "\n" << std::flush;          //   earth semi minor axis (km)
   std::cout <<  "  double flat            " << g->flat              << "\n" << std::flush;          //   earth flattening coefficient = 1-(minor/major)
   std::cout <<  "  double e_sqr           " << g->e_sqr             << "\n" << std::flush;
   std::cout <<  "\n" << std::flush;
   std::cout <<  "  double bt[3][3]        " << g->bt[0][0]<<"   "<< g->bt[0][1]<<"   "<< g->bt[0][2]   << "\n" << std::flush;     //   ecef to instrument coordinates transformation
   std::cout <<  "  double coef_c          " << g->coef_c            << "\n" << std::flush;          //   coefficient for quad eq solved for slant distance
   std::cout <<  "  double sat_x           " << g->sat_x             << "\n" << std::flush;           //   normalized s/c position in ecef coordinates
   std::cout <<  "  double sat_y           " << g->sat_y             << "\n" << std::flush;           //   normalized s/c position in ecef coordinates
   std::cout <<  "  double sat_z           " << g->sat_z             << "\n" << std::flush;           //   normalized s/c position in ecef coordinates
   std::cout <<  "  double pitch           " << g->pitch             << "\n" << std::flush;           //   pitch angles of instrument (rad)
   std::cout <<  "  double roll            " << g->roll              << "\n" << std::flush;            //   roll angles of instrument (rad)
   std::cout <<  "  double yaw             " << g->yaw               << "\n" << std::flush;             //   yaw angles of instrument (rad)
   std::cout <<  "  double pma             " << g->pma               << "\n" << std::flush;             //   pitch misalignments of instrument (rad)
   std::cout <<  "  double rma             " << g->rma               << "\n" << std::flush;             //   pitch misalignments of instrument (rad)
   std::cout <<  "  double subsat_lat      " << g->subsat_lat        << "\n" << std::flush;
   std::cout <<  "  double subsat_lon      " << g->subsat_lon        << "\n" << std::flush;
   std::cout <<  "  int imc                " << g->imc               << "\n" << std::flush;                //   imc on or off



} 



void scanelev_to_xy(int    instr, 
                    double *scan, 
                    double *elev, 
                    double *x,
                    double *y)
{

   nav_constants *g;

   if (instr == imager) {
       g = &i_nav;
   } else {
       g = &s_nav;
   }

   *x = (*scan) /       g->scale[0]   - (g->tiepoints[3]/       g->scale[0]  - g->tiepoints[0]);
   *y = (*elev) / (-1 * g->scale[1])  - (g->tiepoints[4]/ (-1 * g->scale[1]) - g->tiepoints[1]);

} 



void xy_to_scanelev(int    instr, 
                    double *x,
                    double *y,
                    double *scan, 
                    double *elev) 
{

   nav_constants *g;

   if (instr == imager) {
       g = &i_nav;
   } else {
       g = &s_nav;
   }

   //printf("%f %f %f %f\n", g->scale[0], g->scale[1], g->tiepoints[0], g->tiepoints[1]);
   //*scan = (*x + (g->tiepoints[3]/       g->scale[0]  - g->tiepoints[0])) * (g->scale[0]);
   //*elev = (*y + (g->tiepoints[4]/ (-1 * g->scale[1]) - g->tiepoints[1])) * (-1 * g->scale[1]);
   *scan = (*x) *       g->scale[0]  - (g->tiepoints[0] *       g->scale[0]  - g->tiepoints[3]);
   *elev = (*y) * (-1 * g->scale[1]) - (g->tiepoints[1] * (-1 * g->scale[1]) - g->tiepoints[4]);

} 






