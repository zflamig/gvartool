#include "include/gvar.h"
#include "sscan.h"
#include "utils/bitfield/bitfield.h"

const char * SscanText[] = {
	" 0 Frame Start",
	" 1 Frame End",
	" 2 Frame break ( lines lost )",
	" 3 Pixel's lost ",
	" 4 Priority 1 frame data ",
	" 5 Priority 2 frame data ",
	" 6 East-to-west scan",
	" 7 South-to-north scan",
	" 8 IMC active",
	" 9 dwell mode = 4",
	"10 dwell mode = 2",
	"11 dwell mode = 1",
	"12 N/S step mode = double",
	"13 Side 2 (secondary) active",
	"14 Visible normalization active",
	"15 IR calibration active"
};

int SSCAN::bitSet(int bit)
{
  Bitfield bits(sscan);
  return(bits.status(bit));
}

int SSCAN::newFrame(){ 
  Bitfield bits(sscan);
  return( bits.status(0) ); 
}

int SSCAN::prior1()
{
  Bitfield bits(sscan);
  return(bits.status(4));
}

int SSCAN::prior2()
{
  Bitfield bits(sscan);
  return(bits.status(5));
}

int SSCAN::priority()
{
  return ((prior1()>0)<<1)|(prior2()>0);
}

int SSCAN::imcStatus() { 
  Bitfield bits(sscan);
  return(bits.status(8)) ;
}

ostream & SSCAN::print(ostream & out ){ 
  if (sscan != 0 ) {  
    Bitfield bits(sscan);
    for ( int i = 0; i < 16; i++)
      if ( bits.status(i) ) 
	out << SscanText[i] << "\n"; 
  }
  return(out);
}

SSCAN::SSCAN(unsigned int in ) 
 {  sscan = (in << 16)  ; }

