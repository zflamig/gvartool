#include "calibration.h"

void Calibration::ByteSwap(void)
{
  int i,j;
  for (i=0; i<6; i++)
  {
    L1[i].ByteSwap();
    L2[i].ByteSwap();
    PA[i].ByteSwap();
    PB[i].ByteSwap();
    for (j=0; j<6; j++)
      BP1[i][j].ByteSwap();
  }
  for (i=0; i<9; i++)
  {
    for (j=0; j<6; j++)
    {
      if (i<8)
        BB[i][j].ByteSwap();
      OP[i][j].ByteSwap();
    }
  }
}


ostream & Calibration::print(ostream & out ){ 

    for ( int i = 0; i < 6; i++) {
       out << "i     "     <<  i  << "\n";

       out << "L1       "  <<  L1[i]   << "\n";
       out << "L2       "  <<  L2[i]   << "\n";
       out << "BP1[6]   "  <<  BP1[0][i] <<"\t"<< BP1[1][i] <<"\t"<< BP1[2][i] <<"\t"<< BP1[3][i] << "\n";
       out << "BB[8]    "  <<  BB[0][i]  <<"\t"<< BB[1][i]  <<"\t"<< BB[2][i]  <<"\t"<< BB[3][i]  << "\n";
       out << "OP[9]    "  <<  OP[0][i]  <<"\t"<< OP[1][i]  <<"\t"<< OP[2][i]  <<"\t"<< OP[3][i]  << "\n";
       out << "PA       "  <<  PA[i]   << "\n";
       out << "PB       "  <<  PB[i]   << "\n";
    }

  return(out);
}




