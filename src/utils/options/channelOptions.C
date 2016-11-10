#include "channelOptions.h"

ChannelOptions::ChannelOptions(int channels )
{ 
  options = new CoptionsStruct [channels];
  for ( int c = 0; c < channels; c++)
    defaults(options[c]);
}

ChannelOptions::~ChannelOptions()
{ delete options; }
void ChannelOptions::defaults(CoptionsStruct & opt)
{ 
  opt.WordType		= Undef; 
  opt.Units		= Counts; 

  opt.Xstart		=  -1; 
  opt.Xend		=  -1; 
  opt.Xscale		= 1.0; 

  opt.Ystart		=  -1;
  opt.Yend		=  -1;
  opt.Yscale		= 1.0;


  opt.AlignLine		= 0;
  opt.AlignLineValue	= 0;
  opt.AlignPixel	= 0;
  opt.AlignPixelValue	= 0;

  opt.Gain		= 1;
  opt.Bias		= 0;
  opt.Gamma             = 1;

  opt.Map		= grid0;
  opt.MapVal		= 0;

  opt.GmtHoursStart	=  0;
  opt.GmtHoursStop	= 23;

  opt.Directory		= "./" ;
  opt.Area              = 0;
  opt.Navigation        = 0;
}



