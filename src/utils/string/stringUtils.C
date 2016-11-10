#include "stringUtils.h"
#include <string.h>
// ROUTINES FOR fileName manupilation 

char * baseName(char * in ){ 
  char * tmp = strrchr(in, '/');
  if(tmp) tmp++ ;
  else tmp = in; 
  return(tmp);
}

char * dirName(char * in ){ 
  char * tmp  = strrchr(in, '/');
  if (tmp) { *tmp = '\0'; }
  else tmp = "./";
  return(tmp);
}

