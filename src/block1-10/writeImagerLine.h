#ifdef __GNUG__
#pragma interface 
#endif

#ifndef WRITELINE_H
#define WRITELINE_H

#include "include/types.h"
#include "include/gvar.h"

#include "gvarBlock/gvarBlock.h"  

#include "block0/imagerDoc.h"
#include "block11/doc/sounderDoc.h"
#include "block1-10/lineDoc.h"

#include "utils/radiometric/radiometric.h"

#include "utils/reSample/reSample.h"
#include "block0/grid.h" 

#include "utils/fileBuf/genericFile.h"
#include "utils/options/options.h"

#include "block1-10/skipIt.h"

void writeImagerLine(LineDoc * lineDoc); 

#endif

