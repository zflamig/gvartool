//#include <strstream.h>
//#include <stream.h>
#include <sstream>
#include "include/types.h"
#include "include/gvar.h"
#include "utils/fileBuf/genericFile.h"
#include "utils/options/options.h"
#include "utils/radiometric/radiometric.h"

#include "block0/imagerDoc.h"
#include "block11/doc/sounderDoc.h"

/*template<class T> void openFiles(T& DOC);*/

#include "utils/openFiles/openFiles.h"

const char * InstString[] = { "Imager","Sounder"};
const char* WordString[] = {
  "Undef", "Uchar", "char",  "Ushort",
  "short", "Uint",  "int",    "float",
  "double" };

extern FileBuf* (*OutFile)[INSTRUMENTS];   
extern char * RawFile;

/*template<class T >void openFiles(T& DOC){ */
void openFiles(ImagerDoc & DOC)
{
  extern Options Opt; 
  extern Radiometric* 
    (*Runits)[INSTRUMENTS][CHANNELS][SIDES][DETECTORS];

  int i = DOC.instrument();

  char ctext[DESC_STRING_SIZE];
  std::stringstream text;  

  //set_nav_constants( i );

  text<<"\nGOES ";   text<< (int) DOC.spcid();
  text<<" ";         text<< InstString[DOC.instrument()];
  text<<" frame ";   text.width(3); text<< (int) DOC.frame_number; 

  text.fill('0'); 
  text<<" at UTC ";  text.width(2); text<< DOC.T_sps_current.hrs(); 
  text<<":";         text.width(2); text<< DOC.T_sps_current.min(); 
  text<<":";         text.width(2); text<< DOC.T_sps_current.sec();
  text<<".";         text.width(3); text<< DOC.T_sps_current.msec(); 
  text<<" day ";     text.width(3); text<< DOC.T_sps_current.day();
  text<<" of ";                     text<< DOC.T_sps_current.year();
  text<<"\n";
  text.fill(' '); 


  text<<"Vis pixels : ";   text.width(6);   text<<DOC.W_pix_of_frame;   text<<"," ;   text.width(6);   text<<DOC.E_pix_of_frame;
  text<<"    lines : " ;   text.width(6);   text<<DOC.N_line_of_frame;  text<<"," ;   text.width(6);   text<<DOC.S_line_of_frame;
  text<<"\n";
   
  text<<"Vis Lon    : ";   text.width(6);   text<<DOC.NW_lon_of_frame;   text<<",";   text.width(6);   text<<DOC.SE_lon_of_frame;   
  text<<" Lat   : "    ;   text.width(6);   text<<DOC.NW_lat_of_frame;   text<<",";   text.width(6);   text<< DOC.SE_lat_of_frame;
  text<< "\n";
  
  text<<std::ends; 
  text.rdbuf()->sgetn(ctext, DESC_STRING_SIZE);
  
  std::cout << ctext; 
  std::cout.flush();

  for (int f=0; f<Opt.sectors(i) ; f++){

    if (OutFile[f][i]!=NULL)
      delete OutFile[f][i];
    OutFile[f][i] = NULL;

    OutFile[f][i] = GenericFile(Opt.fileType(f,i));

    if (i == sounder) OutFile[f][i]->setBufLines(8);

    OutFile[f][i]->setChannels(N_Channels[i]);
    OutFile[f][i]->setComment(ctext);
    OutFile[f][i]->setCompress(Opt.compress(f,i));
    OutFile[f][i]->linkLatest() = Opt.linkLatest(f,i);

    char cfileName[FILENAME_SIZE]; 
    std::stringstream fileName ;
    if (Opt.timeStamp(f,i) == 1 || Opt.timeStamp(f,i) == 2) {
      fileName << DOC.T_sps_current; 
    }
    else if (Opt.timeStamp(f,i) == 3) {
      DOC.T_sps_current.timestamp( fileName );
    }
    else
    {
      fileName << baseName(RawFile);
    }
    fileName << "G";
    fileName << (int)(DOC.spcid());
    fileName << InstString[i][0];
    fileName << std::ends; 
    fileName.rdbuf()->sgetn(cfileName, FILENAME_SIZE);
    OutFile[f][i]->setFnameStub(cfileName);
    
    for(int c = 0; c < N_Channels[i]; c++){

      int Xstart=VALID(Opt.xstart(f,i,c),DOC.W_pix_of_frame);
      int Ystart=VALID(Opt.ystart(f,i,c),DOC.N_line_of_frame);
      int Xend  =VALID(Opt.xend  (f,i,c),DOC.E_pix_of_frame);
      int Yend  =VALID(Opt.yend  (f,i,c),DOC.S_line_of_frame);

      int Xsize=(Xend-Xstart+1);
      int Ysize=(Yend-Ystart+1);
      double fx, fy;

      fx = ((double)Xsize/    (double)VIS_PIX_PER_PIX[i][c])   * Opt.xscale(f,i,c);
      fy = ((double)Ysize/ (double)VIS_LINES_PER_LINE[i][c])   * Opt.yscale(f,i,c);

      OutFile[f][i]->setChannelSizes(c, (unsigned)fy, (unsigned)fx);

      // start line and pixel in scaled coordinates
      fx = ((double)Xstart/   (double)VIS_PIX_PER_PIX[i][c])   * Opt.xscale(f,i,c);
      fy = ((double)Ystart/(double)VIS_LINES_PER_LINE[i][c])   * Opt.yscale(f,i,c);

      OutFile[f][i]->setChannelOffsets(c, (int)fy, (int)fx);

      //std::cout  << "\n channel offsets " <<  fx << ", "  <<  fy << std::flush;

      fx =    (double)VIS_PIX_PER_PIX[i][c]/Opt.xscale(f,i,c);
      fy = (double)VIS_LINES_PER_LINE[i][c]/Opt.yscale(f,i,c);

      OutFile[f][i]->setChannelStrides(c, (int)fy, (int)fx);

      OutFile[f][i]->setChannelWordType(c,Opt.wordType(f,i,c)) ;

      OutFile[f][i]->setChannelDirectory(c,Opt.directory(f,i,c)) ;

      if( Runits[f][i][c]) 
	OutFile[f][i]->setChannelMinMax
	  (c,
	   Runits[f][i][c][0][0]->getMin(),
	   Runits[f][i][c][0][0]->getMax() );


      char ctext1[DESC_STRING_SIZE];
      std::stringstream text1;  
      
      text1<< InstString[i]; 
      text1<<" ch" << c+1; 
      text1<<":count(0,"<< MAXCOUNTS[i]<<") => [(";
      text1<<Runits[f][i][c][0][0]->unitsStr();
      text1<<"-"; 
      text1<<Runits[f][i][c][0][0]->getUserBias(); 
      text1<<")/";
      text1<<Runits[f][i][c][0][0]->getUserGain(); 
      text1<<"]^";
      text1<<Runits[f][i][c][0][0]->getUserGamma();
      text1<<" =";
      text1<< WordString[Opt.wordType(f,i,c)];
      text1<< "> (";
      text1<<Runits[f][i][c][0][0]->getConverted(0)<<",";
      text1<<Runits[f][i][c][0][0]->getConverted(MAXCOUNTS[i]);

      text1<<")\n(xscale,yscale) => (";
      text1<<Opt.xscale(f,i,c)<<"," ;
      text1<<Opt.yscale(f,i,c)<<")" ; 


      text1<<"\nVis pixels:" ;      text1.width(6);
      text1<<Xstart;
      text1<<", ";
      text1.width(6);
      text1<<Xend;

      text1<<" lines:" ;
      text1.width(6);
      text1<<Ystart;
      text1<<", ";
      text1.width(6);
      text1<<Yend;

      text1<<"\n(xstride,ystride) => (";
      text1<<VIS_PIX_PER_PIX[i][c]/Opt.xscale(f,i,c)<<"," ; 
      text1<<VIS_LINES_PER_LINE[i][c]/Opt.yscale(f,i,c)<<")" ;


      text1<<"\n";
      text1<<"IMC status: " ;
      text1.width(6);
      text1<<DOC.imc() ;


      text1<<"\n";
      text1<<"nadir pixel,line: " ;
      text1.width(6);
      text1<<DOC.pix_of_zero_az;
      text1<<"," ;
      text1.width(6);
      text1<<DOC.line_of_zero_elev;

      text1<<"\n";
      text1<<"Ref lon,d,lat,yaw: " ;
      text1.width(13);
      text1.precision(10);
      text1<<(double)DOC.ReferenceLongitude*45/atan(1)      ;
      text1<<"," ;
      text1.width(14);
      text1.precision(11);
      text1<<(double)DOC.ReferenceRadialDistance+42164.17478 ;
      text1<<"," ;
      text1.width(10);
      text1<<(double)DOC.ReferenceLatitude*45/atan(1)       ;
      text1.precision(8);
      text1<<"," ;
      text1.width(10);
      text1.precision(8);
      text1<<(double)DOC.ReferenceOrbitYaw*45/atan(1)       ;
  
      text1<<"\n";
      text1<<"Attitude roll,pitch,yaw: " ;
      text1.width(10);
      text1.precision(8);
      text1<<(double)DOC.ReferenceAttitudeRoll*45/atan(1) ;
      text1<<"," ;
      text1.width(10);
      text1.precision(8);
      text1<<(double)DOC.ReferenceAttitudePitch*45/atan(1);
      text1<<"," ;
      text1.width(10);
      text1.precision(8);
      text1<<(double)DOC.ReferenceAttitudeYaw*45/atan(1)  ;
  



      text1<<std::ends;
      text1.rdbuf()->sgetn(ctext1, DESC_STRING_SIZE);
      OutFile[f][i]->setComment(ctext1,c);

      //      std::cout << ctext1; 
      //std::cout.flush();



    }
  }
} // end openFiles(imager)


void openFiles(SounderDoc & DOC)
{
  extern Options Opt; 
  extern Radiometric* 
    (*Runits)[INSTRUMENTS][CHANNELS][SIDES][DETECTORS];

  int i = DOC.instrument();

  char ctext[FILENAME_SIZE];
  std::stringstream text;  

  text<<"\n";

  text<<"GOES "; 
  text<< (int) DOC.spcid();
  text<<" " ; 
  text<<InstString[DOC.instrument()];
  text<<" frame "; 
  text<< (int) DOC.frame_number; 
  text<<" at UTC " ; 
  text<<DOC.T_sps_current.hrs() ; 
  text<<":"<<DOC.T_sps_current.min(); 
  text<<":"<<DOC.T_sps_current.sec();
  text<<"."<<DOC.T_sps_current.msec(); 
  text<<" day "<<DOC.T_sps_current.day() ;
  text<<" of "<<DOC.T_sps_current.year()<<" " ;
  
  text<<"\n";

  text<<"Vis pixels:";
  text.width(6);
  text<<DOC.W_pix_of_frame;
  text<<"," ;
  text.width(6);
  text<<DOC.E_pix_of_frame;
  text<<" lines:";
  text.width(6);
  text<<DOC.N_line_of_frame; 
  text<<"," ; 
  text.width(6);
  text<<DOC.S_line_of_frame;
  text<<"\n";

  text<<"Vis Lon :";
  text.width(6);
  text<<DOC.NW_lon_of_frame;
  text<<",";
  text.width(6);
  text<< DOC.SE_lon_of_frame;
  text<<" Lat   :";
  text.width(6);
  text<<DOC.NW_lat_of_frame; 
  text<<",";
  text.width(6);
  text<<DOC.SE_lat_of_frame;   
  text<< "\n";

  text<<std::ends; 
  text.rdbuf()->sgetn(ctext, FILENAME_SIZE);
  
  std::cout << ctext; 
  std::cout.flush();

  for (int f=0; f<Opt.sectors(i) ; f++){
    if (OutFile[f][i]!=NULL)
      delete OutFile[f][i];
    OutFile[f][i] = NULL;

    OutFile[f][i] = GenericFile(Opt.fileType(f,i));
    if (i == sounder) OutFile[f][i]->setBufLines(8);

    OutFile[f][i]->setChannels(N_Channels[i]);
    OutFile[f][i]->setComment(ctext);
    OutFile[f][i]->setCompress(Opt.compress(f,i));
    OutFile[f][i]->linkLatest() = Opt.linkLatest(f,i);

    char cfileName[FILENAME_SIZE]; 
    std::stringstream fileName ;
    if (Opt.timeStamp(f,i) == 1 || Opt.timeStamp(f,i) == 2) {
      fileName << DOC.T_sps_current; 
    }
    else if (Opt.timeStamp(f,i) == 3) {
      DOC.T_sps_current.timestamp( fileName );
    }
    else {
       fileName << baseName(RawFile);
    }
    fileName << "G";
    fileName << (int)(DOC.spcid());
    fileName << InstString[i][0];
    fileName << std::ends; 
    fileName.rdbuf()->sgetn(cfileName, FILENAME_SIZE);
    OutFile[f][i]->setFnameStub(cfileName);
    
    for(int c = 0; c < N_Channels[i]; c++){
      int Xstart=VALID(Opt.xstart(f,i,c),DOC.W_pix_of_frame);
      int Ystart=VALID(Opt.ystart(f,i,c),DOC.N_line_of_frame);
      int Xend  =VALID(Opt.xend  (f,i,c),DOC.E_pix_of_frame);
      int Yend  =VALID(Opt.yend  (f,i,c),DOC.S_line_of_frame);
      int Xsize=(Xend-Xstart+1);
      int Ysize=(Yend-Ystart+1);
      double fx, fy;

      fy = (double)Ysize/(double)VIS_LINES_PER_LINE[i][c]*Opt.yscale(f,i,c);
      fx = (double)Xsize/   (double)VIS_PIX_PER_PIX[i][c]*Opt.xscale(f,i,c);
      OutFile[f][i]->setChannelSizes
	(c,
	 (unsigned)fy,
	 (unsigned)fx);

      fy = (double)Ystart/(double)VIS_LINES_PER_LINE[i][c]*Opt.yscale(f,i,c);
      fx = (double)Xstart/   (double)VIS_PIX_PER_PIX[i][c]*Opt.xscale(f,i,c);
      OutFile[f][i]->setChannelOffsets
	(c,
	 (int)fy,
	 (int)fx);

      OutFile[f][i]->setChannelStrides
	(c,
	 VIS_LINES_PER_LINE[i][c]*Opt.yscale(f,i,c),
	 VIS_PIX_PER_PIX[i][c]*Opt.xscale(f,i,c) );

      OutFile[f][i]->setChannelWordType(c,Opt.wordType(f,i,c)) ;

      OutFile[f][i]->setChannelDirectory(c,Opt.directory(f,i,c)) ;

      if( Runits[f][i][c]) 
	OutFile[f][i]->setChannelMinMax
	  (c,
	   Runits[f][i][c][0][0]->getMin(),
	   Runits[f][i][c][0][0]->getMax() );

      

      char ctext1[FILENAME_SIZE];
      std::stringstream text1;  
      
      text1<< InstString[i]; 
      text1<<" ch" << c+1; 
      text1<<":count(0,"<< MAXCOUNTS[i]<<") => [(";
      text1<<Runits[f][i][c][0][0]->unitsStr();
      text1<<"-"; 
      text1<<Runits[f][i][c][0][0]->getUserBias(); 
      text1<<")/";
      text1<<Runits[f][i][c][0][0]->getUserGain(); 
      text1<<"]^";
      text1<<Runits[f][i][c][0][0]->getUserGamma();
      text1<<" =";
      text1<< WordString[Opt.wordType(f,i,c)];
      text1<< "> (";
      text1<<Runits[f][i][c][0][0]->getConverted(0)<<",";
      text1<<Runits[f][i][c][0][0]->getConverted(MAXCOUNTS[i]);

      text1<<")\n(xscale,yscale) => (";
      text1<<Opt.xscale(f,i,c)<<"," ;
      text1<<Opt.yscale(f,i,c)<<")" ; 


      text1<<"\nVis pixels:" ;
      text1.width(6);
      text1<<Xstart;
      text1<<", ";
      text1.width(6);
      text1<<Xend;

      text1<<" lines:" ;
      text1.width(6);
      text1<<Ystart;
      text1<<", ";
      text1.width(6);
      text1<<Yend;

      text1<<"\n(xstride,ystride) => (";
      text1<<VIS_PIX_PER_PIX[i][c]/Opt.xscale(f,i,c)<<"," ; 
      text1<<VIS_LINES_PER_LINE[i][c]/Opt.yscale(f,i,c)<<")" ;

      text1<<"\n";
      text1<<"IMC status: " ;
      text1.width(6);
      text1<<DOC.imc() ;


      text1<<"\n";
      text1<<"nadir pixel,line: " ;
      text1.width(6);
      text1<<DOC.pix_of_zero_az;
      text1<<"," ;
      text1.width(6);
      text1<<DOC.line_of_zero_elev;

      text1<<"\n";
      text1<<"Ref lon,d,lat,yaw: " ;
      text1.width(13);
      text1.precision(10);
      text1<<(double)DOC.ReferenceLongitude*45/atan(1)      ;
      text1<<"," ;
      text1.width(14);
      text1.precision(11);
      text1<<(double)DOC.ReferenceRadialDistance+42164.17478 ;
      text1<<"," ;
      text1.width(10);
      text1<<(double)DOC.ReferenceLatitude*45/atan(1)       ;
      text1.precision(8);
      text1<<"," ;
      text1.width(10);
      text1.precision(8);
      text1<<(double)DOC.ReferenceOrbitYaw*45/atan(1)       ;
  
      text1<<"\n";
      text1<<"Attitude roll,pitch,yaw: " ;
      text1.width(10);
      text1.precision(8);
      text1<<(double)DOC.ReferenceAttitudeRoll*45/atan(1) ;
      text1<<"," ;
      text1.width(10);
      text1.precision(8);
      text1<<(double)DOC.ReferenceAttitudePitch*45/atan(1);
      text1<<"," ;
      text1.width(10);
      text1.precision(8);
      text1<<(double)DOC.ReferenceAttitudeYaw*45/atan(1)  ;
  

      text1<<std::ends;
      text1.rdbuf()->sgetn(ctext1, FILENAME_SIZE);

      OutFile[f][i]->setComment(ctext1,c);
    }
  }
} // end openFiles(sounder)

