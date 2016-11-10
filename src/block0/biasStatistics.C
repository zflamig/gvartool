#include "biasStatistics.h"

void BiasStatistics::ByteSwap(void)
{
  int i;

  /*CdaTime tod_of_bias_statistics ;  5707 5714*/

  for (i=0; i<7; i++)
  {
    total_sample_size[i]    = ((total_sample_size[i]&0xff00)>>8)|
                              ((total_sample_size[i]&0x00ff)<<8);
    filtered_sample_size[i] = ((filtered_sample_size[i]&0xff00)>>8)|
                              ((filtered_sample_size[i]&0x00ff)<<8);
    unfiltered_min_value[i] = ((unfiltered_min_value[i]&0xff00)>>8)|
                              ((unfiltered_min_value[i]&0x00ff)<<8);
    filtered_min_value[i]   = ((filtered_min_value[i]&0xff00)>>8)|
                              ((filtered_min_value[i]&0x00ff)<<8);
    unfiltered_max_value[i] = ((unfiltered_max_value[i]&0xff00)>>8)|
                              ((unfiltered_max_value[i]&0x00ff)<<8);
    filtered_max_value[i]   = ((filtered_max_value[i]&0xff00)>>8)|
                              ((filtered_max_value[i]&0x00ff)<<8);
    unfiltered_mean_value[i].ByteSwap();
    filtered_mean_value[i].ByteSwap();
    unfiltered_standard_deviation[i].ByteSwap();
    filtered_sigma_counts[i].ByteSwap();
    filtered_sigma_radiance[i].ByteSwap();
    filtered_sigma_temperature[i].ByteSwap();
  }

  clamp_mode.ByteSwap();
}

