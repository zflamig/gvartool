#ifndef IMAGER_IR_CONSTANTS
#define IMAGER_IR_CONSTANTS

/* ConstantS for GOES from 

 http://www.oso.noaa.gov/goes/goes-calibration/gvar-conversion.htm 

 note that all constants are first defined match the layout of tables 
 on the referenced web page, then packed into more easily
 indexed arrays ...


  There are three steps to convert a 
   10-bit GVAR count value (0-1023) to temperature.

Step 1: Convert the GVAR count value to a radiance.
Step 2: Convert radiance to effective temperature using the inverse of the Planck function.
Step 3: Convert effective temperature Teff to actual temperature T (K) 

  */




/*

 I.Conversion of Imager GVAR Count to Scene Radiance

A 10-bit GVAR count value (0-1023) can be converted 
to a scene radiance according to the following equation:  

R = (X - b)/m,      
(1)

where R is radiance (mW/[m2-sr-cm-1]) and 
      X is the GVAR count value.  

The coefficients m and b are the scaling slope and intercept, 
respectively.  The values of m and b are listed in Table 1.
 They depend on the channel selected, but for a given channel 
they are constant for all time and are the same for all 
satellites of the series.

*/

/*Table 1-1. GOES-8 through -11 Imager Scaling Coefficients

        Channel        m        b           */

double v1_ir2[] = {227.3889, 68.2167 };
double v1_ir3[] = { 38.8383, 29.1287 };
double v1_ir4[] = {  5.2285, 15.6854 };
double v1_ir5[] = {  5.0273, 15.3332 };


/*Table 1-2. GOES-12 and -O Imager Scaling Coefficients

        Channel        m        b         */

double v2_ir2[] = {227.3889, 68.2167 };
double v2_ir3[] = { 38.8383, 29.1287 };
double v2_ir4[] = {  5.2285, 15.6854 };
double v2_ir6[] = {  5.5297, 16.5892 };


/*  indexing is IR2 == 1, IR3 == 2, etc, so first row is empty */
/*                               goes8       goes9      goes10    goes11        goes12     goes13     goes14     goes15  */ 

double scaling_coeff_m[6][8] = {{       0 ,        0 ,        0 ,        0 ,           0 ,        0 ,        0 ,        0 },
                                {v1_ir2[0], v1_ir2[0], v1_ir2[0], v1_ir2[0],    v2_ir2[0], v2_ir2[0], v2_ir2[0], v2_ir2[0]},
                                {v1_ir3[0], v1_ir3[0], v1_ir3[0], v1_ir3[0],    v2_ir3[0], v2_ir3[0], v2_ir3[0], v2_ir3[0]},
                                {v1_ir4[0], v1_ir4[0], v1_ir4[0], v1_ir4[0],    v2_ir4[0], v2_ir4[0], v2_ir4[0], v2_ir4[0]},
                                {v1_ir5[0], v1_ir5[0], v1_ir5[0], v1_ir5[0],           0 ,        0 ,        0 ,        0 },
                                {       0 ,        0 ,        0 ,        0 ,    v2_ir6[0], v2_ir6[0], v2_ir6[0], v2_ir6[0]}  };

double scaling_coeff_b[6][8] = {{       0 ,        0 ,        0 ,        0 ,           0 ,        0 ,        0 ,        0 },
                                {v1_ir2[1], v1_ir2[1], v1_ir2[1], v1_ir2[1],    v2_ir2[1], v2_ir2[1], v2_ir2[1], v2_ir2[1]},
                                {v1_ir3[1], v1_ir3[1], v1_ir3[1], v1_ir3[1],    v2_ir3[1], v2_ir3[1], v2_ir3[1], v2_ir3[1]},
                                {v1_ir4[1], v1_ir4[1], v1_ir4[1], v1_ir4[1],    v2_ir4[1], v2_ir4[1], v2_ir4[1], v2_ir4[1]},
                                {v1_ir5[1], v1_ir5[1], v1_ir5[1], v1_ir5[1],           0 ,        0 ,        0 ,        0 },
                                {       0 ,        0 ,        0 ,        0 ,    v2_ir6[1], v2_ir6[1], v2_ir6[1], v2_ir6[1]}  };




/*
Step 2: Convert radiance to effective temperature using the inverse 
of the Planck function as follows:

(c2 * n )
Teff =	
_____________________________
ln [1 + (c1 * n 3) / R]
(2)
c1 = 1.191066 x 10-5 [mW/(m2-sr-cm-4)]	 
c2 = 1.438833 (K/cm-1)	 
 

where Teff is effective temperature (K), 
      ln stands for natural logarithm, and 
      R is radiance. 

The coefficients n, c1, and c2 are the central wavenumber of the channel 
and the two radiation constants, respectively. 

The constants c1 and c2 are invariant, but n depends on the 
spectral characteristics of the channel and will vary from 
instrument to instrument.

Values of n are given in tables below step 3
*/



/*
Step 3: Convert effective temperature Teff to 
actual temperature T (K) using the following equation:  

T = a + b * Teff                     (3)
where a and b are two conversion coefficients.   

Note in the conversions that:

The values of n (cm-1) in step 2 and constants a and b 
in step 3 depend on channel and instrument. Their 
values are listed below in Tables 2-1 through 2-5.

The term side 1 or side 2 in the table headings indicates 
the operation of one of the two redundant sets of detectors 
and electronics on each imager. The coefficients n, a, and b 
depend on the choice of side. 

The GOES-8, -9, -11, -12 and -13 imagers have always operated on side 1. 
The GOES-10 imager is operating on side 2. 
The GOES-O imager is expect to operate on side 1 after launch on April 28, 2009.


We will provide the coefficients for other electronics sides when they are needed. 


*/

/*Table 2-1. GOES-8 Imager (Side 1) Coefficients          */
/*Channel/Detector      n           a             b       */
double g08_2a[] = { 2556.71   ,  -0.578526  ,  1.001512 } ; 
double g08_2b[] = { 2558.62   ,  -0.581853  ,  1.001532 } ; 
double g08_3a[] = { 1481.91   ,  -0.593903  ,  1.001418 } ; 
double g08_4a[] = {  934.30   ,  -0.322585  ,  1.001271 } ; 
double g08_4b[] = {  935.38   ,  -0.351889  ,  1.001293 } ; 
double g08_5a[] = {  837.06   ,  -0.422571  ,  1.001170 } ; 
double g08_5b[] = {  837.00   ,  -0.466954  ,  1.001257 } ; 

/*Table 2-2. GOES-9 Imager (Side 1) Coefficients          */
/*Channel/Detector      n           a             b       */
double g09_2a[] = { 2555.18   ,  -0.579908  ,  1.000942 } ; 
double g09_2b[] = { 2555.18   ,  -0.579908  ,  1.000942 } ; 
double g09_3a[] = { 1481.82   ,  -0.493016  ,  1.001076 } ; 
double g09_4a[] = {  934.59   ,  -0.384798  ,  1.001293 } ; 
double g09_4b[] = {  934.28   ,  -0.363703  ,  1.001272 } ; 
double g09_5a[] = {  834.02   ,  -0.302995  ,  1.000941 } ; 
double g09_5b[] = {  834.09   ,  -0.306838  ,  1.000948 } ; 

/*Table 2-3. GOES-10 Imager (Side 2) Coefficients         */
/*Channel/Detector      n           a             b       */
double g10_2a[] = { 2552.9845 ,  -0.60584483,  1.0011017 } ; 
double g10_2b[] = { 2552.9845 ,  -0.60584483,  1.0011017 } ; 
double g10_3a[] = { 1486.2212 ,  -0.61653805,  1.0014011 } ; 
double g10_4a[] = {  936.10260,  -0.27128884,  1.0009674 } ; 
double g10_4b[] = {  935.98981,  -0.27064036,  1.0009687 } ; 
double g10_5a[] = {  830.88473,  -0.26505411,  1.0009087 } ; 
double g10_5b[] = {  830.89691,  -0.26056452,  1.0008962 } ; 

/*Table 2-4. GOES-11 Imager (Side 1) Coefficients         */
/*Channel/Detector      n           a             b       */
double g11_2a[] = { 2562.07   ,  -0.644790  ,  1.000775 } ; 
double g11_2b[] = { 2562.07   ,  -0.644790  ,  1.000775 } ; 
double g11_3a[] = { 1481.53   ,  -0.543401  ,  1.001495 } ; 
double g11_4a[] = {  931.76   ,  -0.306809  ,  1.001274 } ; 
double g11_4b[] = {  931.76   ,  -0.306809  ,  1.001274 } ; 
double g11_5a[] = {  833.67   ,  -0.333216  ,  1.001000 } ; 
double g11_5b[] = {  833.04   ,  -0.315110  ,  1.000967 } ; 

/*Table 2-5. GOES-12 Imager (Side 1) Coefficients         */
/*Channel/Detector      n           a             b       */
double g12_2a[] = { 2562.45   ,  -0.650731  ,  1.001520 } ; 
double g12_2b[] = { 2562.45   ,  -0.650731  ,  1.001520 } ; 
double g12_3a[] = { 1536.43   ,  -4.764728  ,  1.012420 } ; 
double g12_3b[] = { 1536.94   ,  -4.775517  ,  1.012403 } ; 
double g12_4a[] = {  933.21   ,  -0.360331  ,  1.001306 } ; 
double g12_4b[] = {  933.21   ,  -0.360331  ,  1.001306 } ; 
double g12_6a[] = {  751.91   ,  -0.253449  ,  1.000743 } ; 

/*Table 2-6. GOES-13 Imager (Side 1) Coefficients         */
/*Channel/Detector      n           a             b       */
double g13_2a[] = { 2561.74   ,  -1.437204  ,  1.002562 } ; 
double g13_2b[] = { 2561.74   ,  -1.437204  ,  1.002562 } ; 
double g13_3a[] = { 1522.52   ,  -3.625663  ,  1.010018 } ; 
double g13_3b[] = { 1521.66   ,  -3.607841  ,  1.010010 } ; 
double g13_4a[] = {  937.23   ,  -0.386043  ,  1.001298 } ; 
double g13_4b[] = {  937.27   ,  -0.380113  ,  1.001285 } ; 
//double g13_6a[]= {  753.15   ,  -0.195055  ,  1.000610 } // ITT orig values
//double g13_6a[]= {  751.93   ,  -0.134688  ,  1.000481 } // ITT updated values
double g13_6a[] = {  749.83   ,  -0.134801  ,  1.000482 } ; // STAR updated values


/*Table 2-7. GOES-O Imager (Side 1) Coefficients          */
/*Channel/Detector      n           a             b       */
double g14_2a[] = { 2572.47   ,  -1.530285  ,  1.002507 } ; 
double g14_2b[] = { 2572.47   ,  -1.530285  ,  1.002507 } ; 
double g14_3a[] = { 1529.33   ,  -3.561161  ,  1.009501 } ; 
double g14_3b[] = { 1530.10   ,  -3.577037  ,  1.009444 } ; 
double g14_4a[] = {  934.04   ,  -0.263369  ,  1.001176 } ; 
double g14_4b[] = {  933.94   ,  -0.260576  ,  1.001179 } ; 
double g14_6a[] = {  753.38   ,  -0.199338  ,  1.000616 } ;
double g14_6b[] = {  753.91   ,  -0.234004  ,  1.000692 } ;


/*  
 note many values in the tables below are not the real values, but simply values 
  of a neighbor - for example 

    goes15-16 don't exist yet -  they use g14 values 
    similarly only one side is actually in use for each imager,
      so same values are used for both sides
    IR3 and IR6 only have 1 detector on some imagers
    first line is empty - it's just to get indexing correct
*/

/*                           goes8      goes9     goes10     goes11     goes12     goes13     goes14     goes15     goes16  */ 

double I_S0D0_Nu[6][9]= { {        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0  },
                          { g08_2a[0], g09_2a[0], g10_2a[0], g11_2a[0], g12_2a[0], g13_2a[0], g14_2a[0], g14_2a[0], g14_2a[0] },
                          { g08_3a[0], g09_3a[0], g10_3a[0], g11_3a[0], g12_3a[0], g13_3a[0], g14_3a[0], g14_3a[0], g14_3a[0] },
                          { g08_4a[0], g09_4a[0], g10_4a[0], g11_4a[0], g12_4a[0], g13_4a[0], g14_4a[0], g14_4a[0], g14_4a[0] },
                          { g08_5a[0], g09_5a[0], g10_5a[0], g11_5a[0],        0 ,        0 ,        0 ,        0 ,        0  },
                          {        0 ,        0 ,        0 ,        0 , g12_6a[0], g13_6a[0], g14_6a[0], g14_6a[0], g14_6a[0] }  };

double I_S0D1_Nu[6][9]= { {        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0  },
                          { g08_2b[0], g09_2b[0], g10_2b[0], g11_2b[0], g12_2b[0], g13_2b[0], g14_2b[0], g14_2b[0], g14_2b[0] },
                          {        0 ,        0 ,        0 ,        0 , g12_3b[0], g13_3b[0], g14_3b[0], g14_3b[0], g14_3b[0] },
                          { g08_4b[0], g09_4b[0], g10_4b[0], g11_4b[0], g12_4b[0], g13_4b[0], g14_4b[0], g14_4b[0], g14_4b[0] },
                          { g08_5b[0], g09_5b[0], g10_5b[0], g11_5b[0],        0 ,        0 ,        0 ,        0 ,        0  },
                          {        0 ,        0 ,        0 ,        0 ,        0 ,        0 , g14_6b[0], g14_6b[0], g14_6b[0] }  };



/*                                goes8      goes9     goes10     goes11     goes12     goes13     goes14     goes15     goes16  */ 

double I_S0D0_coeff_a[6][9]= { {        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0  },
                               { g08_2a[1], g09_2a[1], g10_2a[1], g11_2a[1], g12_2a[1], g13_2a[1], g14_2a[1], g14_2a[1], g14_2a[1] },
                               { g08_3a[1], g09_3a[1], g10_3a[1], g11_3a[1], g12_3a[1], g13_3a[1], g14_3a[1], g14_3a[1], g14_3a[1] },
                               { g08_4a[1], g09_4a[1], g10_4a[1], g11_4a[1], g12_4a[1], g13_4a[1], g14_4a[1], g14_4a[1], g14_4a[1] },
                               { g08_5a[1], g09_5a[1], g10_5a[1], g11_5a[1],        0 ,        0 ,        0 ,        0 ,        0  },
                               {        0 ,        0 ,        0 ,        0 , g12_6a[1], g13_6a[1], g14_6a[1], g14_6a[1], g14_6a[1] }  };

double I_S0D1_coeff_a[6][9]= { {        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0  },
                               { g08_2b[1], g09_2b[1], g10_2b[1], g11_2b[1], g12_2b[1], g13_2b[1], g14_2b[1], g14_2b[1], g14_2b[1] },
                               {        0 ,        0 ,        0 ,        0 , g12_3b[1], g13_3b[1], g14_3b[1], g14_3b[1], g14_3b[1] },
                               { g08_4b[1], g09_4b[1], g10_4b[1], g11_4b[1], g12_4b[1], g13_4b[1], g14_4b[1], g14_4b[1], g14_4b[1] },
                               { g08_5b[1], g09_5b[1], g10_5b[1], g11_5b[1],        0 ,        0 ,        0 ,        0 ,        0  },
                               {        0 ,        0 ,        0 ,        0 ,        0 ,        0 , g14_6b[1], g14_6b[1], g14_6b[1] }  };
			                                                                                                        


/*                                goes8      goes9     goes10     goes11     goes12     goes13     goes14     goes15     goes16  */ 

double I_S0D0_coeff_b[6][9]= { {        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0  },
                               { g08_2a[2], g09_2a[2], g10_2a[2], g11_2a[2], g12_2a[2], g13_2a[2], g14_2a[2], g14_2a[2], g14_2a[2] },
                               { g08_3a[2], g09_3a[2], g10_3a[2], g11_3a[2], g12_3a[2], g13_3a[2], g14_3a[2], g14_3a[2], g14_3a[2] },
                               { g08_4a[2], g09_4a[2], g10_4a[2], g11_4a[2], g12_4a[2], g13_4a[2], g14_4a[2], g14_4a[2], g14_4a[2] },
                               { g08_5a[2], g09_5a[2], g10_5a[2], g11_5a[2],        0 ,        0 ,        0 ,        0 ,        0  },
                               {        0 ,        0 ,        0 ,        0 , g12_6a[2], g13_6a[2], g14_6a[2], g14_6a[2], g14_6a[2] }  };

double I_S0D1_coeff_b[6][9]= { {        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0  },
                               { g08_2b[2], g09_2b[2], g10_2b[2], g11_2b[2], g12_2b[2], g13_2b[2], g14_2b[2], g14_2b[2], g14_2b[2] },
                               {        0 ,        0 ,        0 ,        0 , g12_3b[2], g13_3b[2], g14_3b[2], g14_3b[2], g14_3b[2] },
                               { g08_4b[2], g09_4b[2], g10_4b[2], g11_4b[2], g12_4b[2], g13_4b[2], g14_4b[2], g14_4b[2], g14_4b[2] },
                               { g08_5b[2], g09_5b[2], g10_5b[2], g11_5b[2],        0 ,        0 ,        0 ,        0 ,        0  },
                               {        0 ,        0 ,        0 ,        0 ,        0 ,        0 , g14_6b[2], g14_6b[2], g14_6b[2] }  };
			                                                                                                        

/*  make duplicate arrays for second side */


//double I_S1D0_Nu[6][9];
//for (int i=0; i<6*9, i++) I_S1D0_Nu[i] = I_S0D0_Nu[i];
//double I_S1D1_Nu[6][9];
//for (int i=0; i<6*9, i++) I_S1D1_Nu[i] = I_S0D1_Nu[i];
//
//double I_S1D0_coeff_a[6][9];
//for (int i=0; i<6*9, i++) I_S1D0_coeff_a[i] = I_S0D0_coeff_a[i];
//double I_S1D1_coeff_a[6][9];
//for (int i=0; i<6*9, i++) I_S1D1_coeff_a[i] = I_S0D1_coeff_a[i];
//
//double I_S1D0_coeff_b[6][9];
//for (int i=0; i<6*9, i++) I_S1D0_coeff_b[i] = I_S0D0_coeff_b[i];
//double I_S1D1_coeff_b[6][9];
//for (int i=0; i<6*9, i++) I_S1D1_coeff_b[i] = I_S0D1_coeff_b[i];

/*                           goes8      goes9     goes10     goes11     goes12     goes13     goes14     goes15     goes16  */ 

double I_S1D0_Nu[6][9]= { {        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0  },
                          { g08_2a[0], g09_2a[0], g10_2a[0], g11_2a[0], g12_2a[0], g13_2a[0], g14_2a[0], g14_2a[0], g14_2a[0] },
                          { g08_3a[0], g09_3a[0], g10_3a[0], g11_3a[0], g12_3a[0], g13_3a[0], g14_3a[0], g14_3a[0], g14_3a[0] },
                          { g08_4a[0], g09_4a[0], g10_4a[0], g11_4a[0], g12_4a[0], g13_4a[0], g14_4a[0], g14_4a[0], g14_4a[0] },
                          { g08_5a[0], g09_5a[0], g10_5a[0], g11_5a[0],        0 ,        0 ,        0 ,        0 ,        0  },
                          {        0 ,        0 ,        0 ,        0 , g12_6a[0], g13_6a[0], g14_6a[0], g14_6a[0], g14_6a[0] }  };

double I_S1D1_Nu[6][9]= { {        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0  },
                          { g08_2b[0], g09_2b[0], g10_2b[0], g11_2b[0], g12_2b[0], g13_2b[0], g14_2b[0], g14_2b[0], g14_2b[0] },
                          {        0 ,        0 ,        0 ,        0 , g12_3b[0], g13_3b[0], g14_3b[0], g14_3b[0], g14_3b[0] },
                          { g08_4b[0], g09_4b[0], g10_4b[0], g11_4b[0], g12_4b[0], g13_4b[0], g14_4b[0], g14_4b[0], g14_4b[0] },
                          { g08_5b[0], g09_5b[0], g10_5b[0], g11_5b[0],        0 ,        0 ,        0 ,        0 ,        0  },
                          {        0 ,        0 ,        0 ,        0 ,        0 ,        0 , g14_6b[0], g14_6b[0], g14_6b[0] }  };



/*                                goes8      goes9     goes10     goes11     goes12     goes13     goes14     goes15     goes16  */ 

double I_S1D0_coeff_a[6][9]= { {        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0  },
                               { g08_2a[1], g09_2a[1], g10_2a[1], g11_2a[1], g12_2a[1], g13_2a[1], g14_2a[1], g14_2a[1], g14_2a[1] },
                               { g08_3a[1], g09_3a[1], g10_3a[1], g11_3a[1], g12_3a[1], g13_3a[1], g14_3a[1], g14_3a[1], g14_3a[1] },
                               { g08_4a[1], g09_4a[1], g10_4a[1], g11_4a[1], g12_4a[1], g13_4a[1], g14_4a[1], g14_4a[1], g14_4a[1] },
                               { g08_5a[1], g09_5a[1], g10_5a[1], g11_5a[1],        0 ,        0 ,        0 ,        0 ,        0  },
                               {        0 ,        0 ,        0 ,        0 , g12_6a[1], g13_6a[1], g14_6a[1], g14_6a[1], g14_6a[1] }  };

double I_S1D1_coeff_a[6][9]= { {        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0  },
                               { g08_2b[1], g09_2b[1], g10_2b[1], g11_2b[1], g12_2b[1], g13_2b[1], g14_2b[1], g14_2b[1], g14_2b[1] },
                               {        0 ,        0 ,        0 ,        0 , g12_3b[1], g13_3b[1], g14_3b[1], g14_3b[1], g14_3b[1] },
                               { g08_4b[1], g09_4b[1], g10_4b[1], g11_4b[1], g12_4b[1], g13_4b[1], g14_4b[1], g14_4b[1], g14_4b[1] },
                               { g08_5b[1], g09_5b[1], g10_5b[1], g11_5b[1],        0 ,        0 ,        0 ,        0 ,        0  },
                               {        0 ,        0 ,        0 ,        0 ,        0 ,        0 , g14_6b[1], g14_6b[1], g14_6b[1] }  };
			                                                                                                        


/*                                goes8      goes9     goes10     goes11     goes12     goes13     goes14     goes15     goes16  */ 

double I_S1D0_coeff_b[6][9]= { {        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0  },
                               { g08_2a[2], g09_2a[2], g10_2a[2], g11_2a[2], g12_2a[2], g13_2a[2], g14_2a[2], g14_2a[2], g14_2a[2] },
                               { g08_3a[2], g09_3a[2], g10_3a[2], g11_3a[2], g12_3a[2], g13_3a[2], g14_3a[2], g14_3a[2], g14_3a[2] },
                               { g08_4a[2], g09_4a[2], g10_4a[2], g11_4a[2], g12_4a[2], g13_4a[2], g14_4a[2], g14_4a[2], g14_4a[2] },
                               { g08_5a[2], g09_5a[2], g10_5a[2], g11_5a[2],        0 ,        0 ,        0 ,        0 ,        0  },
                               {        0 ,        0 ,        0 ,        0 , g12_6a[2], g13_6a[2], g14_6a[2], g14_6a[2], g14_6a[2] }  };

double I_S1D1_coeff_b[6][9]= { {        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0 ,        0  },
                               { g08_2b[2], g09_2b[2], g10_2b[2], g11_2b[2], g12_2b[2], g13_2b[2], g14_2b[2], g14_2b[2], g14_2b[2] },
                               {        0 ,        0 ,        0 ,        0 , g12_3b[2], g13_3b[2], g14_3b[2], g14_3b[2], g14_3b[2] },
                               { g08_4b[2], g09_4b[2], g10_4b[2], g11_4b[2], g12_4b[2], g13_4b[2], g14_4b[2], g14_4b[2], g14_4b[2] },
                               { g08_5b[2], g09_5b[2], g10_5b[2], g11_5b[2],        0 ,        0 ,        0 ,        0 ,        0  },
                               {        0 ,        0 ,        0 ,        0 ,        0 ,        0 , g14_6b[2], g14_6b[2], g14_6b[2] }  };










#endif
