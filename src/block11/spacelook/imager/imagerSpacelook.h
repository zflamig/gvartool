#ifndef imagerSpacelook_h
#define imagerSpacelook_h

#include "block11/sadId.h"
#include "ISblock1.h"

/*const char warningFlags[] = {
	" " , // 0
	" " , // 1
	" " , // 2
	" " , // 3
	" " , // 4
	" " , // 5
	" " , // 6
	"Filtered sample size too small " , // 7
	"Unfiltered mean value (counts) below low limit ", // 8
	"Filtered mean value (counts) below low limits " , // 9
	"Unfiltered mean value (counts) exceeds high limits ", // 10
	"Filtered mean value (counts) exceeds high limits " , // 11
	"Unfiltered sigma value (counts) exceeds  limits ", //12
	"Filtered sigma value (counts) exceeds limits ", // 13
	" ", // 0
	"Filtered IR sigma value (radiance) exceeds limits ", //15
	"Filtered IR sigma value (temperature) exceeds limits " // 16
	}
*/

class ImagerSpacelook{ 
 private:
	ISblock1 block1;
 public:
	ImagerSpacelook() { }; 
	void copy( SadId * sadId ); 
	float postClampFilteredMean(int channel, int detector); 
	void printStatistics(ostream & out, int channel); 
	void printHeader( ostream & out, int channel );
};

inline	void ImagerSpacelook::
printStatistics(ostream & out, int channel)
{ block1.printStatistics(out, channel); }

inline	void ImagerSpacelook::
printHeader(ostream & out, int channel)
{ block1.printHeader(out, channel);  }

#endif // imagerSpacelook_h
