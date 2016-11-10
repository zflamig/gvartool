#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "utils/selFloat/selFloat.h"


// 7367  - 8014 on block0 and block 11
class Calibration{ 
private:
  SelFloat L1[6]; /* 7367 7390 */
  SelFloat L2[6] ;   /* 7391 7414 */
  SelFloat BP1[6][6] ; /* 7415 7558 */
  SelFloat BB[8][6] ; /* 7559 7750 */
  SelFloat OP[9][6] ; /* 7751 7966 */
  SelFloat PA[6]   ; /* 7967 7990 */
  SelFloat PB[6];    /* 7991 8014 */
public:
  Calibration() { };
  void toIEEE();
  void toMcIDAS();
  void ByteSwap(void);
  ostream & print(ostream & ); 
  
};

#endif // IRCALIBRATION_H
