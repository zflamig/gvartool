#ifndef SSCAN_H
#define SSCAN_H

#include "include/types.h"
//#include <stream.h>
#include <iostream>

class SSCAN{ 
 private:
	uint16 sscan; 
 public: 
	SSCAN(){ } ;
	SSCAN(unsigned int in ); 
	int bitSet(int bit);
	int newFrame() ; 
	int prior1();
	int prior2();
	int priority();
	int imcStatus();
	std::ostream & print(std::ostream & ); 
};

#endif // SSCAN_H
