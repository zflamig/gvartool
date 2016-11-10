#ifndef ATTITUDEANGLE_H
#define ATTITUDEANGLE_H

//#include <stream.h>
#include <iostream>
using std::ostream;

#include "include/types.h"
#include "utils/selFloat/selFloat.h"

class Polar{ 
 public:
  SelFloat Mg;			/* 539  542*/	
  SelFloat Ph ;			/* 543  546*/	
 public: 
  Polar();
  friend ostream& operator << (ostream & out, Polar & p);
  void ByteSwap(void);
};

class MonomialSinusoid{ 
 public:
  uint32   Order_of_applicable;	/* 663  666*/	
  uint32   Order_of;		/* 667  670 */	
  Polar    Sinusoid;		/* 671  678 */
  SelFloat Angle_of_epoch_where_monomial_is_zero; /* 679  682*/	
 public:
  MonomialSinusoid();
  friend ostream& operator << (ostream & out, MonomialSinusoid & p);
  void toNoaaNavigation(double * in);
  void toMcIDASNavigation(sint32 *in);
  void ByteSwap(void);
};

class AttitudeAngle{ 
 public:
  SelFloat         Exponential_magnitude; /* 523  526*/ 
  SelFloat         Exponential_time_constant; /* 527  530*/ 
  SelFloat         Constant_mean_attitude_angle; /* 531  534*/ 
  uint32           Number_of_sinusoidals_per_angle; /* 535  538*/ 
  Polar            Sinusoid[15];		/* 539  658*/ 
  uint32           Number_of_monomial_sinusoids; /* 659  662*/ 
  MonomialSinusoid Monomial[4]; /* 663  742*/
 public:
  AttitudeAngle();
  friend ostream& operator << (ostream & out, AttitudeAngle & p);
  void toNoaaNavigation(double * in);
  void toMcIDASNavigation(sint32 * in);
  void ByteSwap(void);
};

#endif // ATTITUDEANGLE_H













