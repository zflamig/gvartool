#include "include/gvar.h"
#include "iscan.h"
#include "utils/bitfield/bitfield.h"

const char * IscanText[] = {
	" 0 Frame Start",
	" 1 Frame End",
	" 2 Frame break ( lines lost )",
	" 3 Line break ( Pixels lost )",
	" 4 Priority 1 frame data ",
	" 5 Priority 2 frame data ",
	" 6 East-to-west scan",
	" 7 South-to-north scan",
	" 8 IMC active",
	" 9 Lost header block",
	"10 Lost trailer block",
	"11 Lost telemetry data",
	"12 (Star sense) time break",
	"13 Side 2 (secondary) active",
	"14 Visible normalization active",
	"15 IR calibration active",
	"16 yaw-flipped mode (GOES-10)",
	"17 IR detector 1 data not valid",
	"18 IR detector 2 data not valid",
	"19 IR detector 3 data not valid",
	"20 IR detector 4 data not valid",
	"21 IR detector 5 data not valid",
	"22 IR detector 6 data not valid",
	"23 IR detector 7 data not valid",
	"24 Visible detector 1 data not valid",
	"25 Visible detector 2 data not valid",
	"26 Visible detector 3 data not valid",
	"27 Visible detector 4 data not valid",
	"28 Visible detector 5 data not valid",
	"29 Visible detector 6 data not valid",
	"30 Visible detector 7 data not valid",
	"31 Visible detector 8 data not valid"
};

int ISCAN::bitSet(int bit)
{
  Bitfield bits(iscan);
  return(bits.status(bit));
}

int ISCAN::newFrame(){ 
  Bitfield bits(iscan);
  return( bits.status(0) ); 
}

int ISCAN::Frame_start(){ 
  Bitfield bits(iscan);
  return( bits.status(0) ); 
}

int ISCAN::Frame_end(){ 
  Bitfield bits(iscan);
  return( bits.status(1) ); 
}

int ISCAN::prior1()
{
  Bitfield bits(iscan);
  return(bits.status(4));
}

int ISCAN::prior2()
{
  Bitfield bits(iscan);
  return(bits.status(5));
}

//prior1   prior2   priority 
//   0       0       0
//   0       1       1
//   1       0       2
//   1       1       3
int ISCAN::priority()
{
  return ((prior1()>0)<<1)|(prior2()>0);
}

int ISCAN::imcStatus() { 
  Bitfield bits(iscan);
  return(bits.status(8)) ;
}

int ISCAN::flip()
{
  Bitfield bits(iscan);
  return(bits.status(16));
}

ostream & ISCAN::print(ostream & out ){ 
  if (iscan != 0 ) {  
    Bitfield bits(iscan);
    for ( int i = 0; i < 32; i++)
      if ( bits.status(i) ) 
	out << IscanText[i] << "\n"; 
  }
  return(out);
}

ISCAN::ISCAN(unsigned int in ) 
 {  iscan = (in << 16)  ; }



  






