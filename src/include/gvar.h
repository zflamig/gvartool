#ifndef GVAR_H
#define GVAR_H

#include <stdio.h>

#ifndef BUFSIZ 
#define BUFSIZ 4096
#endif

#ifndef FILENAME_SIZE
#define FILENAME_SIZE 1024
#endif

#ifndef DESC_STRING_SIZE
#define DESC_STRING_SIZE 4096
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define ALL -1

#define GVARBUFSIZ  BUFSIZ  
#define MAX_BLOCK_SZ  52348 

#define INSTRUMENTS 2

#define MAX_SECTORS 64
#define MAX_ARGS 3072

#define ICHANNELS  6 // max channel id - each imager has 5 channels, but channel ir6 exists on some
#define SCHANNELS 19
#define CHANNELS SCHANNELS

#define SSIDES 1
#define ISIDES 2
#define SIDES ISIDES

#define IDETECTORS 8 
#define SDETECTORS 4
#define DETECTORS IDETECTORS

#define IMAGERMAXCOUNTS ((1<<10)-1)
#define SOUNDERMAXCOUNTS ((1<<16)-1)


#define SPIXELS 11

static const int MAXCOUNTS[] = { IMAGERMAXCOUNTS, SOUNDERMAXCOUNTS};

#ifdef _MAIN_

// initialize Imager & sounder constants
// will be reset during program depending of which
// generation of GOES is being processed
int N_Channels[INSTRUMENTS]   = { ICHANNELS, SCHANNELS};
int max_channels[INSTRUMENTS] = { ICHANNELS, SCHANNELS};



//skip factors for channels
int VIS_PIX_PER_PIX[INSTRUMENTS][CHANNELS]    = {{1,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0},  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};
int VIS_LINES_PER_LINE[INSTRUMENTS][CHANNELS] = {{1,4,8,4,4,8,0,0,0,0,0,0,0,0,0,0,0,0,0},  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

int max_detectors[INSTRUMENTS][CHANNELS]      = {{8,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0},  {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4}};
int Side[INSTRUMENTS][CHANNELS]               = {{2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0},  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}}; 	

#else

extern int VIS_PIX_PER_PIX[INSTRUMENTS][CHANNELS];
extern int VIS_LINES_PER_LINE[INSTRUMENTS][CHANNELS];
extern int N_Channels[INSTRUMENTS];
extern int Side[INSTRUMENTS][CHANNELS];

#endif


//                       Imager   Sounder
//VISIBLE SCAN LINE NO.  1-15787   1-1582
//SCAN PIXEL NO.         1-30680   1-1758
inline int VALID(int a, int b) { return ((a>=0 && a<=30680)?(a):(b)) ; } 


#endif // gvar_h

