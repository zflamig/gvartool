#ifdef __GNUG__
#pragma interface 
#endif

#ifndef CRCCHECK_H
#define CRCCHECK_H

#define MAGIC_CRC 0x1d0f

unsigned short crcCheck(void *in_buffer, int in_n_bytes ); 
unsigned char parityCheck(void * start, int nchar ); 

#endif
