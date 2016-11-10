#include "skipIt.h"

int skipIt(int sector, int instrument, int channel) {

  extern Options Opt;
  extern ImagerDoc imagerDoc;
  extern SounderDoc sounderDoc;

  int skip = TRUE;

  int i0,i1, s0, s1, c0, c1;

  int xstart=0, ystart=0, xend=0, yend=0, curr_hr=0, visCounts=0;


  if(instrument == ALL) { i0 = 0;          i1 = sounder ;    }
  else                  { i0 = instrument; i1 = instrument ; }
  

  for (int i=i0; i<=i1 && skip; i++){

    if (sector==ALL) { s0 = 0;      s1 = Opt.sectors(i); }
    else             { s0 = sector; s1 = sector+1;       }

    if (channel==ALL) { c0 = 0;       c1 = N_Channels[i]; }
    else              { c0 = channel; c1 = channel+1;  }    
    

    for(int s=s0; s<s1 && skip ; s++){
      for(int c=c0; c<c1 && skip; c++){


	if (i==imager) {
	  xstart=imagerDoc.W_pix_of_frame;
	  ystart=imagerDoc.N_line_of_frame;
	  xend  =imagerDoc.E_pix_of_frame;
	  yend  =imagerDoc.S_line_of_frame;
	  curr_hr = imagerDoc.T_sps_current.hrs();
	  visCounts = IDETECTORS * imagerDoc.abs_scan_count;
	}
	else if (i==sounder){
	  xstart=sounderDoc.W_pix_of_frame;
	  ystart=sounderDoc.N_line_of_frame;
	  xend  =sounderDoc.E_pix_of_frame;
	  yend  =sounderDoc.S_line_of_frame;
	  curr_hr = sounderDoc.T_sps_current.hrs();    
	  visCounts = SDETECTORS * sounderDoc.abs_scan_count;
	}
	
	if(
	   (VALID(Opt.xstart(s,i,c),xstart) < xend      ) && 
	   (VALID(Opt.ystart(s,i,c),ystart) < yend      ) &&
	   (VALID(Opt.xend(s,i,c), xend)    >= xstart   ) &&
	   (VALID(Opt.yend(s,i,c), yend)    >= ystart   ) &&
	   (VALID(Opt.yend(s,i,c), yend)    > visCounts ) &&
	   (Opt.wordType(s,i,c) != Undef )
	   ) skip = FALSE;

	
	if(skip == FALSE && Opt.area(s,i,c)) {

	  float Area = (xend-xstart)*(yend-ystart)/ 1.0E6;
	  if(Opt.area(s,i,c) < 0 ) 
	    { if(Area > -1.0* Opt.area(s,i,c) ) skip = TRUE; }
	  else if ( Area < Opt.area(s,i,c) ) skip = TRUE;

	}
	
	if(skip==FALSE){
	  int h0 = Opt.gmtHoursStart(s,i,c);
	  int h1 = Opt.gmtHoursStop(s,i,c);

	  if      (h1 > h0) {if ((curr_hr > h1 || curr_hr < h0 )) skip = TRUE; }
	  else if (h1 < h0) {if ((curr_hr > h1 && curr_hr < h0 )) skip = TRUE; }

	}



      } 
    }
  }


  return(skip);

}




