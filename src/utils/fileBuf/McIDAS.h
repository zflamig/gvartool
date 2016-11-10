#include "include/types.h"
#include "block0/imagerDoc.h"

const int McIDASImager_SatelliteID[] = { 70, 72};
const int McIDASSounder_SatelliteID[] = { 71, 73};


struct McIDAS{
  sint32 W1;           // W1
  sint32 AreaFormat;   // W2 always 4
  sint32 SSS;          // W3 Satellite Id #
  sint32 YYDDD;        // W4 Nominal Year and Julian day of area
  sint32 HHMMSS;       // W5 Nominal time of image
  sint32 UpperLeftLine;// W6 image line for area line0, elem0
  sint32 UpperLeftElem;// W7 image elem for area line0, elem0
  sint32 W8;           // W8 not used
  sint32 Nlines;       // W9 number of lines in area
  sint32 Neles;        // W10 number of elements in each line
  sint32 Elesiz;       // W11 number of bytes/element
  sint32 Lineres;      // W12 spcng in img lns btwn cons area lines
  sint32 Eleres;       // W13 spcng in img ele bten cons area elems
  sint32 Nchans;       // W14 max # bands/line of area
  sint32 Presiz;       // W15 length of line prefix in bytes
  sint32 Proj;         // W16 McIDAS user project #
  sint32 CreationDate; // W17 Area creation date in YYDDD
  sint32 CreationTime; // W18 Area creation date in HHMMSS
  sint32 FilterMap;    // W19 for multi-channel images 32 bit vector
  sint32 W20_24[5];    // W20-24 for internal use
  sint32 W25_32[8];    // W25-32 Memo, Comments
  sint32 W33_35[3];    // W33_35 for internal use
  sint32 ValidityCode; // W36 Validity Code
  sint32 W37_44[8];    // W37-44 PDL indicates packed byte format
  sint32 W45;          // W45 for mode AA
  sint32 ActualStartDate;// W46 YYDDD
  sint32 ActualStartTime;// W47 HHMMSS
  sint32 ActualStartScan;// W48 
  sint32 LinePrefixBytes;// W49 Line prefix doc in bytes
  sint32 LinePrefixCal;  // W50 Line prefix calibration in bytes
  sint32 LinePrefixMap;  // W51 Line prefix level map in bytes
  sint32 ImageSourceType;// W52 'VISR' || 'GVAR' || 'AAA' || 'ERBE'
  sint32 CalibrationType;// W53 'RAW' || 'TEMP' || 'BRIT'
  sint32 W54_64[11];     // W54-64 Intenal use only

  sint32 NavigationType; // W1 'GVAR'
  sint32 ImcFlag;        // W2 IMC flag (0-active, 1-inactive)
  sint32 W3_5[3];        // W3-5 Not used
  sint32 RefLongitude;   // W6 Ref Longitude (rad * 10^7 )
  sint32 RefNomDist;     // W7 Ref distance from nominal (km *10^7)
  sint32 RefLatitude;    // W8 Ref Latitude  (rad * 10^7 )
  sint32 RefYaw;         // W9 Ref Yaw       (rad * 10^7 )
  sint32 RefAttitudeRoll;// W10 Ref  attitude roll (rad * 10^7 )
  sint32 RefAttitudePitch; // W11 Ref attitude pitch (rad * 10^7 )
  sint32 RefAttitudeYaw; // W12 Ref attitude yaw (rad * 10^7 )
  sint32 EpochTimeDate[2];  // W13-14 Epoch time data BCD format
  sint32 DeltafromEpochTime; // W14 (minutes * 100 )
  sint32 IMCroll;       // W15 img motion comp roll (rad * 10^7)
  sint32 IMCpitch;      // W16 img motion comp pitch (rad * 10^7)
  sint32 IMCyaw;        // W18 img motion comp yaw (rad * 10^7)
  sint32 ChangeLongitude[13] ;//W19-31 long delta from ref (rad*10^7)
  sint32 ChangeRadialDist[11];//W32-42 r dist delta from ref (rad*10^7)
  sint32 SineGeoCentricLat[9];//W43-51 (nounits *10^7)
  sint32 SineOrbitYaw[9]; //W52-60 (nounits * 10^7)
  sint32 DailySolarRate;  //W61 (rad/min * 10^7)
  sint32 ExpStartFromEpoch; // W62 (min * 100 )
  sint32 RollAttitudeAngle[55]; //W63-117
  sint32 W127_118[10];      // W127-118
  sint32 W128; // W128 'MORE'
  sint32 W129; // W129 'GVAR'
  sint32 PitchAngle[55]; // W130_184
  sint32 YawAngle[55];   // W185_239
  sint32 W240_255[16];   // W240-255
  sint32 W256;           // W256 'MORE'
  sint32 W257;           // W257 'GVAR'
  sint32 RollMisalignmentAngle[55]; // W258-312
  sint32 PitchMisalignmentAngle[55]; // W313-367
  sint32 InstrumentFlag; //W368 (1=imager; 2 = sounder)
  sint32 W369_YYDDD; //W369
  sint32 W370_HHMMSS; //W370
  sint32 W371_383[13]; //W371-383 
  sint32 W384; //W384 'MORE'
  sint32 W385; //W385 'GVAR'
  sint32 W386_510[125]; // W386-510
  sint32 W511; // W511 'MORE'
  sint32 W512; // W512 'GVAR'
  sint32 W513_640[128]; //W513-640

  float VisibleBias[8]; // W1-8
  float VisibleFirstOrderGain[8]; //W9-16
  float VisibleSecOrderGain[8];   //W17-24
  float VisibleRadianceToAlbedo; //W25
  float Side1IRBias[4]; // W26-29
  float Side2IRBias[4]; // W30-33
  float Side1IRGain[4]; // W34-37
  float Side2IRGain[4]; // W38-41
  sint32 W128_42[87]; 

};

struct McIDAS_LineDoc{ 
  sint32 ValidityCode;
  uint16 BlockHeaderCRC;
  sint32 ScanStatus;
  uint32 Time[2];
  uchar8 BlockHeader[30];
  uchar8 LineDoc[32];
};
