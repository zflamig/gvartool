#ifndef SELFLOAT_H
#define SELFLOAT_H
//#include <stream.h>
#include <iostream>
using std::ostream;

#ifdef _LSBF_
struct SELFloat
{
  unsigned m:24;
  unsigned exp:7;
  unsigned sign:1;
};
#else
struct SELFloat{ 
  unsigned sign: 1;
  unsigned exp: 7;
  unsigned m: 24;
};
#endif

typedef struct SELFloat SELFloat; 

class SelFloat{
  union {SELFloat SEL; float IEEE; int INT; } F0; 
  double ToDouble(); 
  float ToFloat();
 public:
  SelFloat() {} ; 
  operator float(){ return ToFloat() ;} 
  operator double(){ return ToDouble(); }
  friend ostream & operator<<(ostream & out, SelFloat & s);
  void ByteSwap(void);

};



#endif // SELFLOAT_H
