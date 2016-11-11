/* Makes a test file into somthing suitable to be included
as a character string in a program */

#include <fstream>
#include <sys/types.h>
#include <string.h>

char backSlash = '\\';
char quote = '"';

char * baseName(char * in );
int main(int argc, char * argv[] ){ 
  
  char * in  = argv[1];
  char * out = argv[2];

  std::ifstream In(in);
  std::ofstream Out(out);

  Out << "const char * ";
  Out << baseName(in) ;
  Out << " = { " ;
  Out.put(quote);

  char c ; 
  while ( In.get(c)) { 
    if (c == '\n') { Out.put(backSlash); Out.put('n'); }
    else if (c==backSlash) { Out.put(backSlash); Out.put(backSlash);}
    else Out.put(c);
  }

  Out.put(backSlash); 
  Out.put('n');
  Out.put(quote);
  Out << "  }; \n" ; 

  return(0);
}

char * baseName(char * in ){ 
  char * tmp = strrchr(in, '/');
  if(tmp) tmp++ ;
  else tmp = in; 
  return(tmp);
}
