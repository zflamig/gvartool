#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdlib.h>
#include <ctype.h>
//#include <stream.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <time.h>
#include <unistd.h>

#include <dirent.h>
#include <limits.h>

#include <tiff.h>
#include "include/types.h"

#include "include/gvar.h"
#include "utils/string/stringUtils.h"

#include "sectorOptions.h"
#include "channelOptions.h"

class Options{ 
private:
  SectorOptions* (*sector)[INSTRUMENTS];
  int ImagerSectors, SounderSectors;
  int ImagerSpacelookStats, ImagerIcse, SounderIcse;
  int Inav, Snav;
  int LookForNewFiles; 
  int Progress_Counter; 
  DebugMode Debug; 
  int Priority;
  char * Script;

  char **local_argv; 
  int local_argc;
  char ** snooze_dir_name;

  void printUsage ( char * argv0 ); 
  int FindNewerFile(time_t & time, char ** Dir, char * fname);
  void defaults();
  
  void getLinkName(char * name);
  time_t oldTime ; 

  char * Log;
  char * OutDir;
  char * NavDir;
  
public:
  Options();
  ~Options();
  int sectors(int i =ALL);
  int priority(void);

// sector options
  FILE_TYPE  & fileType(int f, int i);
  int  & timeStamp(int f, int i);
  int  & compress(int f, int i);
  int  & linkLatest(int f, int i);
  double & cLatitude(int f, int i);
  double & cLongitude(int f, int i );

//channel options

  int  & xstart(int f, int i, int c);
  int  & ystart(int f, int i, int c);
  int  & xend(int f, int i, int c);
  int  & yend(int f, int i, int c);
  int & xsize(int f, int i, int c);
  int & ysize(int f, int i, int c);
  int & alignline(int f, int i, int c);
  int & alignlinevalue(int f, int i, int c);
  int & alignpixel(int f, int i, int c);
  int & alignpixelvalue(int f, int i, int c);

  WORD_TYPE  & wordType(int f, int i, int c );
  RadiometricUnits  & units(int f, int i, int c );
  double  & gamma(int f, int i, int c);
  double  & gain(int f, int i, int c);
  double  & bias(int f, int i, int c);
  double  & xscale(int f, int i, int c);
  double  & yscale(int f, int i, int c);
  GRIDSET  & map(int f, int i, int c);
  double  & mapVal(int f, int i, int c );
  int & gmtHoursStart(int f, int i, int c);
  int & gmtHoursStop(int f, int i, int c);
  void setDirectory(int f, int i, int c , char * dir);
  char * directory(int f, int i, int c);
  float & area(int f, int i, int c );
  int & navigation(int f, int i, int c);

  int & imagerIcse();
  int & sounderIcse();
  int & imagerSpacelookStats(); 
  
  int ParseCommandLine(int & argc , char** & argv ); 
  int PrintCommandLine(int & argc , char** & argv ); 
  int & lookForNewFiles();
  char * nextFile(int argc, char** argv );

  int & progress_counter(); 
  DebugMode & debug(); 
  char * script(); 
  char * log(); 
  char * outdir(); 
  char * navdir(); 
  int nROIs(int i = -1 );
  int imagerNav();
  int sounderNav();

};

inline	Options::~Options() 
{ 
  if (local_argv) 
  {
    for (int i = local_argc-1; i>=0; i--)
      delete local_argv[i]; 
    delete local_argv;
    local_argv = NULL;
    local_argc = 0;
  } 
  delete [] sector;
  if (snooze_dir_name)
  {
    int j = 0; 
    while(snooze_dir_name[j])
    {
      delete snooze_dir_name[j];
      j++;
    }
    delete snooze_dir_name;
  }
}

inline int Options::priority(void)
{ return Priority; }

// sector options
inline FILE_TYPE & Options::fileType(int f, int i) 
{ return sector[f][i]->fileType(); }

inline	int  & Options::timeStamp(int f, int  i)
{ return sector[f][i]->timeStamp(); }

inline	int  & Options::compress(int f, int  i )
{ return sector[f][i]->compress(); }

inline	int  & Options::linkLatest(int f, int  i )
{ return sector[f][i]->linkLatest(); }

inline double & Options::cLatitude(int f, int i) 
{  return sector[f][i]->cLatitude(); }

inline double & Options::cLongitude(int f, int i )
{  return sector[f][i]->cLongitude(); }

//channel options

inline	WORD_TYPE & Options::wordType(int f, int i, int c )
{ return sector[f][i]->wordType(c); }

inline RadiometricUnits  & Options::units(int f, int i, int c )
{ return sector[f][i]->units(c); }

inline double  & Options::gamma(int f, int i, int c)
{ return sector[f][i]->gamma(c); }

inline double  & Options::gain(int f, int i, int c)
{ return sector[f][i]->gain(c); }

inline double  & Options::bias(int f, int i, int c)
{ return sector[f][i]->bias(c); }

inline double  & Options::xscale(int f, int i, int c)
{ return sector[f][i]->xscale(c); }

inline double  & Options::yscale(int f, int i, int c)
{ return sector[f][i]->yscale(c); }

inline int & Options::xstart(int f, int i, int c) 
{ return sector[f][i]->xstart(c) ; }

inline	int & Options::ystart(int f, int i, int c) 
{ return sector[f][i]->ystart(c); }

inline	int  & Options::xend(int f, int i, int c) 
{ return sector[f][i]->xend(c); }

inline int & Options::yend(int f, int i, int c)
{ return sector[f][i]->yend(c); }

inline int & Options::xsize(int f, int i, int c)
{ return sector[f][i]->xsize(c);}

inline int & Options::ysize(int f, int i, int c)
{ return sector[f][i]->ysize(c) ;}

inline int & Options::alignline(int f, int i, int c)
{ return sector[f][i]->alignline(c) ;}

inline int & Options::alignlinevalue(int f, int i, int c)
{ return sector[f][i]->alignlinevalue(c) ;}

inline int & Options::alignpixel(int f, int i, int c)
{ return sector[f][i]->alignpixel(c) ;}

inline int & Options::alignpixelvalue(int f, int i, int c)
{ return sector[f][i]->alignpixelvalue(c) ;}

inline GRIDSET  & Options::map(int f, int i, int c)
{ return sector[f][i]->map(c);}

inline double & Options::mapVal(int f, int i, int c )
{ return sector[f][i]->mapVal(c);}

inline int & Options::gmtHoursStart(int f, int i, int c)
{ return sector[f][i]->gmtHoursStart(c); }

inline int & Options::gmtHoursStop(int f, int i, int c)
{ return sector[f][i]->gmtHoursStop(c);}

inline float& Options::area(int f, int i, int c)
{ return sector[f][i]->area(c);}

inline int & Options::navigation(int f, int i, int c)
{ return sector[f][i]->navigation(c);}

inline void Options::setDirectory(int f, int i, int c , char * dir)
{ sector[f][i]->setDirectory(c,dir); }

inline char * Options::directory(int f, int i, int c)
{ return sector[f][i]->directory(c); }

inline	int & Options::imagerIcse() 
{ return ImagerIcse; }

inline	int & Options::imagerSpacelookStats() 
{ return ImagerSpacelookStats; }

inline	int & Options::sounderIcse() 
{ return SounderIcse; }

inline int & Options::lookForNewFiles() 
{ return LookForNewFiles; }		

inline int & Options::progress_counter() 
{ return Progress_Counter; }		

inline DebugMode & Options::debug() 
{ return Debug; } 

inline int Options::imagerNav(){ return Inav;}

inline int Options::sounderNav(){ return Snav; }

#endif // OPTIONS_H



