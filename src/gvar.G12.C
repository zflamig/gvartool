#define _MAIN_
/*
   GOES data processing software
   Disclaimer This software is provided AS IS.
   Neither the author or NASA is responsible for any
   damage incurred directly or indirectly
   through the use of this software.

   Om Sharma, RDC, MAR 15 1995
   */

//SYS includes
#include<stdio.h>
//#include<stream.h>
//#include<strstream.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<time.h>
#include<string.h>
#include<sys/time.h>
#include<sys/resource.h>

//GVAR includes
#include "include/types.h"
#include "include/gvar.h"

#include "gvarBlock/gvarBlock.h"

#include "block0/imagerDoc.h"

#include "block1-10/lineDoc.h"
#include "block1-10/writeImagerLine.h"
#include "block1-10/list.h"

#include "block11/sadId.h"

#include "block11/doc/sounderDoc.h"
#include "block11/data/sounderData.h"

#include "block11/imc/imc.h"
#include "block11/spacelook/imager/imagerSpacelook.h"

#include "utils/radiometric/radiometric.h"
#include "utils/radiometric/setRadiometric.h"

#include "utils/fileBuf/genericFile.h"

#include "utils/unpack/unpack10.h"
#include "utils/reSample/reSample.h"

#include "utils/counter/counter.h"
#include "utils/options/options.h"
#include "utils/crcCheck/crcCheck.h"
#include "utils/string/stringUtils.h"

#include "navigation/imagerNav.h"
#include "navigation/gimloc3.h"

#include "block1-10/skipIt.h"
#include "utils/openFiles/openFiles.h"

//Declare global variables

extern const char *InstString[];

//block of gvar data
GvarBlock gvar;

//block 0 data
ImagerDoc imagerDoc;
ImagerDoc prev_imagerDoc;
ImagerDoc good_imagerDoc;

//nav info derived from imagerdoc
ImagerNav imagerNav;

// nav structures for current imager and sounder
nav_constants i_nav;
nav_constants s_nav;

//block11.35
SounderDoc sounderDoc;

//Radiometric constants for each frame
Radiometric* (*Runits)[INSTRUMENTS][CHANNELS][SIDES][DETECTORS] = 0;

//Output files for each frame
FileBuf* (*OutFile)[INSTRUMENTS];
//user requested options
Options Opt;

//block 1-10 data
LineDoc* lineDoc=NULL;

//block11 header
SadId* sad=NULL;

//Debug information file
std::ofstream DebugFile;
std::ofstream LogFile;

//pointer for raw gvar file
char* RawFile;

// priority frame stack
#include "include/FrameList.h"
FrameList *FList = 0;

int Initialized = 0;

int current_spacecraft_id = -1;

//keep track of when to begin new frame
int imager_frame_end     =  0;
int imager_frame_number  = -1;
int sounder_frame_number = -1;

int thisBlockBadCrc   = 0;

//buffer for 10 bit unpacked data
uint16* block_16bit_data=new uint16[MAX_BLOCK_SZ];



void process_sounder_doc() {

    FList->Save(sounder, sounderDoc.frame_number,
                Opt.priority()?sounderDoc.Sscan.priority():0);

    sounderDoc.copy(gvar.data() );
    if (Opt.debug() == Ddump_all) {
       sounderDoc.Ddump_all(DebugFile);
    }

    if (sounderDoc.frame_number != sounder_frame_number ){
        sounder_frame_number=sounderDoc.frame_number;

        //for(f=0; f<Opt.sectors(sounder); f++)
        ////for(c=0; c<N_Channels[sounder]; c++)
        //{
        //  delete OutFile[f][sounder];
        //  OutFile[f][sounder]=NULL;
        //}

        for (int f=0; f<Opt.sectors(sounder); f++)
            setRadiometric(Runits[f][sounder],sounderDoc);

        //print_s_Radiometric(Runits[0][sounder]);

        if (FList->Check(sounder, sounderDoc.frame_number,
                         Opt.priority()?sounderDoc.Sscan.priority():0))
            openFiles(sounderDoc);
    }

}

void process_sounder_data() {

   SounderData* sData=NULL;       //sounder data block
   sData=(SounderData *) (sad->data());
   for (int f=0; f<Opt.sectors(sounder); f++)
   {
       for (int c=0; c<SOUNDER_CHANNELS; c++)
       {
           if (OutFile[f][sounder] && Opt.wordType(f,sounder,c)!=Undef)
           {
               for (int detector=0; detector<SOUNDER_DETECTORS; detector++)
               {
                   uint16 line = sData->lineNo(detector);
                   for (int pix=0; pix<SOUNDER_PIXELS; pix++)
                   {
                       uint16 pixels = sData->pixelNo(pix);
                       for (float wpix  =  pixels   *Opt.xscale(f,sounder,c);
                                  wpix  < (pixels+1)*Opt.xscale(f,sounder,c);
                                  wpix+=1)
                       {
                           for (double wline  = (line  )*Opt.yscale(f,sounder,c);
                                       wline  < (line+1)*Opt.yscale(f,sounder,c);
                                       wline+=1)
                           {
                               OutFile[f][sounder]->writeData(c,(int)wline,(int)wpix,
                                                              (Runits[f][sounder][c][0][detector])->convert
                                                              (sData->data(c,detector, pix)));
                           }
                       }
                   }
               }
           }
       }
   }
}

void process_imager_spacelook() {

   ImagerSpacelook imagerSpacelook;

   imagerSpacelook.copy(sad);

   for (int f=0; f<Opt.sectors(imager); f++)
       if (Opt.units(f,imager,I1) == Albedo2)
           resetVisibleRadiometricConstants(Runits[f][imager], imagerSpacelook);


   static std::ofstream * ImagerSpacelookStatsFile[ICHANNELS];
   if (!ImagerSpacelookStatsFile[0] ) {
       for (int c=0; c<ICHANNELS; c++){

           char cfileName[FILENAME_SIZE];
           std::stringstream fileName;
           fileName<<"SpacelookStatsImagerCh.";
           fileName.width(2);
           fileName.fill('0');
           fileName<<c+1;
           fileName<<std::ends;
           fileName.rdbuf()->sgetn(cfileName, FILENAME_SIZE);

           if (!ImagerSpacelookStatsFile[c])
               ImagerSpacelookStatsFile[c]=new std::ofstream;

           ImagerSpacelookStatsFile[c]->open(cfileName);

           imagerSpacelook.printHeader(*ImagerSpacelookStatsFile[c],c);
       }
   }

   for (int c=0; c<ICHANNELS; c++)
       imagerSpacelook.printStatistics(*ImagerSpacelookStatsFile[c],c);
       

}

void process_imager_icse() {

    Imc * imc=NULL;

    static std::ofstream * ImagerIcseFile=NULL;
    if (!ImagerIcseFile){
        ImagerIcseFile=new std::ofstream;
        ImagerIcseFile->open("ImagerIcseFile");
        imc->ImagerText(*ImagerIcseFile);
    }

    imc=(Imc*) sad->data();
    for (int i=1; i<sad->nrec(); i++){
        imc->print(*ImagerIcseFile);
        if (imc->Next()) imc=imc->Next();
    }
    ImagerIcseFile->flush();
}

void process_sounder_icse() {

   Imc * imc=NULL;
   static std::ofstream * SounderIcseFile=NULL;

   if (!SounderIcseFile){
       SounderIcseFile=new std::ofstream;
       SounderIcseFile->open("SounderIcseFile");
       imc->SounderText(*SounderIcseFile);
   }

   imc=(Imc*) sad->data();
   for (int i=1; i<sad->nrec(); i++){
       imc->print(*SounderIcseFile);
       if (imc->Next()) imc=imc->Next();
   }
   SounderIcseFile->flush();
}

void process_imager_visible_data() {


    int skipI1=skipIt(ALL,imager,I1);

    if (Opt.debug() != Dlinedoc && Opt.debug() != Dframe)   {
       if (skipI1) return;
   	}

    //int VisDetector=gvar.blockId() - 3;
    int words=gvar.wordCount();
    unpack10(gvar.data(), words , block_16bit_data);

    lineDoc=((LineDoc*) block_16bit_data);

    if (Opt.debug() == Dlinedoc) {
       DebugFile<<"\n linedoc   ";
       lineDoc->printLineDoc(DebugFile); 
   	}
    if (Opt.debug() == Dframe) {
        lineDoc->printDframe(DebugFile); 
   	}

    if (Opt.debug() == Dlinedoc || Opt.debug() == Dframe)   {
       if (skipI1) return;
   	}

    if ( !lineDoc->verify(imagerDoc, gvar.blockId(), 0) == 0)
    {
        std::cout << "block id = " << gvar.blockId() << ", count = " << gvar.blockCount();
        std::cout << ": Invalid Visible line data channel, detector, n words, rel scan count = " ;
        std::cout << ", " << lineDoc->channel() << ", " << lineDoc->detector();
        std::cout << ", " << lineDoc->n_words() << ", " << lineDoc->rel_scan_count() << " skipping\n" << std::flush;
        return;
    }
    if (lineDoc->rel_scan_count() != imagerDoc.rel_scan_count)  imagerDoc.reset();

    words-=lineDoc->n_words();
    if (words<0) return;

    writeImagerLine(lineDoc);
}


void process_imager_ir_data() {

            //For       GOES I-L (8-11)     GOES M-N (12,13)    GOES O and beyond (14+) 
            //Block 1:  IR 4, Detector 1    IR 2, Detector 1    IR 2, Detector 1 
            //          IR 4, Detector 2    IR 2, Detector 2    IR 2, Detector 2 
            //          IR 5, Detector 1    IR 3, Detector 1    IR 3, Detector 1 
            //          IR 5, Detector 2    IR 3, Detector 2    IR 3, Detector 2 
            //
            //Block 2:  IR 2, Detector 1    IR 4, Detector 1    IR 4, Detector 1 
            //          IR 2, Detector 2    IR 4, Detector 2    IR 4, Detector 2 
            //          IR 3, Detector 1    IR 6, Detector 1    IR 6, Detector 1 
            //                                                  IR 6, Detector 2 
            //

    int line_in_block   = 0;
    int remaining_words, skip;

    // convert the block's 10-bit words to 16-bit words
    unpack10(gvar.data(), gvar.wordCount(), block_16bit_data);


    // set remaining_words to words remaining after this lineDoc is used
    // last 2 words are block CRC? in reality each line header
    // is 16bytes, so need at least that for another line
    remaining_words = gvar.wordCount();

    while (remaining_words > 2) 
    {

       if (line_in_block == 0) {
          lineDoc=(LineDoc *) block_16bit_data;
       } else {
          lineDoc = lineDoc->next();
       }

       if (Opt.debug() == Dlinedoc) {
               DebugFile<<"\n linedoc   ";
               lineDoc->printLineDoc(DebugFile); 
        }
       if (Opt.debug() == Dframe) {
	 //if (c != 0 || d != 0) DebugFile<<"\n                                    ";
               lineDoc->printDframe(DebugFile); 
       }

       if ( !lineDoc->verify(imagerDoc, gvar.blockId(), line_in_block) == 0)
       {
           std::cout << "block id = " << gvar.blockId() << ", count = " << gvar.blockCount();
           std::cout << ": Invalid IR line data line in block,channel, detector, n words, rel scan count = " << line_in_block;
           std::cout << ", " << lineDoc->channel() << ", " << lineDoc->detector();
           std::cout << ", " << lineDoc->n_words() << ", " << lineDoc->rel_scan_count() << " skipping\n" << std::flush;
       } else {


         // if the line doc relative scan count does not match the imager scan doc
         // relative scane count ....
         if (lineDoc->rel_scan_count() != imagerDoc.rel_scan_count)
             imagerDoc.reset(); // grid reset???

            // line seems valid, write it out if channel was requested
            skip = skipIt(ALL, imager, lineDoc->channel());
            if (!skip) {
                writeImagerLine(lineDoc);
            }

       }


       line_in_block++;
       // reduce word count by the number of words in this detector record
       if (lineDoc->n_words() > 0)
       {
           remaining_words -= lineDoc->n_words();
       } else {
	 remaining_words = 0;
       }

        
    }
}



void process_imager_block0() {

    std::stringstream name_1 ;

    char navinfo_name[FILENAME_SIZE];
    char IMCName[FILENAME_SIZE];
    char IdatName[FILENAME_SIZE];

    std::ofstream navinfo_file;
    std::ofstream IimcFile;
    std::ofstream IdatFile;

    int f,c;

                // save the previous scan's frame number
                FList->Save(imager, imagerDoc.frame_number,
                            Opt.priority()?imagerDoc.Iscan.priority():0);

	        // save the previous block0 
		prev_imagerDoc.duplicate( imagerDoc.addr() ) ;

	        // copy the new block0 into imagerDoc
	        imagerDoc.copy(gvar.data());


                // if the crc is bad do parity checks on the individual sections ...
                if (thisBlockBadCrc) {
                   if (  imagerDoc.check_copy(gvar.data()) != TRUE) {

		      // section1 failed parity check ...
                      // here is where we could try to correct bad values based on the prvious good imagerdoc
                      std::cout << "*** block0 failed crc and section1 parity check,  previous,current  scan count  " << prev_imagerDoc.rel_scan_count  << " ";
                      std::cout << imagerDoc.rel_scan_count << " *****\n" << std::flush;
 
                      if ( Opt.debug() == Dcrc) return;
		   }
		}





                if ( gvar.versionNumber() == 0) {
		  if ( imagerDoc.Iscan.prior1() != 0 ||  imagerDoc.Iscan.prior2() != 0 ) {

                  } 
                  else {
                  }
                }


                //Debug info
                if (Opt.debug() == Dblock0time) imagerDoc.block0time(DebugFile);
                if (Opt.debug() == Dframe     ) imagerDoc.Dframe(DebugFile);
                if (Opt.debug() == Ddump_all  ) imagerDoc.Ddump_all(DebugFile);


                //Is this a new frame, 3 possible checks ....
                // 1 - frame numbers are different
                // 2 - this imager doc has new frame flag set
		// 3 - the previous imager doc had frame end flag set
                if (imagerDoc.frame_number != imager_frame_number || imagerDoc.Iscan.Frame_start() == 1 || imager_frame_end == 1)
                {
                    imager_frame_number = imagerDoc.frame_number;

                    // make the nav files
                    if (Opt.imagerNav())
                    {
                        name_1 << Opt.navdir();
                        imagerDoc.T_sps_current.timestamp( name_1 );
                        name_1 << "_G" << (int)(imagerDoc.spcid()) << "_INPUT.DAT" << std::ends;

                        name_1.rdbuf()->sgetn(IdatName, FILENAME_SIZE);
                        name_1.str("");

                        name_1 << Opt.navdir();
                        imagerDoc.T_sps_current.timestamp( name_1 );
                        name_1 << "_G" << (int)(imagerDoc.spcid()) << "_nav_info.txt" << std::ends;

                        name_1.rdbuf()->sgetn(navinfo_name, FILENAME_SIZE);
                        name_1.str("");

                        name_1 << Opt.navdir();
                        imagerDoc.T_sps_current.timestamp( name_1 );
                        name_1 << "_G" << (int)(imagerDoc.spcid()) << "_IMCSET.DAT" << std::ends;

                        name_1.rdbuf()->sgetn(IMCName, FILENAME_SIZE);
                        name_1.str("");

                        std::cout << "making nav files :\n   " << IdatName << "\n   " << navinfo_name;
                        std::cout << "\n   " << IMCName << "\n\n" << std::flush;

                        IdatFile.open(IdatName);
                        navinfo_file.open(navinfo_name);
                        IimcFile.open(IMCName);

                        imagerDoc.NavigationFiles(IdatFile, IimcFile);
                        imagerDoc.NavigationParameters(navinfo_file);

                        IdatFile.close();
                        IdatFile.clear();
                        navinfo_file.close();
                        navinfo_file.clear();
                        IimcFile.close();
                        IimcFile.clear();
                    }

                    //Do we really need this block
                    if (Opt.sectors(imager) == 0) return;

                    // Should this only be done once???
                    // it is not dependent on new imagerDoc???
                    // User supplied lat lon converted to line pixel
                    for (f=0; f<Opt.sectors(imager); f++)
                    {
                        for (c=0; c<N_Channels[imager]; c++)
                        {
			  double cLine, cPixel, sline, spixel, eline, epixel;


                            if (Opt.cLongitude(f,imager) != 9999)
                            {
                                latXlon2lineXpixel(Opt.cLatitude(f,imager),
                                                   Opt.cLongitude(f,imager),
                                                   cLine,
                                                   cPixel);

                                sline  = cLine  - Opt.ysize(f,imager,c)/2;
                                spixel = cPixel - Opt.xsize(f,imager,c)/2;

                                eline  = sline  + Opt.ysize(f,imager,c) - 1;
                                epixel = spixel + Opt.xsize(f,imager,c) - 1;

                                Opt.xstart(f,imager,c) = (int)(spixel);
                                Opt.ystart(f,imager,c) = (int)(sline);
                                Opt.xend(f,imager,c) = (int)(epixel);
                                Opt.yend(f,imager,c) = (int)(eline);
                            }
                            else
                            { // make sure xsize, ysize are set
                                Opt.xsize(f,imager,c) = Opt.xend(f,imager,c)-
                                                        Opt.xstart(f,imager,c)+1;
                                Opt.ysize(f,imager,c) = Opt.yend(f,imager,c)-
                                                        Opt.ystart(f,imager,c)+1;
                            }

			    std::cout << " sector, channel, pixel align status " << f << ", " <<  c << ", " << Opt.alignpixel(f,imager,c) << " \n" <<  std::flush;

			    // align top left corner, because of size and rescaling options, bottom left may have excess pixels in vis
                            if (Opt.alignpixel(f,imager,c))
                            {

			       std::cout << "aligning x, before " << Opt.xstart(f,imager,c) << " " <<  Opt.xend(f,imager,c);
			       std::cout << " " <<  Opt.xsize(f,imager,c) << " \n" <<  std::flush;

                                spixel =  Opt.xstart(f,imager,c) + 200; //imagerDoc.pix_to_aligned_pix(Opt.xstart(f,imager,c)); 
                                //epixel = imagerDoc.pix_to_aligned_pix(Opt.xend(f,imager,c)) + 3; 
                                epixel = spixel + Opt.xsize(f,imager,c)   - 1;

                                Opt.xstart(f,imager,c) = (int)(spixel);
                                Opt.xend(f,imager,c)   = (int)(epixel);
                                Opt.xsize(f,imager,c)  = (int)(epixel-spixel+1);
				 std::cout << "aligning x, after " << Opt.xstart(f,imager,c) << " " <<  Opt.xend(f,imager,c);
				 std::cout << " " <<  Opt.xsize(f,imager,c) << " \n" <<  std::flush;

                            }

                            if (Opt.alignline(f,imager,c))
                            {

			      //std::cout << "aligning y, before " << Opt.ystart(f,imager,c) << " " <<  Opt.yend(f,imager,c);
			      //std::cout << " " <<  Opt.ysize(f,imager,c) << " " << c << " \n" <<  std::flush;

                                sline  = imagerDoc.line_to_aligned_line(Opt.ystart(f,imager,c)); 
                                //eline  = imagerDoc.line_to_aligned_line(Opt.yend(f,imager,c)) + 7; 
                                eline  = sline + Opt.ysize(f,imager,c)   - 1;

                                Opt.ystart(f,imager,c) = (int)(sline);
                                Opt.yend(f,imager,c)   = (int)(eline);
                                //Opt.ysize(f,imager,c)  = (int)(eline-sline+1);

				//std::cout << "aligning y, before " << Opt.ystart(f,imager,c) << " " <<  Opt.yend(f,imager,c);
				//std::cout << " " <<  Opt.ysize(f,imager,c) << " \n" <<  std::flush;

                            }
                        }
                    }

                    //Set Radometric constants
                    for (f=0; f<Opt.sectors(imager);f++)
                        setRadiometric(Runits[f][imager],imagerDoc);

		    //printRadiometric(Runits[0][imager]);

                    // case 1: new frame number for this priority close earlier files, open new ones
                    // case 2: same frame number for this priority restore earlier files, continue processing
                    if ( FList->Check(imager,  imagerDoc.frame_number,  Opt.priority()?imagerDoc.Iscan.priority():0)  )
                        openFiles(imagerDoc);

                } // end  "Is this a new frame"


                // for imager, only block0 has frame end flag, but we don't want to close imager sector files
                // until after the blocks 1-10 (which follow the block0) containing the line data are processed
                // so set the imager_frame_end flag. So if the next block is 1-10, data gets written, if it is block0
                // "Is this a new frame" section above closes old files and opens new, and if is a block11, imager
                // sector files are also closed
                imager_frame_end = imagerDoc.Iscan.Frame_end();
                if (imager_frame_end == 1)
                    std::cout <<" found block 0 frame_end flag \n"<<std::flush;


                // If user asks for 'region' timestamp
                for (f=0;f<Opt.sectors(imager); f++)
                {
                    if (Opt.timeStamp(f,imager)  == 2 )
                    {
                        char cfileName[FILENAME_SIZE];
                        std::stringstream fileName ;
                        fileName << imagerDoc.T_sps_current;
                        fileName << "G";
                        fileName << (int)(imagerDoc.spcid());
                        fileName << InstString[imager][0];
                        fileName << std::ends;
                        fileName.rdbuf()->sgetn(cfileName, FILENAME_SIZE);
                        OutFile[f][imager]->setFnameStub(cfileName);

                        //Close imager files not in ROI
                        if (skipIt(f,imager,ALL))
                        {
                            std::cout << "WARNING *********************************************************\n" << std::flush;
                            delete OutFile[f][imager];
                            OutFile[f][imager]=NULL;
                        }
                    }
                }
}







void process_all_block11() {


     // some block 11s have 10bit words
     if (gvar.wordSize() == 10) {
   	   char unpack_8b[30];
        int words=gvar.wordCount();
        unpack10(gvar.data(), words , block_16bit_data);
        for (int i=0; i<24; i++){
   	     unpack_8b[i] = block_16bit_data[i];
        }

        sad=(SadId*) (unpack_8b );

   	} else {
        sad=(SadId*) (gvar.data() );
   	}




     if (Opt.debug() == Dsadid ) {
   	  if (sad->dataId() != 1)
        sad->print(DebugFile);
   	}

     if (thisBlockBadCrc && Opt.debug() == Dcrc) return;





     switch (sad->dataId())
     {
     case 1: //Sounder fill data
         return;

     case 7:{ //Imager imc
         if (!Opt.imagerIcse()) return;
         process_imager_icse();
         }
         return;

     case 14:{//Sounder imc
         if (! Opt.sounderIcse()) return;
         process_sounder_icse();
         }
         return;

     case 21://Imager telemetry statistics
         return;

     case 22:{ //Imager spacelook
         if (!Opt.imagerSpacelookStats() ) return;
         process_imager_spacelook();
         }
         return;

     case 25://Imager Cal Coef/limits
         return;

     case 26://Imager ECAL
         return;

     case 28://Imager blackbody
         return;

     case 31://Imager NLUT
         return;

     case 32:{ //Sounder doc
         if (Opt.sectors(sounder) == 0) return;
         if ( gvar.wordSize() != 8 ) return;
         process_sounder_doc();
   	    }
         return;

     case 35:{ //Sounder scan data
         if (Opt.sectors(sounder) == 0 || sounder_frame_number == -1 ) return;
         process_sounder_data();
         }
         return;

     case 37://Sounder telemetry stats
         return;

     case 38://Sounder spacelook data
         return;

     case 41://Sounder Cal/Coef Limits
         return;

     case 42://Sounder ECAL data
         return;

     case 44://Sounder blackbody data
         return;

     case 47://Sounder NLUT's data
         return;

     case 49://Imager Factory Coefficients
         return;

     case 50://GIMTACS text message
         //SADtext * txt=(SADtext *) sad->data();
         //    txt->print(GimtacsFile);
         return;

     case 52://SPS text message
         return;

     case 59://Imager star sense data
         return;

     case 61://Sounder star sense data
         return;

     default://Unidentified Block 11
         return;
         /* std::cout <<"Block Id 11.";
            std::cout <<(int) sad->dataId();
            std::cout <<" not valid \n";
            return;
            */
     }//switch (sad->dataId())


     return;
}


void check_imager_frame_end() {
   // if the frame end flag is set, close this frame's imager output files
   if (imager_frame_end == 1) {
       imager_frame_end = 0;
       std::cout <<" block 0 frame_end flag and GVAR block 11 triggered sector image close\n"<<std::flush;

       // imagerDoc.frame_number+1 is just a number that is not imagerDoc.frame_number
       // so it triggers all the files to be closed
       FList->Check(imager,   imagerDoc.frame_number+1,  Opt.priority()?imagerDoc.Iscan.priority():0);
   }

}


void set_radiometric_units(int spcid)
{
    int f, s, i, c, d;

    std::cout << "******** Re-initializing to radiometric units for " << spcid << " \n" << std::flush;



    // visible, ir2 and ir4 channels have the same resolution for all generations

    // ir3, ir5 and ir6 channels vary according to generation
    // default values are for GOES 8-11
    N_Channels[imager]             = 5;
    VIS_LINES_PER_LINE[imager][I3] = 8;
    VIS_LINES_PER_LINE[imager][I5] = 4;
    VIS_LINES_PER_LINE[imager][I6] = 0;


    // GOES 12 and 13 
    if (spcid == 12 || spcid == 13)
    {
        N_Channels[imager]             = 6;
        VIS_LINES_PER_LINE[imager][I3] = 4;
        VIS_LINES_PER_LINE[imager][I5] = 0;
        VIS_LINES_PER_LINE[imager][I6] = 8;
    }

    // GOES 14 and 15
    if (spcid >= 14 )
    {
        N_Channels[imager]             = 6;
        VIS_LINES_PER_LINE[imager][I3] = 4;
        VIS_LINES_PER_LINE[imager][I5] = 0;
        VIS_LINES_PER_LINE[imager][I6] = 4;
    }


    //Set up radiometric classes for sectors
    if (!Runits)
        Runits=new Radiometric *
        [Opt.sectors()][INSTRUMENTS][CHANNELS][SIDES][DETECTORS];

    //Set Up radiometric conversion table information
    //from user supplied arguments
    for (i=imager; i<=sounder; i++)
    {
        if (Opt.sectors(i)>0)
        {
            for (f=0; f<Opt.sectors(i); f++)
            {
                for (c=0; c<max_channels[i]; c++)
                {
                    for (s=0; s<Side[i][c]; s++)
                    {
                        for (d=0; d<max_detectors[i][c]; d++)
                        {
                            Runits[f][i][c][s][d]=new Radiometric;
                            Runits[f][i][c][s][d]->units() = Opt.units(f,i,c);
                            Runits[f][i][c][s][d]->setWordType(Opt.wordType(f,i,c));
                            Runits[f][i][c][s][d]->setUserBandGG(Opt.bias(f,i,c), Opt.gain(f,i,c), Opt.gamma(f,i,c) );
                            Runits[f][i][c][s][d]->setMapDot(Opt.mapVal(f,i,c));
                        }
                    }
                }
            }
        }
    }
    Initialized++;

} 














int main(int argc, char ** argv)
{

    //Crc check counters
    int good_crc_count    = 0;
    int bad_crc_count     = 0;

    int f;

    //timer variables
    time_t timerStart;


    //print processID
    std::cout <<"\n My process id is : "<<getpid()<<"\n"<<std::flush;




    //Read command line
    Opt.ParseCommandLine(argc, argv);

    //print argument list for sanity check
    Opt.PrintCommandLine(argc, argv);




    // reset allowed files to system max
    struct rlimit rlp;
    getrlimit(RLIMIT_NOFILE, &rlp);
    rlp.rlim_cur=rlp.rlim_max;
    if (Opt.debug())
        std::cout <<"Setting allowed open files, was "<<rlp.rlim_cur
        <<" reset to to "<<rlp.rlim_max<<"\n"<<std::flush;
    setrlimit(RLIMIT_NOFILE, &rlp);


    //Set up output filebuf classes for sectors
    OutFile=new FileBuf * [Opt.sectors()][INSTRUMENTS];

    //some compilers do not inilize to NULL
    for (f=0; f<Opt.sectors(); f++)
    {
        OutFile[f][imager]=NULL;
        OutFile[f][sounder]=NULL;
    }

    // set up framelist array to keep file names based on priority interrupt levels
    FList = new FrameList;


    //Debug output requested
    if (Opt.debug() !=Debug0)
    {
        DebugFile.open("DebugFile");
        gvar.headerText(DebugFile);
        if (Opt.debug() == Dcrc )
            DebugFile<<"CRC\t";
    }
    //Various levels of debugging information
    switch (Opt.debug())
    {
    case Debug0:
    case Dheader:
        break;
    case Dframe:
        break;
    case Dlinedoc:
        lineDoc->lineDocText(DebugFile);
        break;
    case Dblock0time:
        imagerDoc.block0timeText(DebugFile);
        break;
    case Dsadid:
        sad->printHeader(DebugFile);
    default:
        break;
    }


    // loop over gvar filenames to process
    while ( (RawFile = Opt.nextFile(argc, argv)) != 0 )
    {
        gvar.open(RawFile);
        std::cout <<"\nprocessing file ";
        std::cout <<RawFile<<"\n"<<std::flush;

        //Log output requested
        // on opening a gvar file is 1-line file with name of last file processed
        if (Opt.log() ){
            LogFile.open(Opt.log() ); //, std::ofstream::out | std::ofstream::app);
            LogFile<<RawFile<<"\n";
            LogFile.flush();
        }


        //restart timers
        timerStart=time((time_t*)NULL);

        // loop over blocks found in this raw gvar file
        while ( gvar.getBlock() )
        {
            // At this point we have a GVAR header and the chunk of bytes that it claims are data.
            // So far headers are only accepted if at least one of the triple redundant 30byte
            // headers has passed CRC, so header values are reasonably likely (guaranteed?) to be good;
            // however most block data values are not redundant so those can still be bad
            // even if the header is good

            // Do CRC check on the data in the block
	    thisBlockBadCrc = gvar.crcBlock();

            //   the first time through  OR  if spacecraft id changes
	    //     extract the spacecraft ID in order to initialize radiometric constants
            if (!Initialized || current_spacecraft_id != gvar.spcID())
            {
                set_radiometric_units(gvar.spcID());
                // set the global variable
                current_spacecraft_id = gvar.spcID();

                std::cout <<"   GVAR format version = "<<gvar.versionNumber();
                std::cout <<",   spacecraft ID = "<< current_spacecraft_id <<"\n\n"<<std::flush;
            }


            // print various logging and debug information from the block header
            //if counter is requested, only print every 64th time through the "get block" loop
            if (Opt.progress_counter())
                if ((gvar.blockCount() % 64) == 0 ) StopWatch(gvar.percentToDo());
            if (Opt.debug() !=Debug0 && Opt.debug() !=Dframe && gvar.productId() != 0) {
                DebugFile<<"\n";
                gvar.printHeader(DebugFile);
            }
            if (Opt.debug() == Dframe) {
	        if (gvar.blockId() == 0) DebugFile<<"\n";
                DebugFile<<"\n";
                gvar.printDframe(DebugFile);
	    }
            if (Opt.debug() == Dcrc) {
                DebugFile<<thisBlockBadCrc<<"\t";
                if (thisBlockBadCrc) {
                    std::cerr << "block id = " << gvar.blockId() << ", count = " << gvar.blockCount() << ": Crc Check failed \n";
                    bad_crc_count++;
                } else {
                    good_crc_count++;
		}
            }



            //std::cout << "***** Block " << gvar.blockId() << " *****\n" << std::flush;
            switch (gvar.blockId())
            {
               case 0:  //Block 0; imagerDoc
	              process_imager_block0();
                      break;


               case 1: // Block 1 imager IR channels
               case 2: // Block 2 imager IR channels
	              if (imager_frame_number == -1 || (thisBlockBadCrc && Opt.debug() == Dcrc) ) break;
                      process_imager_ir_data();
                      break;


               case 3: //VIS imager detector 0
               case 4: //VIS imager detector 1
               case 5: //VIS imager detector 2
               case 6: //VIS imager detector 3
               case 7: //VIS imager detector 4
               case 8: //VIS imager detector 5
               case 9: //VIS imager detector 6
               case 10://VIS imager detector 7
	              if (imager_frame_number == -1 || (thisBlockBadCrc && Opt.debug() == Dcrc)) break;
                      process_imager_visible_data();
                      break;


               case 11: //Sounder & aux data
	              check_imager_frame_end();
                      process_all_block11();
                      break;


               default://Unidentified gvar block
	              check_imager_frame_end();
                      std::cout <<"Gvar Block Id "<<gvar.blockId()<<" not valid\n"<<std::flush;
                      break;
            }


        }//while( gvar.getBlock() )



        time_t timerStop=time((time_t*)NULL);

        std::cout <<"\nJunked "<<gvar.getTrashBytes();
        std::cout <<" bytes from input "<<gvar.getBytesRead();
        std::cout <<" ("<<100.* gvar.getTrashBytes()/gvar.getBytesRead();
        std::cout <<"%) "<<std::flush;

        if (Opt.debug() == Dcrc){
            std::cout << "\nBlocks with CRC error " << bad_crc_count << " out of ";
            std::cout << bad_crc_count+good_crc_count << " (";
            std::cout << (100.0*bad_crc_count/(bad_crc_count+good_crc_count)) << "%)";

            bad_crc_count  = 0;
            good_crc_count = 0;
        }

        std::cout <<"\nProcessed ";
        std::cout <<RawFile<<" at rate of ";
        std::cout <<8.0 * gvar.getBytesRead() / (timerStop-timerStart+1) /1E6;
        std::cout <<" Mbits/RealSec\n"<<std::flush;

        //Log output requested
        // for now just 1-line file with name of last file processed
        if (Opt.log() ){
	    LogFile<<RawFile<<"\n";
            LogFile.close();
        }

        std::cerr.flush();
        std::cout.flush();

    }//while RawFile



    delete block_16bit_data;

    if (Opt.debug() !=Debug0){
        DebugFile<<"\n";
        DebugFile.flush();
    }

    delete [] Runits;
    delete FList;
    delete [] OutFile;

    return(0);
}

//main


