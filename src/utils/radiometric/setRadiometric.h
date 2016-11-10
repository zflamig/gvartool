#ifndef RADIOMETRIC_CONSTANTS_H
#define RADIOMETRIC_CONSTANTS_H

#include "block0/imagerDoc.h"
#include "block11/doc/sounderDoc.h"
#include "block11/sadId.h"
#include "block11/spacelook/imager/imagerSpacelook.h"
#include "radiometric.h"
#include "include/gvar.h"

void setRadiometric
(Radiometric * Iunits[CHANNELS][SIDES][DETECTORS], 
 ImagerDoc & imagerDoc);

void printRadiometric
(Radiometric * Iunits[CHANNELS][SIDES][DETECTORS]);

void print_s_Radiometric
(Radiometric * Sunits[CHANNELS][SIDES][DETECTORS]);

void setRadiometric
(Radiometric * Sunits[CHANNELS][SIDES][DETECTORS],
 SounderDoc & sounderDoc);

void resetVisibleRadiometricConstants
(Radiometric * Iunits[CHANNELS][SIDES][DETECTORS],
 ImagerSpacelook & imagerSpacelook);

#endif // SetRadiometricConstants

