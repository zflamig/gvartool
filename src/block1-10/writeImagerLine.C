#ifdef __GNUG__
#pragma implementation
#endif

#include "writeImagerLine.h" 
#include "navigation/gimloc3.h"

static unsigned short BufSpace[MAX_BLOCK_SZ ]; 
static char BufSpace2[MAX_BLOCK_SZ *sizeof(double)]; 

extern GvarBlock gvar;
extern ImagerDoc imagerDoc ; 
extern Options Opt; 
extern Radiometric* 
(*Runits)[INSTRUMENTS][CHANNELS][SIDES][DETECTORS];
extern std::ofstream DebugFile;
extern FileBuf* (*OutFile)[INSTRUMENTS];   

void GetChanDet(LineDoc *lineDoc, int &detector)
{
  int channel;
  int flip;

  channel = lineDoc->channel();

  flip = imagerDoc.Iscan.flip();

  if (flip)
  {
    // modify physical detector so that
    // lineDoc->detector() returns the correct logical detector
    // ??? guess this can only be called once per lineDoc ???
    uint16 *detector_id = &((uint16 *)lineDoc)[3];

    if (channel==I1) // visible channel
      *detector_id = 9 - (*detector_id);
    else if ((channel != I3 && lineDoc->spcid()  < 12) || 
             (channel != I6 && lineDoc->spcid() == 12) ||
             (channel != I6 && lineDoc->spcid() == 13) ||
                              (lineDoc->spcid()  > 13)    ) // multiple-detector IR channels
      *detector_id = (((*detector_id-1)<<1)|3) - *detector_id;
    // else channel 3/6: leave the physical detector alone (there's only one)

                    
                    //#define _MODIFY_LINE_NUMBERS_
                    #ifdef _MODIFY_LINE_NUMBERS_
                        if (channel==I1)
                        {
                    // modify relative imaging scan count (REL_SCAN_COUNT) to adjust for flipped-mode error
                          // assume neither lagged nor non-lagged line numbers is correct
                          // add 1 to lagged line numbers, subtract 1 from unlagged line numbers
                          uint16 *spl = &((uint16 *)lineDoc)[6]; // REL_SCAN_COUNT LSW (10-bit)
                          uint16 *sph = &((uint16 *)lineDoc)[5]; // REL_SCAN_COUNT MSW (10-bit)
                    
                          if (lineDoc->is_lagged) // detector is lagged
                          {
                            (*spl)++;
                            if ((*spl)&0xfc00)
                            {
                              (*sph)++;
                              (*spl) &= 0x03ff;
                            }
                          }
                          else
                          {
                            (*spl)--;
                            if ((*spl)&0xfc00)
                            {
                              (*sph)--;
                              (*spl) &= 0x03ff;
                            }
                          }
                        }
                    #endif
  }

  detector = lineDoc->detector();
} // end GetChanDet

void writeImagerLine(LineDoc * lineDoc){ 

  int c; // channel
  int d; // logical detector


  c = lineDoc->channel();
  GetChanDet(lineDoc, d);


  if(lineDoc->valid() ){
    int abs_scan_count = lineDoc->rel_scan_count() + (imagerDoc.abs_scan_count-imagerDoc.rel_scan_count);  

    int i = imager;

    /*double line= (8.0/VIS_LINES_PER_LINE[i][c])*abs_scan_count+d; */
    /* 8 is number of vis lines per scan */
    double line= (8.0/VIS_LINES_PER_LINE[i][c])*(abs_scan_count-2)+d+1;
    line = lineDoc->vis_line(imagerDoc) / VIS_LINES_PER_LINE[i][c] ;

    int line_in_frame = (lineDoc->vis_line(imagerDoc) - imagerDoc.N_line_of_frame) / VIS_LINES_PER_LINE[i][c] ;

    if (Opt.debug()==Dframe)
    {

      if (c==0) {
DebugFile  <<"\n line,line_in_frame\t"<<line <<"\t"<<line_in_frame ;
      }
    }


    int side            = lineDoc->side(); 
    int scaled_n_pixels = lineDoc->n_pixels(); 
    int VisFirstPixel   = imagerDoc.W_pix_of_frame;
    int scaled_first_pixel;
    void * converted  = NULL;



    for(int f=0; f<Opt.sectors(i); f++){
    converted = NULL ;

      if(  (OutFile[f][i] )              && 
           Runits[f][i][c][side][d]      && 
           Opt.wordType(f,i,c) != Undef  &&  
           skipIt(f,i,c) == FALSE           ){

	scaled_first_pixel= (int)(Opt.xscale(f,i,c) * VisFirstPixel / VIS_PIX_PER_PIX[i][c]);
	scaled_n_pixels   = (int)(Opt.xscale(f,i,c) * lineDoc->n_pixels() ); 
	
	double yscale = Opt.yscale(f,i,c); 
        double scaled_line_1, scaled_line_2;

        scaled_line_1 = yscale*line;
        scaled_line_2 = yscale*(line+1.0) - 1;

      if (c==0) {
      DebugFile  <<"\nws\t"<<scaled_line_1<<"\txoff\t"<<scaled_first_pixel<<"\tpix\t"<<scaled_n_pixels<<"\txst\t"<<OutFile[f][i]->xStart(c)<<"\tyst\t"<<OutFile[f][i]->yStart(c)<<"\txsz\t"<<OutFile[f][i]->xSize(c)<<"\tysz\t"<<OutFile[f][i]->ySize(c) ;
      }

	for(double scaled_line=scaled_line_1;  scaled_line<=scaled_line_1; scaled_line+=1.0){ 


	  if(OutFile[f][i]->lineClean(c, (int)scaled_line, scaled_first_pixel, scaled_n_pixels) ){


      if (c==0) {
	    DebugFile  <<"\t"<<"will write";
      }

	    if(!converted){
	      // converted == bufspace, both are pointers to same memory
              // scaled_n_pixels = number of x pixels to copy
              // every 1/scale pixels are copied from data to converted
	      converted = reSample(lineDoc->data(), scaled_n_pixels, Opt.xscale(f,i,c), BufSpace);
	      
	      if(Opt.map(f,i,c) ){ 
		int d0 = (unsigned)(d-(int)(0.5/yscale)*VIS_LINES_PER_LINE[i][c]) +1;

		int d1 = MIN(IDETECTORS,
			     (d + 1 + (int)(0.5/yscale)) * VIS_LINES_PER_LINE[i][c] ) ;
		
		converted = Gridburn(&imagerDoc, (unsigned short*)converted,
		                     d0, d1,
		                     VIS_PIX_PER_PIX[i][c]/Opt.xscale(f,i,c),
				     Opt.map(f,i,c));
	      }
	      
	      converted = Runits[f][i][c][side][d]->convert((unsigned short*) converted
		                                             ,scaled_n_pixels
		                                             ,BufSpace2);
	    }

		double lat, lon;
                double xstride = VIS_PIX_PER_PIX[i][c]/Opt.xscale(f,i,c);
                double ystride = VIS_LINES_PER_LINE[i][c]/Opt.yscale(f,i,c);
                unsigned char *fc = (unsigned char*)converted;
                //printf("line is %f, scaled_line is %f, line_in_frame %i\n", line, VIS_LINES_PER_LINE[i][c], VIS_LINES_PER_LINE[i][c]);
                for (int k = 0; k < scaled_n_pixels; k++) {
                	lineXpixel2latXlon((int)(scaled_line_1) * ystride, (scaled_first_pixel + k) * xstride, lat, lon);
                	if (lon == 9999.0 || lat == 9999.00) {
                		fc[k] = 0;
                	}
                }


	    OutFile[f][i]->writeData(c,
	                             (int)(scaled_line),
	                             scaled_first_pixel,
	                             converted, 
				     scaled_n_pixels);
	  }
	}
      }
    }




  }


  else gvar.trash(lineDoc->bytes()); 

}

