#ifdef __GNUG__
#pragma implementation
#endif

#include <stdio.h>
//#include <stream.h>
//#include <strstream.h>
#include <sstream>

#include "fileMcIDAS.h"
#include "block1-10/lineDoc.h"
#include "gvarBlock/gvarBlock.h"

extern ImagerDoc imagerDoc; /* current block 0*/
extern LineDoc* lineDoc; /* current line DOC */
extern GvarBlock gvar; /* current block header */

FileMcIDAS::FileMcIDAS() 
{ Bin = NULL ; Doc = NULL; FileBuf::BufLines = 1; }

FileMcIDAS::~FileMcIDAS(){
  for(int c = 0; c < (int)Channels; c++)
    if(Bin && Bin[c]) Close(c);
  delete Bin;
  delete Doc;
}

int FileMcIDAS::Open(int channel){
  if (!Bin) { 
    Bin = new fildes [Channels];
    Doc = new fildes [Channels];
    for ( int c = 0; c < (int)Channels; c++) { 
      Bin[c] = 0;
      Doc[c] = 0;
    }
  }
  if ( ! Bin[channel]){
    Bin[channel] = open(tmpName(FileName(channel)), 
			O_RDWR | O_CREAT, 
			S_IROTH| S_IRUSR |S_IWUSR |S_IRGRP  );

    Doc[channel] = open(FileName(channel,"info"),
			O_RDWR | O_CREAT ,
			S_IROTH| S_IRUSR |S_IWUSR |S_IRGRP  );
  
    InitMcIDAS(imagerDoc,channel);
    write(Bin[channel],&mcidas,(64+640+128)*sizeof(int));
  }
  return(0);
}

int FileMcIDAS::Close(int channel){
  if (Bin[channel]) { 
    bufFlush(channel);
    
    if(GlobalComment)
      write(Doc[channel],GlobalComment,strlen(GlobalComment));

    if(ChannelComment[channel])
      write(Doc[channel],ChannelComment[channel],strlen(ChannelComment[channel]));

    close(Bin[channel]);
    close(Doc[channel]);
    
    rename(tmpName(FileName(channel)), FileName(channel));
    if(linkLatest()) MakeLink(channel);
  }
  return(0);
}

int FileMcIDAS::DiskWrite(int channel){  
  
  McIDAS_LineDoc mline;

  mline.ValidityCode = 0 ;
  mline.BlockHeaderCRC = gvar.crc();
  memcpy(&mline.ScanStatus,&imagerDoc.Iscan,4); 
  memcpy(mline.Time,&imagerDoc.T_sps_current,sizeof(imagerDoc.T_sps_current) );  
  memcpy(mline.BlockHeader,&gvar.Hdr, 30);
  memcpy(mline.LineDoc, lineDoc, 32);

  write(Bin[channel],&mline, 80);
  write(Bin[channel],DataPtr(channel,0,0), 
	xSize(channel)*wordSize(channel));
  
  return(0);
}

char* FileMcIDAS::FileName(int channel, char * Extn){
  static char tmp[FILENAME_SIZE];
  std::stringstream fn; 
  fn << ChannelDirectory[channel];
  fn << FnameStub;
  fn.fill('0'); fn.width(2);
  fn << channel+1;
  if(Extn) fn << Extn;
  else fn << ".mcidas";
  fn << std::ends;
  fn.rdbuf()->sgetn(tmp, FILENAME_SIZE);
  return(tmp);
}


void FileMcIDAS::InitMcIDAS(ImagerDoc & DOC /*block 0*/, int channel){

  mcidas.W1 = 0 ;           // W1

  mcidas.AreaFormat = 4 ;   // W2 always 4

  mcidas.SSS = McIDASImager_SatelliteID[DOC.spcid() - 8 ]; 
  // W3 Satellite Id #

  mcidas.YYDDD = ((DOC.T_sps_current.year()-1900) * 1000 
		  + DOC.T_sps_current.day() );
  // W4 Nominal Year and Julian day of area

  mcidas.HHMMSS = ( DOC.T_sps_current.hrs()*10000 
		   + DOC.T_sps_current.min() * 100 
		   + DOC.T_sps_current.sec() );
  // W5 Nominal time of image

  mcidas.UpperLeftLine = yStart(channel);
  // W6 image line for area line0, elem0

  mcidas.UpperLeftElem = xStart(channel) ;
  // W7 image elem for area line0, elem0

  mcidas.W8 = 1 ;           // W8 not used

  mcidas.Nlines = (long)ySize(channel);
  // W9 number of lines in area

  mcidas.Neles  = (long)xSize(channel);
  // W10 number of elements in each line

  mcidas.Elesiz = wordSize(channel);  
  // W11 number of bytes/element

  mcidas.Lineres = (int) yStride(channel); 
  // W12 spcng in img lns btwn cons area lines
  
  mcidas.Eleres  = (int) xStride(channel);  
  // W13 spcng in img ele bten cons area elems

  mcidas.Nchans = 1 ;
  // W14 max # bands/line of area

  mcidas.Presiz = 80;
  // W15 length of line prefix in bytes

  mcidas.Proj = 0 ; 
  // W16 McIDAS user project #

  time_t now = time(NULL);
  struct tm * local =  localtime(&now);
  
  mcidas.CreationDate = (local->tm_year* 1000 
			 + local->tm_yday);
  // W17 Area creation date in YYDDD

  mcidas.CreationTime = (local->tm_hour * 10000 
			 + local->tm_min * 100 
			 + local->tm_sec );
  // W18 Area creation date in HHMMSS

  mcidas.FilterMap = 1<<channel;
  // W19 for multi-channel images 32 bit vector
  
  int i;
  for( i = 0; i < 5; i++)
    mcidas.W20_24[i] = 0 ; 
  // W20-24 for internal use

  for(i = 0; i < 8; i++)
    mcidas.W25_32[i] = 0 ;    // W25-32 Memo, Comments

  for(i = 0; i < 3; i++)
    mcidas.W33_35[i] = 0 ;    // W33_35 for internal use
  mcidas.W33_35[2] = 256;

  mcidas.ValidityCode= 0 ;
  // W36 Validity Code
  
  for(i=0; i<8; i++)
    mcidas.W37_44[i] = 0;  
  // W37-44 PDL indicates packed byte format
  
  mcidas.W45 = 0 ;
  // W45 for mode AA
  
  mcidas.SSS = McIDASImager_SatelliteID[DOC.spcid() - 8 ]; 
  // W3 Satellite Id #

  mcidas.ActualStartDate = mcidas.YYDDD;
  // W46 YYDDD
  
  mcidas.ActualStartTime = mcidas.HHMMSS;
  // W47 HHMMSS
  
  mcidas.ActualStartScan = DOC.N_line_of_frame; // W48 

  mcidas.LinePrefixBytes = 76 ;
  // W49 Line prefix doc in bytes

  mcidas.LinePrefixCal = 0 ;
  // W50 Line prefix calibration in bytes

  mcidas.LinePrefixMap = 0 ;
  // W51 Line prefix level map in bytes

  mcidas.ImageSourceType = 1196835154 ; // 'GVAR'
  // W52 'VISR' || 'GVAR' || 'AAA' || 'ERBE'

  mcidas.CalibrationType = 1380013856; // 'RAW'
  // W53 'RAW' || 'TEMP' || 'BRIT'

  for(i=0;i<11;i++)
    mcidas.W54_64[i] = 0;     // W54-64 Intenal use only

  // Navigation 

  mcidas.NavigationType = 1196835154; // 'GVAR'
  // W1 'GVAR'

  mcidas.ImcFlag = 0; // DOC.imc(); 
  // W2 IMC flag (0-active, 1-inactive)

  for(i=0;i<3;i++)
    mcidas.W3_5[i] = 0 ;        // W3-5 Not used

  mcidas.RefLongitude = (int)( (float)DOC.ReferenceLongitude * 1E7 );
  // W6 Ref Longitude (rad * 10^7 )

  mcidas.RefNomDist = (int)( (float) DOC.ReferenceRadialDistance * 1E7);
  // W7 Ref distance from nominal (km *10^7)

  mcidas.RefLatitude = (int)((double) DOC.ReferenceLatitude * 1E7);
  // W8 Ref Latitude  (rad * 10^7 )

  mcidas.RefYaw = (int)((double)DOC.ReferenceOrbitYaw *1E7);
  // W9 Ref Yaw       (rad * 10^7 )

  mcidas.RefAttitudeRoll = (int)((double)DOC.ReferenceAttitudeRoll *1E7);
  // W10 Ref  attitude roll (rad * 10^7 )

  mcidas.RefAttitudePitch = (int)((double)DOC.ReferenceAttitudePitch * 1E7);
  // W11 Ref attitude pitch (rad * 10^7 )

  mcidas.RefAttitudeYaw = (int)((double)DOC.ReferenceAttitudeYaw * 1E7);
  // W12 Ref attitude yaw (rad * 10^7 )

  memcpy(mcidas.EpochTimeDate,&DOC.EpochDate,sizeof(DOC.EpochDate) );
  // W13-14 Epoch time data BCD format

  mcidas.DeltafromEpochTime = (int)((double)DOC.IMCenableFromEpoch*100);
  // W14 (minutes * 100 )

  mcidas.IMCroll = (int)((double)DOC.CompensationRoll*1E7);
  // W15 img motion comp roll (rad * 10^7)

  mcidas.IMCpitch = (int)((double)DOC.CompensationPitch*1E7);
  // W16 img motion comp pitch (rad * 10^7)

  mcidas.IMCyaw = (int)((double)DOC.CompensationYaw*1E7);
  // W18 img motion comp yaw (rad * 10^7)

  for(i=0;i<13;i++)
    mcidas.ChangeLongitude[i] =
      (int)((double)DOC.ChangeLongitude[i]*1E7) ;
  //W19-31 long delta from ref (rad*10^7)
  
  for(i=0;i<11;i++)
    mcidas.ChangeRadialDist[i] = 
      (int)((double)DOC.ChangeRadialDistance[i]*1E7);
  //W32-42 r dist delta from ref (rad*10^7)
  
  for(i=0;i<9;i++)
    mcidas.SineGeoCentricLat[i] =
      (int)((double)DOC.SineGeocentricLatitude[i]*1E7);
  //W43-51 (nounits *10^7)

  for(i=0;i<9;i++)
    mcidas.SineOrbitYaw[i] =
      (int)((double)DOC.SineOrbitYaw[i]*1E7);
  //W52-60 (nounits * 10^7)

  mcidas.DailySolarRate = 
    (int)((double)DOC.DailySolarRate*1E7);
  //W61 (rad/min * 10^7)
  
  mcidas.ExpStartFromEpoch = 
    (int)((double)DOC.ExponentialStartFromEpoch*100);
  // W62 (min * 100 )

  DOC.RollAngle.toMcIDASNavigation(mcidas.RollAttitudeAngle);
  //W63-117
  
  for(i=0;i<10;i++ )
    mcidas.W127_118[i]=0;   // W127-118 

  mcidas.W128 = 1297044037 ; // W128 'MORE'

  mcidas. W129 =  1196835154; // W129 'GVAR'
  
  DOC.PitchAngle.toMcIDASNavigation(mcidas.PitchAngle); // W130_184
     
  DOC.YawAngle.toMcIDASNavigation(mcidas.YawAngle);   // W185_239

  for(i=0;i<16;i++)
    mcidas.W240_255[i] = 0; //240-255

  mcidas.W256 = 1297044037;  // W256 'MORE'

  mcidas.W257 =  1196835154; //  W257 'GVAR'

  DOC.RollMisalignment.toMcIDASNavigation(mcidas.RollMisalignmentAngle); // W258-312
     
  DOC.PitchMisalignment.toMcIDASNavigation(mcidas.PitchMisalignmentAngle); // W313-367

  mcidas.InstrumentFlag = 1; //W368 (1=imager; 2 = sounder)

  mcidas.W369_YYDDD = mcidas.YYDDD; //W369
     
  mcidas.W370_HHMMSS = mcidas.HHMMSS; //W370

  for(i=0;i<13;i++)
    mcidas.W371_383[i] = 0; //W371-383 

  mcidas.W384 = 1297044037; //W384 'MORE'

  mcidas.W385 =  1196835154; //W385 'GVAR'

  for(i=0;i<125;i++)
    mcidas.W386_510[i] = 0; // W386-510

  mcidas.W511 = 1297044037; // W511 'MORE'

  mcidas.W512 =  1196835154; // W512 'GVAR'

  for(i=0;i<128;i++)
    mcidas.W513_640[i] = 0; //W513-640

  // Calibration 
     
  for(i=0;i<8;i++){
    mcidas.VisibleBias[i] = (float) DOC.Ivcrb[i]; // W1-8
    mcidas.VisibleFirstOrderGain[i] = (float) DOC.Ivcr1[i]; //W9-16
    mcidas.VisibleSecOrderGain[i] = (float) DOC.Ivcr2[i];   //W17-24
  }

  mcidas.VisibleRadianceToAlbedo = (float) DOC.Ivral; //W25
  
  mcidas.Side1IRBias[0] = (float) DOC.Iisfb[0][4]; // W26
  mcidas.Side1IRBias[1] = (float) DOC.Iisfb[0][6]; // W27
  mcidas.Side1IRBias[2] = (float) DOC.Iisfb[0][0]; // W28
  mcidas.Side1IRBias[3] = (float) DOC.Iisfb[0][2]; // W29

  mcidas.Side2IRBias[0] = (float) DOC.Iisfb[1][4]; // W30
  mcidas.Side2IRBias[1] = (float) DOC.Iisfb[1][6]; // W31
  mcidas.Side2IRBias[2] = (float) DOC.Iisfb[1][0]; // W32
  mcidas.Side2IRBias[3] = (float) DOC.Iisfb[1][2]; // W33

  mcidas.Side1IRGain[0] = (float) DOC.Iisf1[0][4]; // W34
  mcidas.Side1IRGain[1] = (float) DOC.Iisf1[0][6]; // W35
  mcidas.Side1IRGain[2] = (float) DOC.Iisf1[0][0]; // W36
  mcidas.Side1IRGain[3] = (float) DOC.Iisf1[0][2]; // W37
  
  mcidas.Side2IRGain[0] = (float) DOC.Iisf1[1][4]; // W38
  mcidas.Side2IRGain[1] = (float) DOC.Iisf1[1][6]; // W39
  mcidas.Side2IRGain[2] = (float) DOC.Iisf1[1][0]; // W40
  mcidas.Side2IRGain[3] = (float) DOC.Iisf1[1][2]; // W41


  for(i=0;i<87;i++)
    mcidas.W128_42[i] = 0; 
}




  
  
