#ifndef BIASSTATISTICS_H
#define BIASSTATISTICS_H

#include "include/gvar.h"
#include "include/types.h"
#include "block0/clampMode.h"
#include "utils/selFloat/selFloat.h"
#include "utils/cdaTime/cdaTime.h"

/* goes i-m specs; pg 77 */

class BiasStatistics{ 
public:
  CdaTime tod_of_bias_statistics ; /* 5707 5714*/

  uint16 total_sample_size[7]; /*5715 5728 */
  uint16 filtered_sample_size[7]; /*5729 5742 */

  uint16 unfiltered_min_value[7]; /* 5743 5756 */
  uint16 filtered_min_value[7]; /* 5757 5770 */
  uint16 unfiltered_max_value[7]; /* 5771 5784 */
  uint16 filtered_max_value[7]; /* 5785 5798 */

  SelFloat  unfiltered_mean_value[7]; /* 5799 5826 */
  SelFloat  filtered_mean_value[7]; /* 5787 5854 */
  SelFloat  unfiltered_standard_deviation[7]; /* 5855 5882 */

  SelFloat  filtered_sigma_counts[7]; /* 5883 5910 */
  SelFloat  filtered_sigma_radiance[7]; /* 5911 5938 */
  SelFloat  filtered_sigma_temperature[7]; /* 5939 5966 */

  ClampMode clamp_mode ; /* 5967 5970 */
public:
  BiasStatistics(){} ;
  void toIEEE();
  void toMcIDAS();
  void ByteSwap(void);
}; 


#endif // BIASSTAISTICS_H

