#ifndef SOUNDERDATA_H
#define SOUNDERDATA_H

#include <string.h>
#include "utils/selFloat/selFloat.h"
#include "block11/sadId.h"

#define SOUNDER_CHANNELS 19
#define SOUNDER_DETECTORS 4 
#define SOUNDER_PIXELS 11 

struct InstrumentDataRecord{ 
	char DataRecord[500];
	uint32 Status; 
	SelFloat AimLatitude;
	SelFloat AimLongitude; 
	SelFloat Ch8LatLon[2][SOUNDER_DETECTORS];
};

class SounderData{ 
 private:
	struct InstrumentDataRecord IDR[SOUNDER_PIXELS];
	
	uint16 LineNo[SOUNDER_DETECTORS];// 6015-6016
	uint16 PixelNo[SOUNDER_PIXELS]; // 6023-6044
	char Sloss; // 6045;
	char Srestore; // 6046
	char spare[322]; // 6047-6368
		
	uint16 Data[SOUNDER_CHANNELS][SOUNDER_DETECTORS][SOUNDER_PIXELS]; 
	uint16 crc; 

public: 
	SounderData(){ } ;
	//void copy( SadId * sad );  
	uint16 lineNo(int detector);
	uint16 pixelNo(int p );
	uint16 data(int channel, int detector, int pixNumber);
	//float aimLat(int pixNumber);
	//float aimLon(int pixNumber);
        //void ByteSwap(void);
};

inline uint16 SounderData::lineNo(int detector)
{
#ifdef _LSBF_
  return (((LineNo[detector]&0xff00)>>8)|
          ((LineNo[detector]&0x00ff)<<8));
#else
  return LineNo[detector];
#endif
} 


inline uint16 SounderData::pixelNo(int p )
{
#ifdef _LSBF_
  return (((PixelNo[p]&0xff00)>>8)|
          ((PixelNo[p]&0x00ff)<<8));
#else
  return PixelNo[p];
#endif
}

inline unsigned short 
 SounderData::data(int channel, int detector, int pixNumber) 
{
#ifdef _LSBF_
  return (((Data[channel][detector][pixNumber]&0xff00)>>8)|
          ((Data[channel][detector][pixNumber]&0x00ff)<<8));
#else
  return Data[channel][detector][pixNumber];
#endif
}

/*inline float 
 SounderData::ch8Lat(int detector, int pixelNumber) 
{return (float) IDR[pixelNumber].Ch8LatLon[0][detector]; }

inline	float
SounderData::ch8Lon(int detector, int pixelNumber) 
{return (float) IDR[pixelNumber].Ch8LatLon[1][detector]; }


inline void SounderData::copy(SadId * sad) 
{memcpy(this, sad->data() , sizeof(SounderData) ) ;}
*/

#endif // SOUNDERDATA_H







