#include "sectorOptions.h"

SectorOptions::SectorOptions(int channels)
:ChannelOptions(channels) 
{
  Channels = channels;
  defaults(); 
}

SectorOptions::~SectorOptions()
{
}

void SectorOptions::defaults()
{ 
  TimeStamp  = 0;
  Compress   = 0;
  LinkLatest = 0;

  FileType   = tiffLine;

  cLatitude()  = 9999;
  cLongitude() = 9999;
}
