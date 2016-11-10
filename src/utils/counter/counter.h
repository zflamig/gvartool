#ifndef COUNTER_H
#define COUNTER_H

#include <stdio.h>
#include <string.h>

#define COUNTER_ELEMENTS 9 

static char counter[] = {'|','\b','/','\b','-','\b','\\','\b','\0'};
static int ticks = 0; 
static int eraseChar=0;

inline void StopWatch(float f  ) { 
  fprintf(stderr, "\b\b\b\b\b\b%6.2f",f);
}

inline void StopWatch() { 
  fprintf(stderr, "%c", counter[++ticks%COUNTER_ELEMENTS]);
}

inline void StopWatch(int i ){ 
	fprintf(stderr, "\b\b\b\b%4i",i);
}

void StopWatch(char * s ){ 
  for (int i = 0 ; i < eraseChar; i++)
    fprintf(stderr, "\b");
  eraseChar = strlen(s);
  fprintf(stderr, "%s", s);  
}
    
#endif // COUNTER_H



