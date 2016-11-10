#include "reSample.h"
#include "include/gvar.h"

void * reSample(unsigned short * data,
		int size,
		double xscale,
		unsigned short * buf){

  unsigned short * tmp = buf;
  if (xscale != 1 ) { 
    double scale = 1.0/xscale; 
    for(int i=0; i<size;  i++)
      *(tmp++) = *(data+(int)(i*scale)) ;
  }
  else { memcpy(buf, data, size*sizeof(short) );}
  
  return(buf);
}


