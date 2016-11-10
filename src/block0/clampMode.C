#include "clampMode.h"
#include "utils/bitfield/bitfield.h"
//#include <stream.h>
#include <iostream>
using std::ostream;


void ClampMode::ByteSwap(void)
{
  unsigned int ui = (unsigned int)clamp;
  ui = ((ui&0xff000000)>>24)|
       ((ui&0x00ff0000)>>8)|
       ((ui&0x0000ff00)<<8)|
       ((ui&0x000000ff)<<24);
  clamp = (int)ui;
}
/*
static char * ClampModeText[]={
	"  36.6 second space clamp mode active",
	"   9.2 second space clamp mode active",
	"   Scan clamp mode active",
	" ",
	" ",
	" ",
	" ",
	" ",
	" 0 ",
	" 1 Detector 4/1 invalid cal condition (no statistics )",
	" 2 Detector 4/2 invalid cal condition (no statistics )",
	" 3 Detector 5/1 invalid cal condition (no statistics )",
	" 4 Detector 5/2 invalid cal condition (no statisitics)",
	" 5 Detector 2/1 invalid cal condition (no statisitics)",
	" 6 Detector 2/2 invalid cal condition (no statisitics)",
	" 7 Detector 3/1 invalid cal condition (no statisitics)",
	" 8 ",
	" 9 Detector 4/1 excessive drift alarm rate",
	"10 Detector 4/2 excessive drift alarm rate",
	"11 Detector 5/1 excessive drift alarm rate",
	"12 Detector 5/2 excessive drift alarm rate",
	"13 Detector 2/1 excessive drift alarm rate",
	"14 Detector 2/2 excessive drift alarm rate",
	"15 Detector 3/1 excessive drift alarm rate",
	"16 ",
	"17 ",
	"18 ",
	"19 ",
	"20 ",
	"21 ",
	"22 Excessive interpolation interval ",
	"23 Atmospheric exclusion zone activated"
};
*/
/*
int ClampMode:: check() { 
	if (!clamp ) return( 0);
	Bitfield bits(clamp) ;
	for ( int i = 0 ; i < 32 ; i++)
			if ( bits.status(i)) return(i);
	cout << "Clamp mode and status flags:" <<  ClampModeText[i];
}
*/
