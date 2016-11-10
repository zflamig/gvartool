#ifndef RESAMPLE_H
#define RESAMPLE_H

#include <string.h>

void * reSample(unsigned short * data, 
		int size, 
		double xscale,
		unsigned short * buf);

#endif // RESAMPLE_H
