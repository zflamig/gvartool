//#include <stream.h>
#include <iostream>
#include <iomanip>
using std::ostream;

#include <string.h>
//#include <strstream.h>
#include <time.h>

#include "cdaTime.h"

CdaTime::CdaTime(int * in){ 
  memcpy(this, in, sizeof(CdaTime));
}

ostream& operator << (ostream & out,  CdaTime & t)
{ 
  struct tm time_str; 
  memset(&time_str, 0, sizeof(time_str));

  time_str.tm_mday = t.day();

  // was: year 2000 becomes 100
  time_str.tm_year = (t.year()-1900 );
  // now: year 2000 becomes 00
  //time_str.tm_year = (t.year()%100);

  mktime(&time_str);
  
  out.unsetf(std::ios::left);
  out.setf(std::ios::right);
  out << std::setfill('0');

  out << std::setw(2) << time_str.tm_year%100;
  out << std::setw(2) << time_str.tm_mon+1;
  out << std::setw(2) << time_str.tm_mday;
  out << std::setw(2) << t.hrs() ;
  out << std::setw(2) << t.min();
  
  out << std::setfill(' ');
  out.unsetf(std::ios::right);
  out.setf(std::ios::left);

  return (out) ; }


void CdaTime::timestamp( ostream& out ){ 
  struct tm time_str; 
  memset(&time_str, 0, sizeof(time_str));

  time_str.tm_mday = day();
  time_str.tm_year = (year()-1900 );

  mktime(&time_str);
  

  out.unsetf(std::ios::left);
  out.setf(std::ios::right);
  out << std::setfill('0');

  out << std::setw(4) << year(); 
  out << std::setw(2) << time_str.tm_mon+1;
  out << std::setw(2) << time_str.tm_mday;
  out << std::setw(1) << '_'; 
  out << std::setw(2) << hrs(); 
  out << std::setw(2) << min();
  out << std::setw(2) << sec();

  out << std::setfill(' ');
  out.unsetf(std::ios::right);
  out.setf(std::ios::left);

}

void CdaTime::print( ostream& out ){ 

  out.unsetf(std::ios::left);
  out.setf(std::ios::right);
  out << std::setfill('0');

  out << std::setw(4) << year() <<"\t"; 
  out << std::setw(3) << day() << "\t";
  out << std::setw(2) << hrs() << "\t"; 
  out << std::setw(2) << min() << "\t";
  out << std::setw(2) << sec() << "\t";
  out << std::setw(3) << msec() << "\t";

  out << std::setfill(' ');
  out.unsetf(std::ios::right);
  out.setf(std::ios::left);

  out << time_code_generator_state();

}

long int CdaTime::TimDifSec(CdaTime & t2){
  return  ((year() - t2.year())* (365 * 24 * 60 * 60 ) +
	   (day()  - t2.day() )* (      24 * 60 * 60 ) +
	   (hrs()  - t2.hrs() )* (           60 * 60 ) +
	   (min()  - t2.min() )* (                60 ) + 
	   (sec()  - t2.sec() ) 
	   );
}
    
void CdaTime::toIEEE(int* ieee){
  ieee[0] = ( 
	     (yr1000 * 1000 + yr100 * 100 + yr10 * 10 + yr1 ) * 10000
	     + (doy100 * 100 + doy10 * 10 + doy1 ) * 10
             + hr10 
             );

  ieee[1] = (
             hr1 * 10000000  
             + (min10*10 + min1 )* 100000 
             + (sec10 * 10 + sec1)* 1000 
             + msec100 * 100 + msec10*10 + msec1
	     );
}
      
       
    
