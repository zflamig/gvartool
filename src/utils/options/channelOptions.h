#ifndef CHANNEL_OPTIONS_H
#define CHANNEL_OPTIONS_H

#include "include/gvar.h"
#include "include/types.h"
#include <limits.h>

struct CoptionsStruct{ 
  WORD_TYPE WordType;
  RadiometricUnits Units; 
  double Gain, Bias, Gamma; 
  int Xstart, Ystart;
  int Xend, Yend;
  int Xsize, Ysize;
  int AlignLine, AlignLineValue;
  int AlignPixel, AlignPixelValue;
  double Xscale, Yscale; 
  GRIDSET Map;
  float Area;
  double  MapVal;
  int Navigation;
  int GmtHoursStart, GmtHoursStop; 
  char* Directory; 
};  
typedef struct CoptionsStruct CoptionsStruct; 

class ChannelOptions{ 
private:
  CoptionsStruct * options; 
  void defaults(CoptionsStruct & opt);

public:
  ChannelOptions(int channels = 0);
  virtual ~ChannelOptions();

  WORD_TYPE & wordType(int channel);
  RadiometricUnits & units(int channel);
  double & gain(int channel);
  double & bias(int channel);
  double & gamma(int channel);
  double & xscale(int channel);
  double & yscale(int channel);
  int & xstart(int channel);
  int & ystart(int channel);
  int & xend(int channel);
  int & yend(int channel);
  int & xsize(int channel);
  int & ysize(int channel);
  int & alignline(int channel);
  int & alignlinevalue(int channel);
  int & alignpixel(int channel);
  int & alignpixelvalue(int channel);
  GRIDSET & map(int channel);
  double & mapVal(int channel);
  int & gmtHoursStart(int channel);
  int & gmtHoursStop(int channel);
  char* directory(int channel);
  char* linkName(int channel);
  void setDirectory(int channel, char * dir);
  float & area (int channel);
  int & navigation (int channel);
};

inline WORD_TYPE & ChannelOptions::wordType(int channel)
{ return options[channel].WordType; }

inline RadiometricUnits & ChannelOptions::units(int channel)
{ return options[channel].Units; }

inline double & ChannelOptions:: gain(int channel)
{ return options[channel].Gain; }

inline double & ChannelOptions:: bias(int channel)
{ return options[channel].Bias; }

inline double & ChannelOptions:: gamma(int channel)
{ return options[channel].Gamma; }

inline double & ChannelOptions:: xscale(int channel)
{ return options[channel].Xscale; }

inline double & ChannelOptions:: yscale(int channel)
{ return options[channel].Yscale; }

inline int & ChannelOptions:: xstart(int channel)
{ return options[channel].Xstart; }

inline int & ChannelOptions:: ystart(int channel)
{ return options[channel].Ystart; }

inline int & ChannelOptions:: xend(int channel)
{ return options[channel].Xend; }

inline int & ChannelOptions:: yend(int channel)
{ return options[channel].Yend; }

inline int & ChannelOptions:: xsize(int channel)
{ return options[channel].Xsize; }

inline int & ChannelOptions:: ysize(int channel)
{ return options[channel].Ysize; }

inline int & ChannelOptions:: alignline(int channel)
{ return options[channel].AlignLine; }

inline int & ChannelOptions:: alignlinevalue(int channel)
{ return options[channel].AlignLineValue; }

inline int & ChannelOptions:: alignpixel(int channel)
{ return options[channel].AlignPixel; }

inline int & ChannelOptions:: alignpixelvalue(int channel)
{ return options[channel].AlignPixelValue; }

inline GRIDSET & ChannelOptions:: map(int channel)
{ return options[channel].Map; }

inline double & ChannelOptions:: mapVal(int channel)
{ return options[channel].MapVal; }

inline int & ChannelOptions:: gmtHoursStart(int channel)
{ return options[channel].GmtHoursStart; }

inline int & ChannelOptions:: gmtHoursStop(int channel)
{ return options[channel].GmtHoursStop; }

inline char* ChannelOptions::directory(int channel)
{ return options[channel].Directory; }

inline void ChannelOptions::setDirectory(int channel, char * dir)
{ options[channel].Directory = dir ;  }

inline float & ChannelOptions::area (int channel)
{ return options[channel].Area; }

inline int & ChannelOptions::navigation (int channel)
{ return options[channel].Navigation; }

#endif


