#ifndef SECTOR_OPTIONS_H
#define SECTOR_OPTIONS_H

#include "include/gvar.h"
#include "include/types.h"

#include "channelOptions.h"

class SectorOptions:public ChannelOptions{
private:
  int Channels;
  FILE_TYPE FileType;  

  int TimeStamp;
  int Compress; 
  int LinkLatest;
  void defaults();
  double CenterLatitude, CenterLongitude;

public:
  SectorOptions(int channels=1);
  ~SectorOptions();

  FILE_TYPE & fileType();

  int & timeStamp();
  int & compress();
  int & linkLatest();
  int channels() ;

  double & cLatitude(); 
  double & cLongitude();

};

inline FILE_TYPE & SectorOptions::fileType()
{ return FileType; }

inline int & SectorOptions::timeStamp()
{ return TimeStamp; }

inline int & SectorOptions::compress()
{ return Compress; }  

inline int SectorOptions::channels() 
{ return Channels; }  

inline int & SectorOptions::linkLatest()
{ return LinkLatest; }  

inline double & SectorOptions::cLatitude()
{ return CenterLatitude; }

inline double & SectorOptions::cLongitude()
{ return CenterLongitude; }


#endif
