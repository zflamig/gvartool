#ifndef GRID_H
#define GRID_H

#include "block0/imagerDoc.h"

void * Gridburn(ImagerDoc * imagerDoc,
		unsigned short * converted,
		int d0, int d1 ,
		double scale =1, 
		GRIDSET Map = grid0 ); 

#endif
