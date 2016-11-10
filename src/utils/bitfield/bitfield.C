//#include <stream.h>
#include <iostream>
using std::ostream;

#include "bitfield.h"

int Bitfield::status(const  int & in ) { 
  if ( (bits|(1<<(31-in))) == bits ) return( 1 ) ; 
  else return(0); 
}


void Bitfield::print(ostream & out) { 
	out << " Bit #      \n" ; 
	for ( int i = 0; i < 32 ; i++)
		out << i << " " << status( i ) << "\n" ;  
}


