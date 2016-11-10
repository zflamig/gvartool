#include "include/gvar.h"
#include "grid.h"

void * Gridburn(ImagerDoc * imagerDoc,
		 unsigned short * converted,
		 int d0, int d1,
		 double scale , 
		GRIDSET Map ){
  
  int gpnts, i; 
  
  switch (Map){ 
  case grid0:
    break;

  case grid12:

  case grid1:{
    gpnts = MIN(imagerDoc->G1cnt, 512);
    if(gpnts>0){
      for (i = 0; i < gpnts ; i++)
	if (imagerDoc->Grid1Detector[i] >= d0 &&
	    imagerDoc->Grid1Detector[i] <= d1 &&
	    imagerDoc->Grid1Pixel[i]/scale  < MAX_BLOCK_SZ )
	  converted[(int)(imagerDoc->Grid1Pixel[i]/scale)] = 1024; 
    }
    if(Map==grid1) break;
  }
    
  case grid2:{ 
    gpnts = MIN(imagerDoc->G2cnt, 512);
    if(gpnts>0){
      for (i = 0; i < gpnts ; i++)
	if (imagerDoc->Grid2Detector[i] >= d0  &&
	    imagerDoc->Grid2Detector[i] <= d1  &&
	    imagerDoc->Grid2Pixel[i]/scale < MAX_BLOCK_SZ )
	  converted[(int)(imagerDoc->Grid2Pixel[i]/scale)] = 1024; 
    }
    break;
  }

  default:
    break;
  }
  return(converted);
}


