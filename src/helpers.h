#ifndef HELPERS_H
#define HELPERS_H

#include <stdlib.h>
#include "settings.h"
#include <cglm/cglm.h>
#include <stdio.h>

float* calcQuadVertromfWidth(float x, float y, float w, float h, float r, float g, float b, float a, int* outSize);
float convertPixelXToNDC(float x);
float convertPixelYToNDC(float y);
float convertPixelWidthToNDC(float w);
float convertPixelHeightToNDC(float h);

#endif