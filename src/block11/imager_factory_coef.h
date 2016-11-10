#ifndef IFCOEFF_H
#define IFCOEFF_H

//#include <stream.h>
#include <iostream>
#include "include/types.h"

class f_coeff{
 private:

  uint16 status;		/* 1 2 */
  uint16 t0;			/*  3 4 */

  uint32 t1;			/* 5 6 */

  uint16 t2;			/* 9 10 */
  uint16 EWcycles;		/* 11 12 */

  uint16 EWincrement;		/* 13 14 */
  uint16 NScycles;		/* 15 16 */

  uint16 NSincrement;		/* 17 18 */
  uint16 NScompensation;	/* 19 20 */

  uint16 EWcompensation;	/* 21 22 */
  uint16 NSservoError;		/* 23 24 */

  uint16 EWservoError;		/* 25 26 */
  uint16 next; 

 public:
  Imc() { } ;
  void print(std::ostream & out ) ; 
  void ImagerText( std::ostream & out );
  void SounderText( std::ostream & out );

  Imc * Next() { return (Imc*) (&next); }
	
};


#endif // IFCOEFF_H
