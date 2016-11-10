#include "radiometric.h"
//#include <iostream.h>
#include <iostream>
#include <limits.h>

#ifndef DBL_MIN
#define DBL_MIN -1E-20
#endif

#ifndef FLT_MIN
#define FLT_MIN -1E-20
#endif

#ifndef DBL_MAX
#define DBL_MAX 1E37
#endif

#ifndef FLT_MAX
#define FLT_MAX 1E37
#endif

Radiometric::Radiometric() 
{
  Table = NULL; 
  Units = Counts; 
  setWordType() ; 
  setScalingMandC(); 
  set_coeff_a_and_b(); 
  setUserMandCG(); 
}

Radiometric::~Radiometric() 
{
  std::cout << "Radiometric Destructor\n" << std::flush;
  if(Table) delete (char *)Table;
}

double Radiometric::MODEA(unsigned short kounts){ 
  double modea;
  double val = SCENE_TEMPERATURE(kounts);
  if ( val < 242) modea = 418 -   MAX(val,163);
  else            modea = 660 - 2*MIN(val,330) ;
  return(modea);
}

void * Radiometric::convert(unsigned short kounts){ 
  memset(& Cnvrt, 0, 4);
  CopyToTable(0, getRadiometric(kounts), & Cnvrt) ; 
  return(& Cnvrt); 
}

double  Radiometric::getRadiometric(unsigned short kounts) {
  double  val ;

  switch( Units ){
  case Radiance:  
    val = USERVAL(RADIANCE(kounts));
    break; 
  case Tscene: 
    val = USERVAL(SCENE_TEMPERATURE(kounts));
    break;
  case Tstar:
    val = USERVAL(BRIGHTNESS_TEMPERATURE(kounts));
    break; 
  case Albedo: 
    val = USERVAL(ALBEDO(kounts));
    break; 
  case Albedo2: 
    val = USERVAL(ALBEDO(kounts));
    break; 
  case ModeA:
    val = USERVAL(MODEA(kounts));
    break;
  default:
    val = USERVAL((short)(kounts)); 
    break;
  }

  return (val) ; 
}


void Radiometric::CreateLookupTable(){
  int tableSz = (1 << MAXDATABITS) +1 ; 

  delete (char *)Table;
  Table = new char [tableSz* WordSize[WordType] ];
  
  for(short i = 0; i < tableSz; i++)
    CopyToTable(i, getRadiometric(i), Table);

  CopyToTable((short) tableSz-1, MapDot, Table);
}


void Radiometric::CopyToTable(int i, double Data , void * tbl ) { 
  switch (WordType ) { 
  case Char : 
   *((char           *) tbl + i) =  (char          )MAX( MIN( Data, SCHAR_MAX*1.0), SCHAR_MIN*1.0);    break;
  case Uchar: 
   *((unsigned char  *) tbl + i) =  (unsigned char )MAX( MIN( Data, UCHAR_MAX*1.0),           0.0);    break;
  case Short: 
   *((short          *) tbl + i) =  (short         )MAX( MIN( Data,  SHRT_MAX*1.0),  SHRT_MIN*1.0);    break;
  case Ushort:
   *((unsigned short *) tbl + i) =  (unsigned short)MAX( MIN( Data, USHRT_MAX*1.0),           0.0);    break;
  case Int:   
   *((int            *) tbl + i) =  (int           )MAX( MIN( Data,   INT_MAX*1.0),   INT_MIN*1.0);    break;
  case Uint:  
   *((unsigned int   *) tbl + i) =  (unsigned int  )MAX( MIN( Data,  UINT_MAX*1.0),             0);    break;
  case Float: 
   *((float          *) tbl + i) =  (float         )         (Data);                                   break;
  case Double:
   *((double         *) tbl + i) =                            Data;                                    break;
  default:
    break;
  }
}


void * Radiometric::convert(unsigned short *kounts, int size, void *buf)
{
  
  if(Units == Counts && 
     (WordType == Short || WordType == Ushort) &&
     UserM == 1.0 &&
     UserC ==  0.0 && 
     UserG == 1.0 ) 
    memcpy(buf,kounts,size*WordSize[WordType]);
  
  else{ 
    if (!Table) CreateLookupTable();
    switch(WordType){ 
      
    case Char:{ 
      register char * table = ( char *) Table;
      register char * convertedData = (char *) buf; 
      for(int i=0; i < size; i++)  
	*convertedData++ = *(table+(*kounts++));
    }
      break;
      
    case Uchar:{ 
      register unsigned char * table = ( unsigned char * ) Table; 
      register unsigned char * convertedData  =
	(unsigned char * ) buf; 
      for ( int i = 0; i < size; i++ )  
	*convertedData++ = *(table+(*kounts++));
    }
      break;
      
    case Short:{ 
      register short * table = ( short * ) Table; 
      register short * convertedData  = ( short * ) buf; 
      for ( int i = 0; i < size; i++ )  
	*convertedData++ = *(table+(*kounts++));
    }
      break;

    case Ushort:{ 
      register unsigned short * table = ( unsigned short * ) Table; 
      register unsigned short * convertedData  = 
	( unsigned short * ) buf; 
      for ( int i = 0; i < size; i++ )  
	*convertedData++ = *(table+(*kounts++));
    }
      break;

    case Int:{ 
      register int * table = ( int * ) Table; 
      register int * convertedData  = (int * ) buf; 
      for ( int i = 0; i < size; i++ )  
	*convertedData++ = *(table+(*kounts++));
    }
      break;

    case Uint:{ 
      register unsigned int * table = ( unsigned int * ) Table; 
      register unsigned int * convertedData  = (unsigned int * ) buf; 
      for ( int i = 0; i < size; i++ )  
	*convertedData++ = *(table+(*kounts++));
    }
      break;

    case Float:{ 
      register float * table = ( float *) Table;
      register float * convertedData  = (float * ) buf; 
      for ( int i = 0; i < size; i++ )  
	*convertedData++ = *(table+(*kounts++));
    }
      break;

    case Double:{
      register double * table = (double *) Table;
      register double * convertedData  = (double * ) buf; 
      for ( int i = 0; i < size; i++ )  
	*convertedData++ = *(table+(*kounts++));
    }
      break;

    default:
      break;
    }
  }
  return (buf); 
}

double Radiometric::getConverted(unsigned short kount){ 

  double ret = getRadiometric(kount);

  switch ( WordType ) {  
  case Char: 
    ret = (char) MIN(SCHAR_MAX,MIN(ret,SCHAR_MIN));
    break;
  case Uchar:
    ret = (unsigned char) MIN(UCHAR_MAX,MAX(ret,0));
    break;
  case Short: 
    ret = (short) MIN(SHRT_MAX,MAX(ret,SHRT_MIN));
    break;
  case Ushort: 
    ret = (unsigned short) MIN(USHRT_MAX,MAX(ret,0));
    break;
  case Int:
    ret = (int) MIN(INT_MAX,MAX(INT_MIN,ret));
    break;
  case Uint:
    ret = (unsigned int ) MIN(UINT_MAX,MAX(ret,0));
    break;
  case Float:
    ret = (float) ret;
    break;
  case Double:
    ret = ret;
    break;
  default:
    break;
  }
  return (ret); 
}


double Radiometric::getMin(){ 

  double ret = 0;

  switch ( WordType ) {  
  case Char: 
    ret = SCHAR_MIN;
    break;
  case Uchar:
    ret = 0;
    break;
  case Short: 
    ret = SHRT_MIN;
    break;
  case Ushort: 
    ret = 0;
    break;
  case Int:
    ret = INT_MIN;
    break;
  case Uint:
    ret = 0;
    break;
  case Float:
    ret = FLT_MIN;
    break;
  case Double:
    ret = DBL_MIN;
    break;
  default:
    break;
  }
  return (ret); 
}

double Radiometric::getMax(){ 

  double ret = 0;

  switch ( WordType ) {  
  case Char: 
    ret = SCHAR_MAX;
    break;
  case Uchar:
    ret = UCHAR_MAX;
    break;
  case Short: 
    ret = SHRT_MAX;
    break;
  case Ushort: 
    ret = USHRT_MAX;
    break;
  case Int:
    ret = INT_MAX;
    break;
  case Uint:
    ret = UINT_MAX;
    break;
  case Float:
    ret = FLT_MAX;
    break;
  case Double:
    ret = DBL_MAX;
    break;
  default:
    break;
  }
  return (ret); 
}




