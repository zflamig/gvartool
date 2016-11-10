#include "imagerSpacelook.h"

float ImagerSpacelook::postClampFilteredMean
(int channel, int detector ){ 
	const int D0[] = {7, 4, 6, 0, 2 };
	return( block1.PostClampFilteredMean[D0[channel]+detector]); 
}
	



void ImagerSpacelook::copy( SadId * sad ){ 
	
	switch( sad->blockCount() ){ 
		
	case 1:{ 
		block1.copy(sad) ;
		break;
	}
		
	case 2: 
	case 3:
	case 4:
	case 5:
	case 6:

	default:
		break;
	}
}



		 
