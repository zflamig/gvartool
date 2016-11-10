#include "block0/imagerDoc.h"
#include "block11/doc/sounderDoc.h"

struct nav_constants{ 
    double e;               //  replicate({j_ellipsoid},1)
    double scale[3];     
    double tiepoints[6]; 
    double pma;             //   pitch misalignments of instrument (rad)
    double rma;             //   pitch misalignments of instrument (rad)
    double a     ;          //   earth semi major axis (km)
    double b     ;          //   earth semi minor axis (km)
    double bt[3][3];        //   ecef to instrument coordinates transformation
    double coef_c;          //   coefficient for quad eq solved for slant distance
    double sat_x;           //   normalized s/c position in ecef coordinates
    double sat_y;           //   normalized s/c position in ecef coordinates
    double sat_z;           //   normalized s/c position in ecef coordinates
    double elevation_ul;        //   bounds in elevation (radians)
    double scan_ul;             //   bounds in scan angle (radians)
    double elevation_per_y;     //   elevation angle per detector line (radians)
    double scan_per_x;          //   scan angle per pixel (radians)
    double line_constant;
    double nomorb;          //   nominal radial distance of satellite (km)
    double flat  ;          //   earth flattening coefficient = 1-(minor/major)
    double e_sqr;
    double pitch;           //   pitch angles of instrument (rad)
    double roll;            //   roll angles of instrument (rad)
    double yaw;             //   yaw angles of instrument (rad)
    double x_scale;
    double x_start;
    double y_scale;
    double y_start;
    double subsat_lat;
    double subsat_lon;
    int imc;                //   imc on or off
}; 
typedef struct nav_constants nav_constants; 



extern "C"{
  extern void setcon_(int* instrument,
		      int * NS_NAD_CY, 
		      int * NS_NAD_INC, 
		      int * EW_NAD_CY,
		      int * EW_NAD_INC );
  
  extern void lmodel_(double * currentTime,
		      double * EpochTime, 
		      void * RECORD,
		      int * imc, 
		      double * rlat, 
		      double * rlon );
  
  extern double evln_(int * instrument, 
		     double * referenceLine);

  extern double scpx_(int * instrument, 
		     double * referencePixel);
  
  extern void lpoint_(double * elevationAngle, 
		      double * scanAngle, 
		      double * latitude, 
		      double * longitude, int * ier);

  extern void gpoint_(double * latitude, 
		      double * longitude, 
		      double * elevation, 
		      double * scan, 
		      int   * ier);

  extern void evsc2l_(int   * INSTRUMENT,
		      double * elevation, 
		      double * scan, 
		      double * refLn, 
		      double * refPx);
}

void gimlocInit(ImagerDoc & imagerDoc );


void lineXpixel2latXlon(double  line, 
			double  pixel,
			double & latitude, 
			double & longitude );
			 
void latXlon2lineXpixel(double  latitude, 
			double  longitude,
			double & line,
			double & pixel);

void timex(int ny, 
           int nd, 
           int nh, 
           int nm, 
  	   double  s, 
  	   double  & out_time);

void set_nav_constants( int instr  );
void print_nav_constants( nav_constants *g );

void scanelev_to_xy(int    instr, 
                    double *elev, 
                    double *scan, 
                    double *y,
                    double *x);

void xy_to_scanelev(int    instr, 
                    double *y,
                    double *x,
                    double *elev, 
                    double *scan);



