#ifndef RADIOMETRIC_H
#define RADIOMETRIC_H

#include "include/gvar.h"
#include <math.h>
#include <string.h>
#include "include/types.h"

/* y = Mx + C = (x-B)/G  */

static const char * RadiometricString[] = { 
  "undef",
  "count",
  "rdnce",
  "tstar",
  "tscen",
  "modea",
  "albdo",
  "albd2"
};


#define MAXDATABITS 10

#define SIGN(R) ((R<0.0)?-1.0:1.0 )

#define USERVAL(val) \
(SIGN(val)*pow(fabs(UserC+UserM*val),UserG))\



/*
Step 1: Conversion of Imager GVAR Count to Scene Radiance

A 10-bit GVAR count value (0-1023) can be converted 
to a scene radiance according to the following equation:  

R = (X - b)/m, 

where R is radiance (mW/[m2-sr-cm-1]) and X is the GVAR count value.
The coefficients m and b are the scaling slope and intercept, respectively.
The values of m and b are listed in Table 1. They depend on the channel 
selected, but for a given channel they are constant for all time 
and are the same for all satellites of the series.

Table 1-1. GOES-8 through -11 Imager Scaling Coefficients

Channel m      b
2  227.3889  68.2167
3   38.8383  29.1287
4    5.2285  15.6854
5    5.0273  15.3332

Table 1-2. GOES-12 and -O Imager Scaling Coefficients

Channel m    b
2 227.3889  68.2167
3  38.8383  29.1287
4   5.2285  15.6854
6   5.5297  16.5892
*/
#define RADIANCE(kounts) (ScalingM*kounts + ScalingC)

/*
Step 2: Convert radiance to effective temperature using 
the inverse of the Planck function as follows:

                (c2 * n )
Teff = _____________________________
           ln [1 + (c1 * n 3) / R]

c1 = 1.191066 x 10-5 [mW/(m2-sr-cm-4)]  
c2 = 1.438833 (K/cm-1)
*/
#define BRIGHTNESS_TEMPERATURE(kounts) \
(InversePlanckFunction(RADIANCE(kounts)) )



/*
Step 3: Convert effective temperature Teff to actual 
temperature T (K) using the following equation:  

T = a + b * Teff	

where a and b are two conversion coefficients.
tables of a and b are in SatelliteConstants.h

*/
#define SCENE_TEMPERATURE(kounts) \
(coeff_a + coeff_b * BRIGHTNESS_TEMPERATURE(kounts) )


#define ALBEDO(kounts) \
(100.0*RadianceToAlbedo*RADIANCE(kounts))\





class Radiometric{ 
 private:

  double Nu; 
  double ScalingM, ScalingC; 
  double coeff_b, coeff_a; 
  double UserM, UserC, UserG;
  double MapDot; 

  double RadianceToAlbedo; 
  RadiometricUnits Units; 
 
  long long Cnvrt; 

  WORD_TYPE WordType;
  void * Table ;

  double InversePlanckFunction(double R);
  void CreateLookupTable();
  void CopyToTable(int entry, double data, void * Table );
  void reComputeLookupTable();
 
 public: 

  Radiometric();
  ~Radiometric();

  void setWordType( WORD_TYPE dtype = Short );  

  void setScalingBandG(double b=0, double g=1); 
  void setScalingMandC(double m=1, double c=0);

  void setSceneTempBandG(double b=0, double g=1);
  void set_coeff_a_and_b(double a=0, double b=1);

  void setUserBandGG(double b=0, double g=1, double gamma =1);
  void setUserMandCG(double m=1, double c=0, double gamma = 1);
  void setMapDot(double mDot);

  double getScalingGain();
  double getScalingBias();
  double getScalingM();
  double getScalingC();
 
  double getSceneTempBias();
  double getSceneTempGain();
  double get_coeff_b();
  double get_coeff_a();
 
  double getUserBias();
  double getUserGain();
  double getUserGamma();
  double getUserM();
  double getUserC();

  double & nu();
  double & radianceToAlbedo();
  RadiometricUnits & units();

  double radiance(unsigned short kounts);
  double albedo  (unsigned short kounts) ;
  double brightnessTemperature(unsigned short kounts);
  double sceneTemperature(unsigned short kounts);
  double MODEA(unsigned short kounts);

  double getRadiometric(unsigned short kounts);
  double getConverted(unsigned short kount);

  double getMax();
  double getMin();

  void * convert(unsigned short  kounts);
  void * convert(unsigned short *kounts, int size, void * buf = NULL);

  const char * unitsStr();


};


/*
Step 2: Convert radiance to effective temperature using 
the inverse of the Planck function as follows:

                (c2 * n )
Teff = _____________________________
           ln [1 + (c1 * n 3) / R]

c1 = 1.191066 x 10-5 [mW/(m2-sr-cm-4)]  
c2 = 1.438833 (K/cm-1)
*/

inline double Radiometric::InversePlanckFunction(double R){
  // Nu in cm-1
  // R in erg/cm2/sec/steradian/cm-1

  double c_1 =1.191066E-5 ; 
  // 2*6.626176E-27*2.99792458E10*2.99792458E10 ; // 2h c^2
  double c_2 = 1.438833; 
  // 6.626176E-27*2.99792458E10/1.380662E-16 ; // hc/k 

  double Num =  c_2 * Nu;
  double Den =  1.0 + (c_1*Nu*Nu*Nu) / fabs(R); 

  return ( SIGN(R) * Num/log(Den) );
}


inline double & Radiometric::nu() 
{ return Nu; }

inline double & Radiometric::radianceToAlbedo()
{ return RadianceToAlbedo; }

inline RadiometricUnits & Radiometric::units() 
{ return Units; }

inline void Radiometric::setWordType( WORD_TYPE dtype ) 
{ WordType = dtype; }

inline  double Radiometric::radiance(unsigned short kounts)
{ return RADIANCE(kounts); }

inline  double Radiometric:: albedo  (unsigned short kounts) 
{ return ALBEDO(kounts); }

inline double Radiometric::brightnessTemperature(unsigned short kounts)
{ return BRIGHTNESS_TEMPERATURE(kounts); }

inline double Radiometric::sceneTemperature( unsigned short kounts)
{ return SCENE_TEMPERATURE(kounts); }

inline const char * Radiometric::unitsStr() 
{ return RadiometricString[Units]; }

inline void Radiometric::reComputeLookupTable()
{ if (Table) { delete (char *)Table; Table = NULL ; } }

inline void Radiometric::setScalingBandG(double b, double g) 
{ ScalingM = 1.0/g; ScalingC = -b/g; reComputeLookupTable();}

inline void Radiometric::setScalingMandC(double m, double c) 
{ ScalingM = m; ScalingC = c;  reComputeLookupTable();}

inline void Radiometric::setSceneTempBandG(double b, double g)
{ coeff_b = 1.0/g; coeff_a = -b/g;  reComputeLookupTable();}

inline void Radiometric::set_coeff_a_and_b(double a, double b)
{ coeff_b = b; coeff_a = a;  reComputeLookupTable();}

inline void Radiometric::setUserBandGG(double b, double g, double gamma)
{ UserM = 1.0/g; UserC = -b/g;  UserG = gamma; reComputeLookupTable();}

inline void Radiometric::setUserMandCG(double m, double c, double gamma)
{ UserM = m; UserC = c;  UserG = gamma; reComputeLookupTable();}

inline void Radiometric::setMapDot(double mDot)
{ MapDot = mDot; }

inline double Radiometric::getScalingGain()
{ return 1.0/ScalingM; }

inline double Radiometric::getScalingBias()
{ return -ScalingC/ScalingM; }

inline double Radiometric::getScalingM()
{ return ScalingM; }

inline double Radiometric::getScalingC()
{ return ScalingC; }

inline double Radiometric::getSceneTempBias()
{ return -coeff_a/coeff_b; }

inline double Radiometric::getSceneTempGain()
{ return 1.0/coeff_b; }

inline double Radiometric::get_coeff_b()
{ return coeff_b; }

inline double Radiometric::get_coeff_a()
{ return coeff_a; }

inline double Radiometric::getUserBias()
{ return -UserC/UserM; }

inline double Radiometric::getUserGain()
{ return 1.0/UserM; }

inline double Radiometric::getUserM()
{ return UserM; }

inline double Radiometric::getUserC()
{ return UserC; }

inline double Radiometric::getUserGamma()
{ return UserG; }

#endif // RADIOMETRIC_CONVERSIONS_H









