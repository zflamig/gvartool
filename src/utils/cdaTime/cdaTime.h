#ifndef CDATIME_H
#define CDATIME_H
/* To interpret CDA time tags. See goes i-m oge specs, pg 69.
 */

//#include <stream.h>
#include <iostream>
using std::ostream;

class CdaTime{ 
 private: 
#ifdef _LSBF_
  /* high bits are taken first */
  unsigned yr100:4;		/*w1 4 7*/ 
  unsigned yr1000:4;		/*w1 0 3*/

  unsigned yr1:4;		/*w2 4 7*/ 
  unsigned yr10:4;		/*w2 0 3*/ 

  unsigned doy10:4;		/*w3 4 7*/ 
  unsigned doy100:3;		/*w3 1 3*/ 
  unsigned flywheel:1;		/*w3 0  */ 

  unsigned hr10:4;		/*w4 4 7*/ 
  unsigned doy1:4;		/*w4 0 3*/ 

  unsigned min10:4;		/*w5 4 7*/ 
  unsigned hr1:4;		/*w5 0 3*/ 

  unsigned sec10:4;		/*w6 4 7*/ 
  unsigned min1:4;		/*w6 0 3*/ 

  unsigned msec100:4;		/*w7 4 7*/ 
  unsigned sec1:4;		/*w7 0 3*/ 

  unsigned msec1:4;		/*w8 4 7*/ 
  unsigned msec10:4;		/*w8 0 3*/ 
#else
  /* low bits are taken first */
  unsigned yr1000:4;		/*w1 0 3*/
  unsigned yr100:4;		/*w1 4 7*/ 

  unsigned yr10:4;		/*w2 0 3*/ 
  unsigned yr1:4;		/*w2 4 7*/ 

  unsigned flywheel:1;		/*w3 0  */ 
  unsigned doy100:3;		/*w3 1 3*/ 
  unsigned doy10:4;		/*w3 4 7*/ 

  unsigned doy1:4;		/*w4 0 3*/ 
  unsigned hr10:4;		/*w4 4 7*/ 

  unsigned hr1:4;		/*w5 0 3*/ 
  unsigned min10:4;		/*w5 4 7*/ 

  unsigned min1:4;		/*w6 0 3*/ 
  unsigned sec10:4;		/*w6 4 7*/ 

  unsigned sec1:4;		/*w7 0 3*/ 
  unsigned msec100:4;		/*w7 4 7*/ 

  unsigned msec10:4;		/*w8 0 3*/ 
  unsigned msec1:4;		/*w8 4 7*/ 
#endif
 public:
  CdaTime(){ };
  CdaTime(int * in);
  CdaTime(CdaTime & out);
  int msec();
  int  sec();
  int  min();
  int  hrs();
  int  day();
  int year();
  int ieeea();
  int ieeeb();
  int time_code_generator_state();
  void print( ostream & out) ; 
  void timestamp( ostream & out) ; 
  void timeStr( char * ); 

  friend ostream& operator << (ostream & out, CdaTime & t);
  long int TimDifSec(CdaTime & t2);
  
  void toIEEE(int *);
  void toMcIDAS(int * );
};


inline int CdaTime::msec()
{ return(100 * msec100 + 10 * msec10 + msec1 );}

inline int CdaTime::sec()
{ return( 10 * sec10 + sec1 );}

inline int CdaTime::min()
{ return( 10 * min10 + min1 );}

inline int CdaTime::hrs()
{ return( 10 * hr10  + hr1  );}

inline int CdaTime::day()
{ return(100 * doy100 + 10 * doy10 + doy1 );}

inline int CdaTime::year()
{ return(1000* yr1000 + 100* yr100 + 10* yr10  +  yr1 );}

inline int CdaTime::ieeea()
{ return ( (yr1000 * 1000 + yr100 * 100 + yr10 * 10 + yr1 ) * 10000
              + (doy100 * 100 + doy10 * 10 + doy1 ) * 10
             + hr10 
    );}

inline int CdaTime::ieeeb()
{ return(hr1 * 10000000  
             + (min10*10 + min1 )* 100000 
             + (sec10 * 10 + sec1)* 1000 
             + msec100 * 100 + msec10*10 + msec1
);}

inline int CdaTime::time_code_generator_state()
{ return ( flywheel); }

#endif /* CDATIME_H */


