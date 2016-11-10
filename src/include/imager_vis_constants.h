#ifndef IMAGER_VIS_CONSTANTS
#define IMAGER_VIS_CONSTANTS

/* ConstantS for GOES from 

 http://www.oso.noaa.gov/goes/goes-calibration/goes-vis-ch-calibration.htm

 note that all constants are first defined match the layout of tables 
 on the referenced web page, then packed into more easily
 indexed arrays ...


  There are three steps to convert a 
   10-bit GVAR count value (0-1023) to albedo

Step 1: Convert the GVAR count value to a radiance.
Step 2: Convert radiance to albedo

*/

/* Step 1
Although the visible detector-channels are not calibrated in orbit, calibration coefficients measured by ITT before launch[1] are transmitted to users in the GVAR data stream. A factor that converts radiance to reflectance factor, or effective albedo, is included as well. Since detector responsivities can vary unpredictably, the pre-launch calibration may not be valid after launch. The calibration equation is either Equation (1)

		R = m X + b			(1)

or Equation (2)

 		R = m (X -Xsp),			(2)

where X is the instrument output in counts, the subscript sp refers to the view of space, and m and b are the calibration coefficients measured before launch[1]. For each visible-channel detector, the radiance R is the average of the spectral (monochromatic) radiance over the spectral response function for that detector, i.e.,

  R = (integral of R(lambda)PHI(lambda) )/(integral of PHI(lambda) )    	(3)

where lambda is wavelength in µm, PHI the spectral response function, and R(lambda ) the spectral radiance of the target. Units of R are W/(m2-sr-µm).

The value of b in Equation (1) depends on the electronic zero level. This level varies with a standard deviation of approximately one count for the imagers and tens of counts for the sounders. Therefore, when the satellite is in orbit, the value of b determined in the laboratory (or at any other earlier time) may not be valid. Equation (1) is preferred[2]. Visible-channel data from the GOES-8 and -9 imagers (but not GOES-10 through GOES-13, nor any sounder) are being normalized. Since normalization makes the responses from all eight imager detectors the same as that of the reference detector, users of the pre-launch calibration of the GOES-8 and -9 imagers should apply the calibration coefficients for the reference detector (identified in Table 1) to the data from all detectors. With relativization enabled, an instrument's output at the view of space is modified slightly, and the value of b in the calibration equation (1) needs to be modified accordingly. For this reason also, the best approach is to use Equation (2). If Equation (1) must be used, then the value of b should be determined from the equation

  	         b = - m Xo,			(4)

in which, for the imagers, m is the slope for the reference detector. For the sounders, it is the slope for an individual detector. Values of b determined in this way, as well as the values of m and X0, appear in Tables 1 - 6.
*/


/*
Step 2

The reflectance factor (or effective albedo) is obtained from the radiance by

		A = k R,				(5)  where

		k = p / H,			        (6)

and where H is the solar spectral irradiance H(lamda) averaged over the spectral 
response function of the visible detector, i.e.,

 h = (integral of H(lambda)PHI(lambda) )/(integral of PHI(lambda) )      (7)

Values of H were computed by ITT[1] from tables of solar irradiance vs wavelength 
provided by Rossow et al.[3], whose values are based on measurements by 
Neckel and Labs[4]. 
The values of A lie between 0 and 1. The value of 1 corresponds to the 
radiance of a perfectly reflecting diffuse surface illuminated at 
normal incidence when the sun is at its annual-average distance from the Earth. 
Values of k appear in Tables 1 - 6.

*/

/*
Table 1. Visible-channel calibration coefficients for GOES-8 and -9 imagers

Satellite                       GOES-8             GOES-9
Identity of reference detector   2                  3
(number in physical array)
m (reference detector)           0.5501873          0.5492361
(W/(m2-sr-m-count)) =
X0                              29                  29
b (W/(m2-sr-µm))               -15.955             -15.928
k ((m2-sr-µm)/W)                 1.92979 x 10-3      1.94180 x 10-3
*/

double g08_vis_x0 =  29;
double g08_vis_k  =  1.92979e-3;

double g08_vis_m = 0.5501873;
double g08_vis_b = -15.955;

/* goes-8 values from factory coef in block 0
 d 0 ivcrb -15.4116 ivcr1 0.552808  
 d 1 ivcrb -15.3044 ivcr1 0.550187  
 d 2 ivcrb -15.389  ivcr1 0.553975  
 d 3 ivcrb -15.2684 ivcr1 0.550833  
 d 4 ivcrb -15.3111 ivcr1 0.550946  
 d 5 ivcrb -15.273  ivcr1 0.55219   
 d 6 ivcrb -15.3534 ivcr1 0.550459  
 d 7 ivcrb -15.33   ivcr1 0.550728  
*/




double g09_vis_x0 =  29;
double g09_vis_k  =  1.94180e-3;

double g09_vis_m = 0.5492361;
double g09_vis_b = -15.928;



/*
Table 2. Visible-channel calibration coefficients for GOES-10 imagers
(normalization not enabled; no reference detector chosen)

Detector number       m (reference detector)           b                 
(in physical array)    W/(m2-sr-µm-count)           W/(m2-sr-µm) 
1                        0.5605602,                  -16.256
2                        0.5563529,                  -16.134      
3                        0.5566574,                  -16.143      
4                        0.5582154,                  -16.188      
5                        0.5583361,                  -16.192      
6                        0.5571736,                  -16.158      
7                        0.5563135,                  -16.133      
8                        0.5613536,                  -16.279

*/

double g10_vis_x0 =  29;
double g10_vis_k  =  1.98808e-3;

double g10_vis_m[8] = { 0.5605602, 0.5563529, 0.5566574, 0.5582154, 0.5583361, 0.5571736, 0.5563135, 0.5613536 };
double g10_vis_b[8] = { -16.256,   -16.134,   -16.143,   -16.188,   -16.192,   -16.158,   -16.133,   -16.279   };



/*
Table 3. Visible-channel calibration coefficients for GOES-11 imagers
(normalization not enabled; no reference detector chosen)

Detector number       m (reference detector)           b                 
(in physical array)    W/(m2-sr-µm-count)           W/(m2-sr-µm) 
1                        0.5561568,                  -16.129
2                        0.5552979,                  -16.104      
3                        0.5558981,                  -16.121      
4                        0.5577627,                  -16.175      
5                        0.5557238,                  -16.116      
6                        0.5587978,                  -16.205      
7                        0.5586530,                  -16.201      
8                        0.5528971,                  -16.034

*/

double g11_vis_x0 =  29;
double g11_vis_k  =  2.01524e-3;

double g11_vis_m[8] = { 0.5561568, 0.5552979, 0.5558981, 0.5577627, 0.5557238, 0.5587978, 0.5586530, 0.5528971 };
double g11_vis_b[8] = { -16.129,   -16.104,   -16.121,   -16.175,   -16.116,   -16.205,   -16.201,   -16.034   };


/*
Table 4. Visible-channel calibration coefficients for GOES-12 imagers
(normalization not enabled; no reference detector chosen)

Detector number       m (reference detector)           b                 
(in physical array)    W/(m2-sr-µm-count)           W/(m2-sr-µm) 
1                        0.5771030,                  -16.736
2                        0.5761764,                  -16.709      
3                        0.5775825,                  -16.750      
4                        0.5790699,                  -16.793      
5                        0.5787051,                  -16.782      
6                        0.5755969,                  -16.692      
7                        0.5753973,                  -16.687      
8                        0.5752099,                  -16.681

*/

double g12_vis_x0 =  29;
double g12_vis_k  =  1.97658e-3;

double g12_vis_m[8] = { 0.5771030, 0.5761764, 0.5775825, 0.5790699, 0.5787051, 0.5755969, 0.5753973, 0.5752099 };
double g12_vis_b[8] = { -16.736,   -16.709,   -16.750,   -16.793,   -16.782,   -16.692,   -16.687,   -16.681   };

/*
Table 5. Visible-channel calibration coefficients for GOES-13 imagers
(normalization not enabled; no reference detector chosen)

Detector number       m (reference detector)           b                 
(in physical array)    W/(m2-sr-µm-count)           W/(m2-sr-µm) 
1                        0.6120196,                  -17.749
2                        0.6118504,                  -17.744      
3                        0.6096360,                  -17.769      
4                        0.6087055,                  -17.653      
5                        0.6132860,                  -17.785      
6                        0.6118208,                  -17.743      
7                        0.6122307,                  -17.755      
8                        0.6066968,                  -17.594

*/

double g13_vis_x0 =  29;
double g13_vis_k  =  1.89544e-3;

double g13_vis_m[8] = { 0.6120196, 0.6118504, 0.6096360, 0.6087055, 0.6132860, 0.6118208, 0.6122307, 0.6066968 };
double g13_vis_b[8] = { -17.749,   -17.744,   -17.679,   -17.653,   -17.785,   -17.743,   -17.755,   -17.594   };

/*
Table 6. Visible-channel calibration coefficients for GOES-14 imagers
(normalization not enabled; no reference detector chosen)

Detector number       m (reference detector)           b                 
(in physical array)    W/(m2-sr-µm-count)           W/(m2-sr-µm) 
1                        0.5874693,                  -17.037
2                        0.5865367,                  -17.010      
3                        0.5862807,                  -17.002      
4                        0.5864086,                  -17.006      
5                        0.5857146,                  -16.986      
6                        0.5852004,                  -16.971      
7                        0.5860814,                  -16.996      
8                        0.5841697,                  -16.941

*/

double g14_vis_x0 =  29;
double g14_vis_k  =  1.88772e-3;

double g14_vis_m[8] = { 0.5874693, 0.5865367, 0.5862807, 0.5864086, 0.5857146, 0.5852004, 0.5860814, 0.5841697 };
double g14_vis_b[8] = { -17.037,   -17.010,   -17.002,   -17.006,   -16.986,   -16.971,   -16.996,   -16.941   };





/*
Table 7. Visible-channel calibration coefficients for GOES-15 imagers
(normalization not enabled; no reference detector chosen)

Detector number       m (reference detector)           b                 
(in physical array)    W/(m2-sr-µm-count)           W/(m2-sr-µm) 
1                        0.5874693,                  -17.037
2                        0.5865367,                  -17.010      
3                        0.5862807,                  -17.002      
4                        0.5864086,                  -17.006      
5                        0.5857146,                  -16.986      
6                        0.5852004,                  -16.971      
7                        0.5860814,                  -16.996      
8                        0.5841697,                  -16.941

*/







double g15_vis_x0 =  29;
double g15_vis_k  =  1.88852e-3;

double g15_vis_m[8] = { 0.5851966, 0.5879772, 0.5856793, 0.5854250, 0.5866992, 0.5836241, 0.5846555, 0.5843753 };
double g15_vis_b[8] = { -16.9707,  -17.0513,  -16.9847,  -16.9773,  -17.0143,  -16.9251,  -16.9550,  -16.9469  };



/* put constants from all satellites in one array */

double i_vis_x0[9] = { g08_vis_x0, g09_vis_x0,  g10_vis_x0, g11_vis_x0,  g12_vis_x0, g13_vis_x0,  g14_vis_x0, g15_vis_x0,  g15_vis_x0  };
double  i_vis_k[9] = { g08_vis_k,  g09_vis_k,   g10_vis_k,  g11_vis_k,   g12_vis_k,  g13_vis_k,   g14_vis_k,  g15_vis_k,   g15_vis_k   };

double  i_vis_m[8][9] = { { g08_vis_m,  g09_vis_m,   g10_vis_m[0],  g11_vis_m[0],   g12_vis_m[0],  g13_vis_m[0],   g14_vis_m[0],  g15_vis_m[0],   g15_vis_m[0] },
                          { g08_vis_m,  g09_vis_m,   g10_vis_m[1],  g11_vis_m[1],   g12_vis_m[1],  g13_vis_m[1],   g14_vis_m[1],  g15_vis_m[1],   g15_vis_m[1] },
                          { g08_vis_m,  g09_vis_m,   g10_vis_m[2],  g11_vis_m[2],   g12_vis_m[2],  g13_vis_m[2],   g14_vis_m[2],  g15_vis_m[2],   g15_vis_m[2] },
                          { g08_vis_m,  g09_vis_m,   g10_vis_m[3],  g11_vis_m[3],   g12_vis_m[3],  g13_vis_m[3],   g14_vis_m[3],  g15_vis_m[3],   g15_vis_m[3] },
                          { g08_vis_m,  g09_vis_m,   g10_vis_m[4],  g11_vis_m[4],   g12_vis_m[4],  g13_vis_m[4],   g14_vis_m[4],  g15_vis_m[4],   g15_vis_m[4] },
                          { g08_vis_m,  g09_vis_m,   g10_vis_m[5],  g11_vis_m[5],   g12_vis_m[5],  g13_vis_m[5],   g14_vis_m[5],  g15_vis_m[5],   g15_vis_m[5] },
                          { g08_vis_m,  g09_vis_m,   g10_vis_m[6],  g11_vis_m[6],   g12_vis_m[6],  g13_vis_m[6],   g14_vis_m[6],  g15_vis_m[6],   g15_vis_m[6] },
                          { g08_vis_m,  g09_vis_m,   g10_vis_m[7],  g11_vis_m[7],   g12_vis_m[7],  g13_vis_m[7],   g14_vis_m[7],  g15_vis_m[7],   g15_vis_m[7] }  };

/* since b == -m*X0, use that instead 
double  i_vis_b[8][9] = { { g08_vis_b,  g09_vis_b,   g10_vis_b[0],  g11_vis_b[0],   g12_vis_b[0],  g13_vis_b[0],   g14_vis_b[0],  g14_vis_b[0],   g14_vis_b[0] },
                          { g08_vis_b,  g09_vis_b,   g10_vis_b[1],  g11_vis_b[1],   g12_vis_b[1],  g13_vis_b[1],   g14_vis_b[1],  g14_vis_b[1],   g14_vis_b[1] },
                          { g08_vis_b,  g09_vis_b,   g10_vis_b[2],  g11_vis_b[2],   g12_vis_b[2],  g13_vis_b[2],   g14_vis_b[2],  g14_vis_b[2],   g14_vis_b[2] },
                          { g08_vis_b,  g09_vis_b,   g10_vis_b[3],  g11_vis_b[3],   g12_vis_b[3],  g13_vis_b[3],   g14_vis_b[3],  g14_vis_b[3],   g14_vis_b[3] },
                          { g08_vis_b,  g09_vis_b,   g10_vis_b[4],  g11_vis_b[4],   g12_vis_b[4],  g13_vis_b[4],   g14_vis_b[4],  g14_vis_b[4],   g14_vis_b[4] },
                          { g08_vis_b,  g09_vis_b,   g10_vis_b[5],  g11_vis_b[5],   g12_vis_b[5],  g13_vis_b[5],   g14_vis_b[5],  g14_vis_b[5],   g14_vis_b[5] },
                          { g08_vis_b,  g09_vis_b,   g10_vis_b[6],  g11_vis_b[6],   g12_vis_b[6],  g13_vis_b[6],   g14_vis_b[6],  g14_vis_b[6],   g14_vis_b[6] },
                          { g08_vis_b,  g09_vis_b,   g10_vis_b[7],  g11_vis_b[7],   g12_vis_b[7],  g13_vis_b[7],   g14_vis_b[7],  g14_vis_b[7],   g14_vis_b[7] }  };
*/




/* vis channel Nu is not used for anything */
/* these are all spurious values anyway   */
/* Visible channel Nu - Central Wavenumber */
double i_vis_nu[8][9] = {
    { 14367, 14367, 14367, 14367, 14367, 14367, 14367, 14367, 14367 },
    { 14367, 14367, 14367, 14367, 14367, 14367, 14367, 14367, 14367 },
    { 14367, 14367, 14367, 14367, 14367, 14367, 14367, 14367, 14367 },
    { 14367, 14367, 14367, 14367, 14367, 14367, 14367, 14367, 14367 },
    { 14367, 14367, 14367, 14367, 14367, 14367, 14367, 14367, 14367 },
    { 14367, 14367, 14367, 14367, 14367, 14367, 14367, 14367, 14367 },
    { 14367, 14367, 14367, 14367, 14367, 14367, 14367, 14367, 14367 },
    { 14367, 14367, 14367, 14367, 14367, 14367, 14367, 14367, 14367 }  };



#endif
