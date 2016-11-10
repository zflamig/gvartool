#ifndef TYPES_H
#define TYPES_H

#define SIZEOF_INT 4

typedef unsigned char uchar8 ;
typedef char schar8; 
typedef unsigned short uint16 ;
typedef short sint16;

#if defined(__alpha) || _MIPS_SZLONG == 64 ||  SIZEOF_INT == 4
typedef int sint32;
typedef unsigned int uint32;    /* sizeof (uint32) must == 4 */
#else
typedef long sint32; /* is this true in 64-bit i386 Linux ??? */
typedef unsigned long uint32;   /* sizeof (uint32) must == 4 */
#endif

typedef int fildes; 

enum IChannel{ I1 = 0,  I2 = 1,  I3 = 2,  I4 = 3,  I5 = 4, I6 = 5 } ;
#define IVIS I1

enum SChannel{
  S1=0,  S2=1,  S3=2,  S4=3,  S5=4,  S6=5,  S7=6,  S8=7,  S9=8,
  S10=9, S11=10,S12=11,S13=12,S14=13,S15=14,S16=15,S17=16,S18=17, 
  S19=18} ;  
#define SVIS S19
  
enum WORD_TYPE {
  Undef = 0,  Uchar = 1,  Char  = 2,  Ushort= 3,
  Short = 4,  Uint  = 5,  Int   = 6,  Float = 7,
  Double= 8} ;

static const int WordSize[] = {
  0,          1,          1,           2,
  2,          4,          4,           4,
  8 };

/*
static const char* WordString[] = { 
  "Undef", "Uchar", "char",  "Ushort",
  "short", "Uint",  "int",    "float",
  "double" };
*/

enum Instrument{
  imager=0,  sounder=1,  none};

enum RadiometricUnits{
  Nada,  Counts,  Radiance,  Tstar,   Tscene, 
  ModeA,  Albedo,  Albedo2 };

enum FILE_TYPE{
  noFile,  binFile,  tiffLine,  tiffTile,  tiffStrip,
  hdfFile,   jpegFile,  mcidasFile };

enum TiffFileMode{
  line,  strip,   tile};

enum GRIDSET{
  grid0 = 0,  grid1 = 1, 
  grid2 = 2,  grid12 = 3};

enum DebugMode{
  Debug0=0,    Dheader,  Dlinedoc, Dframe,
  Dblock0time, Dsadid,   Dcrc, Dparity , Ddump_all };


#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define ABS(x) ((x>0)?(x):-(x)) 

#endif // TYPES_H




