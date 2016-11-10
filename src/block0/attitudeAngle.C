#include <iomanip>
#include "attitudeAngle.h"

Polar::Polar() { } 

MonomialSinusoid::MonomialSinusoid(){ }

void MonomialSinusoid::toNoaaNavigation(double * in){
  in[0] = (double) Order_of_applicable;
  in[1] = (double) Order_of;
  in[2] = (double) Sinusoid.Ph;
  in[3] = (double) Sinusoid.Mg;
  in[4] = (double) Angle_of_epoch_where_monomial_is_zero;
}

void MonomialSinusoid::toMcIDASNavigation(sint32 * in){
  in[0] = (long)Order_of_applicable;
  in[1] = (long)Order_of;
  in[2] = (int)((float) Sinusoid.Ph * 1E7) ;
  in[3] = (int)((float) Sinusoid.Mg * 1E7);
  in[4] = (int)((float) Angle_of_epoch_where_monomial_is_zero * 1E7);
}

AttitudeAngle::AttitudeAngle() { } 

void AttitudeAngle::toNoaaNavigation(double * in) 
{ 
  in[0] = (double) Exponential_magnitude;
  in[1] = (double) Exponential_time_constant;
  in[2] = (double) Constant_mean_attitude_angle;
  in[3] = (double) Number_of_sinusoidals_per_angle; /* 535  538*/ 
  for(int i = 0; i < 15; i++) {
    in[4+2*i] = (double) Sinusoid[i].Mg;
    in[2+2*i+1] = (double) Sinusoid[i].Ph;
  }
  in[33]  =  (double) Number_of_monomial_sinusoids; /* 659  662*/ 
  Monomial[0].toNoaaNavigation(in+34);
  Monomial[1].toNoaaNavigation(in+39);
  Monomial[2].toNoaaNavigation(in+44);
  Monomial[3].toNoaaNavigation(in+49);  
}	  

void AttitudeAngle::toMcIDASNavigation(sint32 * in) 
{ 
  in[0] = (int)((float) Exponential_magnitude * 1E7);
  in[1] = (int)((float) Exponential_time_constant * 100);
  in[2] = (int)((float) Constant_mean_attitude_angle * 1E7);
  in[3] = (long)Number_of_sinusoidals_per_angle; /* 535  538*/ 
  for(int i = 0; i < 15; i++) {
    in[4+2*i] = (int)((float) Sinusoid[i].Mg * 1E7);
    in[2+2*i+1] = (int)((float) Sinusoid[i].Ph * 1E7);
  }
  in[33]  =  (long)Number_of_monomial_sinusoids; /* 659  662*/ 
  Monomial[0].toMcIDASNavigation(in+34);
  Monomial[1].toMcIDASNavigation(in+39);
  Monomial[2].toMcIDASNavigation(in+44);
  Monomial[3].toMcIDASNavigation(in+49);  
}	  



ostream& operator << (ostream & out, Polar & p){
  out << std::setw(24) << (double)p.Mg  << "Magnitude\n";
  out << std::setw(24) << (double)p.Ph  << "Phase\n";
  return(out);
}

ostream& operator << (ostream & out, MonomialSinusoid & m){
  out << std::setw(24) << m.Order_of_applicable  << "Order of Applicable\n";
  out << std::setw(24) << m.Order_of             << "Order of\n";
  out << m.Sinusoid;
  out << std::setw(24) << (double)m.Angle_of_epoch_where_monomial_is_zero << "Angle of epoch where monomial is zero\n";
  return(out);
}

ostream& operator << (ostream & out, AttitudeAngle & a){
  out << std::setw(24) << (double)a.Exponential_magnitude         << "Exponential Magnitude\n";
  out << std::setw(24) << (double)a.Exponential_time_constant     << "Exponential Time Constant\n";
  out << std::setw(24) << (double)a.Constant_mean_attitude_angle  << "Constant Mean Attitude Angle\n";
  out << std::setw(24) << a.Number_of_sinusoidals_per_angle       << "Number of Sinusoidals per Angle\n";
  for ( int i = 0; i < 15; i++)
    out << a.Sinusoid[i];
  out << std::setw(24) << a.Number_of_monomial_sinusoids          << "Number of Monomial Minusoids\n";
  for (int j = 0; j < 4 ; j++)
   out << a.Monomial[j];
  return(out);
}


void Polar::ByteSwap(void)
{
  Mg.ByteSwap();
  Ph.ByteSwap();
}

void MonomialSinusoid::ByteSwap(void)
{
  Order_of_applicable = ((Order_of_applicable&0xff000000)>>24)|
                        ((Order_of_applicable&0x00ff0000)>>8)|
                        ((Order_of_applicable&0x0000ff00)<<8)|
                        ((Order_of_applicable&0x000000ff)<<24);
  Order_of = ((Order_of&0xff000000)>>24)|
             ((Order_of&0x00ff0000)>>8)|
             ((Order_of&0x0000ff00)<<8)|
             ((Order_of&0x000000ff)<<24);
  Sinusoid.ByteSwap();
  Angle_of_epoch_where_monomial_is_zero.ByteSwap();
}

void AttitudeAngle::ByteSwap(void)
{
  int i;
  Exponential_magnitude.ByteSwap();
  Exponential_time_constant.ByteSwap();
  Constant_mean_attitude_angle.ByteSwap();
  Number_of_sinusoidals_per_angle =
    ((Number_of_sinusoidals_per_angle&0xff000000)>>24)|
    ((Number_of_sinusoidals_per_angle&0x00ff0000)>>8)|
    ((Number_of_sinusoidals_per_angle&0x0000ff00)<<8)|
    ((Number_of_sinusoidals_per_angle&0x000000ff)<<24);
  for (i=0; i<15; i++)
    Sinusoid[i].ByteSwap();
  Number_of_monomial_sinusoids =
    ((Number_of_monomial_sinusoids&0xff000000)>>24)|
    ((Number_of_monomial_sinusoids&0x00ff0000)>>8)|
    ((Number_of_monomial_sinusoids&0x0000ff00)<<8)|
    ((Number_of_monomial_sinusoids&0x000000ff)<<24);
  for (i=0; i<4; i++)
    Monomial[i].ByteSwap();
}













