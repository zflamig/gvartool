
#ifndef BITFIELD_H
#define BITFIELD_H

//#include <stream.h>
#include <iostream>
using std::ostream;

class Bitfield{ 
 private: 
	int bits;
 public:

	Bitfield() { } ; 

	Bitfield( const unsigned short & in );
	Bitfield( const unsigned int & in );
	Bitfield( const  int & in );
	Bitfield( const unsigned long & in );
	Bitfield( const  long & in );
	int status( const int & in ); 
	int check(); 
	void print(ostream & out ); 
	
};

inline	Bitfield::Bitfield( const unsigned short & in ) 
{ bits =  (int) in ; }

inline	Bitfield::Bitfield( const unsigned int & in ) 
{ bits =  (int) in ; }

inline	Bitfield::Bitfield( const  int & in )
{ bits =  (int) in ; }

inline	Bitfield::Bitfield( const unsigned long & in )
{ bits =  (int) in ; }

inline	Bitfield::Bitfield( const  long & in ) 
{ bits =  (int) in ; }

#endif // BITFIELD_H	
