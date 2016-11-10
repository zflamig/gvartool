#ifndef UNPACK10_H
#define UNPACK10_H

#include "include/types.h"

void unpack10(void *in,	    // pointer to packed data
              int outWords, // size of output buffer in words
              uint16 * Out);// pointer to unpacked data

#endif // UNPACK10_H
