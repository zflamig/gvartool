#include <string.h>
#include <iomanip>
#include "block0/imagerDoc.h"
#include "imagerNav.h"
#include "utils/options/options.h"
#include "gvarBlock/gvarBlock.h"

extern Options Opt; 
extern GvarBlock gvar;
extern ImagerDoc imagerDoc;

int ImagerNav::imc()
{ int imc=1;
return imc;}

ImagerNav::ImagerNav(){
 memset(this, 0, 4) ; }

