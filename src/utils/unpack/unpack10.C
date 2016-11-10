#ifndef UNPACK10_H
#define UNPACK10_H
/* 
   GOES data processing software 
   Disclaimer This software is provided AS IS.  
   Neither the author or NASA is responsible for any 
   damage incurred directly or indirectly 
   through the use of this software.

   Om Sharma, RDC, MAR 15 1995
   */

#include "include/types.h"
#include "unpack10.h"

#ifdef _MSBF_
void unpack10(void *in,     // pointer to packed data
              int outWords, // size of output buffer in words
              uint16 *Out)  // pointer to unpacked data
{
  struct TenBitWord
  { 
    unsigned c0 : 10; // byte 0 7654321076
    unsigned c1 : 10; // byte 1 5432107654
    unsigned c2 : 10; // byte 2 3210765432
    unsigned c3u: 2;  // byte 3 10
    
    unsigned c3l: 8;  // byte 4   76543210
    unsigned c4 : 10; // byte 5 7654321076
    unsigned c5 : 10; // byte 6 5432107654
    unsigned c6u: 4;  // byte 7 3210
    
    unsigned c6l: 6;  // byte 8     765432
    unsigned c7: 10;  // byte 9 1076543210
    unsigned c8: 10;  // byte10 7654321076
    unsigned c9u: 6;  // byte11 543210
    
    unsigned c9l: 4;  // byte12       7654
    unsigned c10: 10; // byte13 3210765432
    unsigned c11: 10; // byte14 1076543210
    unsigned c12u: 8; // byte15 76543210
    
    unsigned c12l: 2; // byte16         76
    unsigned c13: 10; // byte17 5432107654
    unsigned c14: 10; // byte18 3210765432
    unsigned c15: 10; // byte19 1076543210
  }; 

  struct TenBitWord *ten = (struct TenBitWord *)in ; 
  uint16* out = Out; 
  int reps, last = (outWords>>4), count = 0;
  
  for(reps=0; reps<=last; reps++,ten++)
  {
    if (reps!=last)
    {
      *out = (uint16)ten->c0; out++;
      *out = (uint16)ten->c1; out++;
      *out = (uint16)ten->c2; out++;
      *out = ((uint16)ten->c3u<<8)|(uint16)ten->c3l; out++;
      *out = (uint16)ten->c4; out++;
      *out = (uint16)ten->c5; out++;
      *out = ((uint16)ten->c6u<<6)|(uint16)ten->c6l ; out++;
      *out = (uint16)ten->c7; out++;
      *out = (uint16)ten->c8; out++;
      *out = ((uint16)ten->c9u<<4)|(uint16)ten->c9l ; out++;
      *out = (uint16)ten->c10; out++;
      *out = (uint16)ten->c11; out++;
      *out = ((uint16)ten->c12u<<2)|(uint16)ten->c12l; out++;
      *out = (uint16)ten->c13; out++;
      *out = (uint16)ten->c14; out++;
      *out = (uint16)ten->c15; out++;
      count += 16;
    }
    else
    { /* brute force to prevent overflow of output buffer */
      if (count<outWords)
        {*out = (uint16)ten->c0; out++; count++;}
      if (count<outWords)
        {*out = (uint16)ten->c1; out++; count++;}
      if (count<outWords)
        {*out = (uint16)ten->c2; out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c3u<<8)|(uint16)ten->c3l; out++; count++;}
      if (count<outWords)
        {*out = (uint16)ten->c4; out++; count++;}
      if (count<outWords)
        {*out = (uint16)ten->c5; out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c6u<<6)|(uint16)ten->c6l; out++; count++;}
      if (count<outWords)
        {*out = (uint16)ten->c7; out++; count++;}
      if (count<outWords)
        {*out = (uint16)ten->c8; out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c9u<<4)|(uint16)ten->c9l; out++; count++;}
      if (count<outWords)
        {*out = (uint16)ten->c10; out++; count++;}
      if (count<outWords)
        {*out = (uint16)ten->c11; out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c12u<<2)|(uint16)ten->c12l; out++; count++;}
      if (count<outWords)
        {*out = (uint16)ten->c13; out++; count++;}
      if (count<outWords)
        {*out = (uint16)ten->c14; out++; count++;}
      if (count<outWords)
        {*out = (uint16)ten->c15; out++; count++;}
    }
  }
}

#else //  _LSBF_

void unpack10 (void *in, int outWords, uint16 *Out)
{
  struct TenBitWord
  {
    unsigned c00u:8; // byte  0 76543210
    unsigned c01u:6; // byte  1 543210
    unsigned c00l:2; // byte  1         76
    unsigned c02u:4; // byte  2 3210
    unsigned c01l:4; // byte  2       7654
    unsigned c03u:2; // byte  3 10
    unsigned c02l:6; // byte  3     765432
    unsigned c03l:8; // byte  4   76543210

    unsigned c04u:8; // byte  5 76543210
    unsigned c05u:6; // byte  6 543210
    unsigned c04l:2; // byte  6         76
    unsigned c06u:4; // byte  7 3210
    unsigned c05l:4; // byte  7       7654
    unsigned c07u:2; // byte  8 10
    unsigned c06l:6; // byte  8     765432
    unsigned c07l:8; // byte  9   76543210

    unsigned c08u:8; // byte 10 76543210
    unsigned c09u:6; // byte 11 543210
    unsigned c08l:2; // byte 11         76
    unsigned c10u:4; // byte 12 3210
    unsigned c09l:4; // byte 12       7654
    unsigned c11u:2; // byte 13 10
    unsigned c10l:6; // byte 13     765432
    unsigned c11l:8; // byte 14   76543210

    unsigned c12u:8; // byte 15 76543210
    unsigned c13u:6; // byte 16 543210
    unsigned c12l:2; // byte 16         76
    unsigned c14u:4; // byte 17 3210
    unsigned c13l:4; // byte 17       7654
    unsigned c15u:2; // byte 18 10
    unsigned c14l:6; // byte 18     765432
    unsigned c15l:8; // byte 19   76543210
  } *ten = (struct TenBitWord *)in;
  uint16 *out = Out; 
  int reps, last = outWords>>4, count = 0;
  
  for(reps=0; reps<=last; reps++,ten++)
  {
    if (reps!=last)
    {
      *out = ((uint16)ten->c00u<<2)|((uint16)ten->c00l); out++;
      *out = ((uint16)ten->c01u<<4)|((uint16)ten->c01l); out++;
      *out = ((uint16)ten->c02u<<6)|((uint16)ten->c02l); out++;
      *out = ((uint16)ten->c03u<<8)|((uint16)ten->c03l); out++;
      *out = ((uint16)ten->c04u<<2)|((uint16)ten->c04l); out++;
      *out = ((uint16)ten->c05u<<4)|((uint16)ten->c05l); out++;
      *out = ((uint16)ten->c06u<<6)|((uint16)ten->c06l); out++;
      *out = ((uint16)ten->c07u<<8)|((uint16)ten->c07l); out++;
      *out = ((uint16)ten->c08u<<2)|((uint16)ten->c08l); out++;
      *out = ((uint16)ten->c09u<<4)|((uint16)ten->c09l); out++;
      *out = ((uint16)ten->c10u<<6)|((uint16)ten->c10l); out++;
      *out = ((uint16)ten->c11u<<8)|((uint16)ten->c11l); out++;
      *out = ((uint16)ten->c12u<<2)|((uint16)ten->c12l); out++;
      *out = ((uint16)ten->c13u<<4)|((uint16)ten->c13l); out++;
      *out = ((uint16)ten->c14u<<6)|((uint16)ten->c14l); out++;
      *out = ((uint16)ten->c15u<<8)|((uint16)ten->c15l); out++;
      count += 16;
    }
    else
    { /* brute force to prevent overflow of output buffer */
      if (count<outWords)
        {*out = ((uint16)ten->c00u<<2)|((uint16)ten->c00l); out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c01u<<4)|((uint16)ten->c01l); out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c02u<<6)|((uint16)ten->c02l); out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c03u<<8)|((uint16)ten->c03l); out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c04u<<2)|((uint16)ten->c04l); out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c05u<<4)|((uint16)ten->c05l); out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c06u<<6)|((uint16)ten->c06l); out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c07u<<8)|((uint16)ten->c07l); out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c08u<<2)|((uint16)ten->c08l); out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c09u<<4)|((uint16)ten->c09l); out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c10u<<6)|((uint16)ten->c10l); out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c11u<<8)|((uint16)ten->c11l); out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c12u<<2)|((uint16)ten->c12l); out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c13u<<4)|((uint16)ten->c13l); out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c14u<<6)|((uint16)ten->c14l); out++; count++;}
      if (count<outWords)
        {*out = ((uint16)ten->c15u<<8)|((uint16)ten->c15l); out++; count++;}
    }
  }
}
#endif

#endif // UNPACK10_H


