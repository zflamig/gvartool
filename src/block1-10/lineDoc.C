//#include <stream.h>

#include "block0/imagerDoc.h"
#include "include/types.h"
#include "block1-10/lineDoc.h"
#include "block1-10/SpaceCraftIDText.h"
#include "block1-10/SourceChannelIDText.h"
#include "block0/iscan.h"

istream & LineDoc::read( istream & in){
  return(in.read( (char*) this, LINEDOC_BSZ) ); 
}

void LineDoc::print( ostream & out ){ 
  
  out << "\n\t Line Documentation Definition" ; 

  out << "\n spcid : Spacecraft ID               " << spcid() ; 
  //	out << "\n\t " << SpaceCraftIDText[spcid() ];  

  out << "\n spsid : Sensor processing system ID " << ground_system_id ; 

  out << "\n lside : Active detector config ID   " << side_id ; 

  out << "\n lidet : Detector ID                 " << detector_id ; 

  out << "\n licha : Source channel ID           " << channel_id ;
  out << "\n\t" << SourceChannelIDText[channel_id];

  out << "\n rel_scan_count : Output scan count           " << rel_scan_count() ; 

  out << "\n l1scan: Imager scan status (1)      " << l1scan() ; 

  out << "\n l2scan: Imager scan status (2)      " << l2scan() ; 

  out << "\n lpixls: Number of pixels in record  " << n_pixels() ; 

  out << "\n lwords: Number of words in record   " << n_words() ; 

  out << "\n lzcor : Pixel offset employed at    ";
  out << "\n\t western edge of scan line   " << lzcor() ; 

  out << "\n llag  : Data lag flag               " << is_lagged ; 

  out << "\n" ; 
  
}

void LineDoc::lineDocText(ostream& out ){
  out << "IdSpaceCraft\t" ;
  out << "IdSensorPr\t" ; 
  out << "side\t";
  out << "detector\t";
  out << "channel\t";
  out << "rel_scan_count\t";
  out << "scan1\t";
  out << "scan2\t";
  out << "pixls\t";
  out << "words\t";
  out << "zcor\t";
  out << "lag\t";
}

void LineDoc::printLineDoc(ostream & out ){
  out << spcid() << "\t";
  out << ground_system_id << "\t";
  out << side_id << "\t";
  out << detector_id << "\t";
  out << channel_id << "\t";
  out << rel_scan_count() << "\t";
  out << l1scan() << "\t";
  out << l2scan() << "\t";
  out << n_pixels() << "\t";
  out << n_words() << "\t";
  out << lzcor() << "\t";
  out << is_lagged  << "\t";
}

void LineDoc::printDframe(ostream & out ){
  out << "\t" << rel_scan_count() << "\t";
  out << "  " << channel_id << "    ";
  out << is_lagged  << "\t";
  out << n_pixels() << "\t";
  out << detector_id << "\t";
  out << detector() << "\t";
  out << rel_vis_line() << "\t";

}

int ExChan[3][2][4] =
{ // expected channel for [version][block][index]
  {{4,4,5,5}, // GVAR version 0 (pre-G12)
   {2,2,3,0}},
  {{2,2,3,3}, // GVAR version 1 (G12-G13)
   {4,4,6,0}},
  {{2,2,3,3}, // GVAR version 2 (G14-G15)
   {4,4,6,6}}
};
                                                                                
int LineDoc::expectedChannel(int version, int block, int index)
{ // version may be defind as pre-G12, G12-G13, and G14-G15
  // this may also correspend to byte 8 of block 0
  int retval = 0;
  if (version<0 || version>2 || block<0 || block>10 || index<0 || index>3)
    retval = 0;
  else if (block>2 && block<11)
  {
    retval = 1;
  }
  else if (block>0 && block<3)
    retval = ExChan[version][block-1][index];
  return retval;
} // end LineDoc::expectedChannel

int LineDoc::verify(ImagerDoc &b0, int block, int linendx)
{
  int retval = 0;
  int version = 0;
  int expectedDetector;
  /* int bit, mask, x; */

  if (b0.spcid() == 12 || b0.spcid() == 13)
    version = 1;

  if (b0.spcid() >= 14 )
    version = 2;

  if (version<0 || version>2 || block<0 || block>10 || linendx<0 || linendx>3)
    return retval;

  if ((int)b0.spcid()!=this->spcid())
  { // Spacecraft mismatch!
    retval++;
  }

  //if ((int)b0.ground_system_id != this->ground_system_id )
  //{ // sensor processing system mismatch!
  //}

  if ( b0.Iscan.bitSet(13) != (this->side_id&1)  )
  { // active detector side mismatch!
    retval++;
  }

  expectedDetector = XLDet[version][block-1][b0.Iscan.flip()][linendx];
  if (this->detector_id != expectedDetector)
  { // unexpected detector
    retval++;
  }

  if (this->channel_id != expectedChannel(version, block, linendx))
  { // unexpected channel
    retval++;
  }

  //if (b0.rel_scan_count+x! = this->rel_scan_count())
  //{ // unexpected relative scan count
  //  retval++;
  //}

  // L1scan
  //mask = 1<<4;
  //for (x=2; x<=7; x++)
  //{
  //  bit = this->L1scan&mask;
  //  if (b0.Iscan.bitSet(x)!=(bit!=0))
  //    retval++;
  //  mask<<=1;
 // }

  //L2scan
  //mask = 1<<2;
  //for (x=8; x<=15; x++)
 // {
 //   bit = this->L1scan&mask;
 //   if (b0.Iscan.bitSet(x)!=(bit!=0))
 //     retval++;
 //   mask<<=1;
 // }

  //Lpixls
  //Lwords
  return retval;
} // end LineDoc::verify




float LineDoc::vis_line(ImagerDoc &b0)
{
  float vis_line = 0;

  vis_line = rel_vis_line() + b0.N_Line_In_Scan;

  return vis_line;
} 


float LineDoc::rel_vis_line()
{
  float rel_vis_line = 0;

  rel_vis_line= VIS_LINES_PER_LINE[imager][channel()] * detector()  + VIS_LINES_PER_LINE[imager][channel()]/2.  - 0.5 ;

  return rel_vis_line;
} 





