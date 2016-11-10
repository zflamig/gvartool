#include "setRadiometric.h"
#include "include/imager_ir_constants.h"
#include "include/imager_vis_constants.h"
#include "include/sounder_constants.h"
#include "utils/crcCheck/crcCheck.h"
#include "gvarBlock/gvarBlock.h"  

void resetVisibleRadiometricConstants
(Radiometric * Iunits[CHANNELS][SIDES][DETECTORS],
 ImagerSpacelook & imagerSpacelook){ 

  // only the zero point (bias) changes  
for (int side=0; side<Side[imager][I1]; side++)
{ // duplicate the data for sides 1 and 2 of visible channel
  // because we don't know any better
  for ( int d = 0; d < IDETECTORS; d ++ ){ 
    Iunits[I1][side][d]->setScalingBandG(imagerSpacelook.postClampFilteredMean(I1,d),
                                         Iunits[I1][side][d]->getScalingGain() ); 
  }
}
}

extern int GVersion; // see gvar.C (gvar.G12.C)

void setRadiometric
(Radiometric * Iunits[CHANNELS][SIDES][DETECTORS],
 ImagerDoc & imagerDoc ){


   // sat indexing is GOES-8  == 0
   //                 GOES-9  == 1
   //                 ...
   //                 GOES-14 == 6, etc

   int sat = ( imagerDoc.spcid() ) - 8 ; 
   if (sat<0 || sat>7) sat = 0; // GOES8 is default



  // visible channel, 8 detectors, set both sides to same values
  for (int side=0; side<Side[imager][I1]; side++)
  { 
    if(Iunits[I1][side][0] != NULL) { 

      for ( int d = 0; d < IDETECTORS ; d++){
        (Iunits[I1][side][d])->radianceToAlbedo() = i_vis_k[sat];
        (Iunits[I1][side][d])->setScalingMandC( i_vis_m[d][sat], -i_vis_m[d][sat]*i_vis_x0[sat]   );

	//   (Iunits[I1][side][d])->nu() = i_vis_nu[d][sat]; - not used anywhere
	//        (Iunits[I1][side][d])->setScalingMandC(  (double)imagerDoc.Ivcr1[d],  (double)imagerDoc.Ivcrb[d]   );

      }
    }
  }


  // set IR coefficients, once again fill in both sides even though only 1 is active at a time
  for (int chan=1; chan<6; chan++) 
  {
     if( Iunits[chan][0][0] != NULL )
     {
       ( Iunits[chan][0][0])->nu( ) = I_S0D0_Nu[chan][sat];
       ( Iunits[chan][0][0])->set_coeff_a_and_b( I_S0D0_coeff_a[chan][sat], I_S0D0_coeff_b[chan][sat]);
       ( Iunits[chan][0][0])->setScalingBandG( scaling_coeff_b[chan][sat],   scaling_coeff_m[chan][sat]);
     }
     if( Iunits[chan][0][1] != NULL )
     {
       ( Iunits[chan][0][1])->nu( ) = I_S0D1_Nu[chan][sat];
       ( Iunits[chan][0][1])->set_coeff_a_and_b( I_S0D1_coeff_a[chan][sat], I_S0D1_coeff_b[chan][sat]);
       ( Iunits[chan][0][1])->setScalingBandG( scaling_coeff_b[chan][sat],   scaling_coeff_m[chan][sat]);    
     }
     if( Iunits[chan][1][0] != NULL )
     {
       ( Iunits[chan][1][0])->nu( ) = I_S1D0_Nu[chan][sat];
       ( Iunits[chan][1][0])->set_coeff_a_and_b( I_S1D0_coeff_a[chan][sat], I_S1D0_coeff_b[chan][sat]);
       ( Iunits[chan][1][0])->setScalingBandG( scaling_coeff_b[chan][sat],   scaling_coeff_m[chan][sat]);
     }
     if( Iunits[chan][1][1] != NULL )
     {
       ( Iunits[chan][1][1])->nu( ) = I_S1D1_Nu[chan][sat];
       ( Iunits[chan][1][1])->set_coeff_a_and_b( I_S1D1_coeff_a[chan][sat], I_S1D1_coeff_b[chan][sat]);
       ( Iunits[chan][1][1])->setScalingBandG( scaling_coeff_b[chan][sat],   scaling_coeff_m[chan][sat]);
     }
  }


}





void setRadiometric
(  Radiometric * Sunits[CHANNELS][SIDES][DETECTORS], 
 SounderDoc &  sounderDoc){

   // sat indexing is GOES-8  == 0
   //                 GOES-9  == 1
   //                 ...
   //                 GOES-14 == 6, etc
  int sat = ( sounderDoc.spcid( ) -8 );
  if ( sat<0 || sat>6) sat = 0; // see similar situation above

  int side = 0;



  //  std::cout << "\n\n Sounder coef  \n" <<  std::flush;


  // visible channel, 4 detectors, set both sides to same values
  for ( int d = 0; d < SDETECTORS ; d++){

    if ( Sunits[SVIS][side][d] != NULL ) {

      // for now don't use sounderdoc values, use those from web site
      //        Sunits[SVIS][side][d]->radianceToAlbedo( ) =  sounderDoc.Svral;
      //        Sunits[SVIS][side][d]->setScalingMandC( sounderDoc.Svcr1[d], sounderDoc.Svcrb[d]);
      // 	std::cout << " " << sounderDoc.Svral << " " << sounderDoc.Svcrb[d] << " " << sounderDoc.Svcr1[d]  << "\n" <<  std::flush;

	 (Sunits[SVIS][side][d])->radianceToAlbedo() = s_vis_k[sat];
	 (Sunits[SVIS][side][d])->setScalingMandC( s_vis_m[sat][d], -s_vis_m[sat][d]*s_vis_x0[sat]   );


      }
  }
  


  //   std::cout << "\n\n Sounder coef  \n" <<  std::flush;

  // infrared channels
  // s_ir_all is array of all sounder coefficients from tables 4-1a through 4-7a at
  //  http://www.oso.noaa.gov/goes/goes-calibration/gvar-conversion.htm
  //  array elements are  0 == nu
  //                      1 == a
  //                      2 == b
  for( int chan = 0; chan < SCHANNELS-1 ; chan++){
    for( int det =0; det < SDETECTORS ; det++) {
      if ( Sunits[chan][side][det] != NULL ){

        Sunits[chan][side][det]->nu( ) = s_ir_all[sat][chan][det][0];
        Sunits[chan][side][det]->set_coeff_a_and_b(  s_ir_all[sat][chan][det][1],    s_ir_all[sat][chan][det][2]);
	Sunits[chan][side][det]->setScalingBandG( sounderDoc.Sisfb[chan][det],sounderDoc.Sisf1[chan][det]); 

 	//std::cout << " " << sounderDoc.Sisfb[chan][det] << " " << sounderDoc.Sisf1[chan][det]  << "\n" <<  std::flush;
        }
     }
  }
  

}



void printRadiometric
(Radiometric * Iunits[CHANNELS][SIDES][DETECTORS]){

   int ss = 0;
   int cc = 0;
   int dd = 0;

   std::cout.precision( 12 );


   std::cout << "\n\n Imager radiometric constants have been set   \n" <<  std::flush;


   std::cout << "\n    Visible channel counts to radiance:\n" <<  std::flush;
   for (dd = 0; dd < 8 ; dd++) {
      std::cout << " detector " << dd+1 << ":  Radiance = " <<  Iunits[cc][ss][dd]->getScalingM() << " * counts " << Iunits[cc][ss][dd]->getScalingC() << " \n" <<  std::flush;
   }

   std::cout << "\n    Visible channel radiance to effective albedo:\n" <<  std::flush;
   std::cout << " all detectors:  Albedo = " <<  Iunits[cc][ss][0]->radianceToAlbedo() << " * Radiance \n" <<  std::flush;




   std::cout << "\n    Infrared channel counts to radiance:\n" <<  std::flush;
   dd = 0; // all detectors have same value
   for (cc=1; cc<6; cc++)    {
     if( Iunits[cc][ss][dd] != NULL )     {
       if (Iunits[cc][ss][dd]->getScalingGain() != 0) {
	std::cout << " channel " << cc+1 << ":  Radiance =  (counts - " << Iunits[cc][ss][dd]->getScalingBias() << ") / " <<  Iunits[cc][ss][dd]->getScalingGain() << " \n" <<  std::flush;
       }
     }
   }

   std::cout << "\n    Infrared channel radiance to effective brightness temperature:\n" <<  std::flush;
   std::cout << "      calculate inverse planck funtion using central wavelengths listed below\n" <<  std::flush;
   for (cc=1; cc<6; cc++)  {
      for (dd = 0; dd < 2 ; dd++)  {
         if ( Iunits[cc][ss][dd] != NULL )  {
            if (Iunits[cc][ss][dd]->nu() != 0) {
	      std::cout << " channel " << cc+1 << ", detector "<<dd+1<<":  Nu = " << Iunits[cc][ss][dd]->nu() << " \n" <<  std::flush;
            }
         }
      }
   }


   std::cout << "\n    Infrared channel effective to actual brightness temperature:\n" <<  std::flush;

   for (cc=1; cc<6; cc++)   {
      for (dd = 0; dd < 2 ; dd++)  {
        if ( Iunits[cc][ss][dd] != NULL )     {
           if (Iunits[cc][ss][dd]->get_coeff_b() != 0) {
	      std::cout << " channel " << cc+1 << ", detector  "<<dd+1<<":  T(actual) = " << Iunits[cc][ss][dd]->get_coeff_b() << " * T(eff) + " <<  Iunits[cc][ss][dd]->get_coeff_a() << " \n" <<  std::flush;
           }
       }
     }
   }



   std::cout << "\n\n" <<  std::flush;


}



void print_s_Radiometric
(Radiometric * Sunits[CHANNELS][SIDES][DETECTORS]){

   int ss = 0;
   int cc = 0;
   int dd = 0;

   std::cout.precision( 12 );


   std::cout << "\n\n Sounder radiometric constants have been set   \n" <<  std::flush;




   std::cout << "\n    Visible channel counts to radiance:\n" <<  std::flush;
   for (dd = 0; dd < 4 ; dd++) {
      std::cout << " detector " << dd+1 << ":  Radiance = " <<  Sunits[SVIS][ss][dd]->getScalingM() << " * counts " << Sunits[SVIS][ss][dd]->getScalingC() << " \n" <<  std::flush;
   }

   std::cout << "\n    Visible channel radiance to effective albedo:\n" <<  std::flush;
   std::cout << " all detectors:  Albedo = " <<  Sunits[SVIS][ss][0]->radianceToAlbedo() << " * Radiance \n" <<  std::flush;




   std::cout << "\n    Infrared channel counts to radiance:\n" <<  std::flush;
   dd = 0; // all detectors have same value
   for (cc=0; cc<18; cc++)    {
     if( Sunits[cc][ss][dd] != NULL )     {
       if (Sunits[cc][ss][dd]->getScalingGain() != 0) {
	std::cout << " channel " << cc+1 << ":  Radiance =  (counts - " << Sunits[cc][ss][dd]->getScalingBias() << ") / " <<  Sunits[cc][ss][dd]->getScalingGain() << " \n" <<  std::flush;
       }
     }
   }

   std::cout << "\n    Infrared channel radiance to effective brightness temperature:\n" <<  std::flush;
   std::cout << "      calculate inverse planck funtion using central wavelengths listed below\n" <<  std::flush;
   for (cc=0; cc<18; cc++)  {

	      std::cout << " channel " << cc+1 << ",   Nu =  [ ";

      for (dd = 0; dd < 4 ; dd++)  {
         if ( Sunits[cc][ss][dd] != NULL )  {
            if (Sunits[cc][ss][dd]->nu() != 0) {
	      std::cout << Sunits[cc][ss][dd]->nu();
	      if (dd != 3) std::cout <<  ", ";
            }
         }
      }

	      std::cout  << " ] \n" <<  std::flush;

   }


   std::cout << "\n    Infrared channel effective to actual brightness temperature:\n" <<  std::flush;

   for (cc=0; cc<18; cc++)   {
      for (dd = 0; dd < 4 ; dd++)  {
        if ( Sunits[cc][ss][dd] != NULL )     {
           if (Sunits[cc][ss][dd]->get_coeff_b() != 0) {
	      std::cout << " channel " << cc+1 << ", detector  "<<dd+1<<":  T(actual) = " << Sunits[cc][ss][dd]->get_coeff_b() << " * T(eff) + " <<  Sunits[cc][ss][dd]->get_coeff_a() << " \n" <<  std::flush;
           }
       }
     }
   }



   std::cout << "\n\n" <<  std::flush;


}







