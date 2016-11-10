#ifndef ISCAN_H
#define ISCAN_H

#include "include/types.h"
//#include <stream.h>
#include <iostream>
using std::ostream;

class ISCAN{ 
 private:
	uint32 iscan; 
 public: 
	ISCAN(){ } ;
	ISCAN(unsigned int in ); 
	int bitSet(int bit);
	int newFrame() ; 
	int Frame_start() ; 
	int Frame_end() ; 
	int prior1();
	int prior2();
	int priority();
	int imcStatus();
	int flip();
	ostream & print(ostream & ); 
};

#endif // ISCAN_H
