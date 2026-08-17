#ifndef BARS_H
#define BARS_H

#include "raylib.h"
#include "twoqubit.h"

/* 
   draws 4 bars (one per basis state |00>,|01>,|10>,|11>) showing probability
   (bar height) and the sign of the real part of the amplitude (color/label),
   anchored at top-left corner (x, y), spanning roughly `width` x `height` px.
   
   this is what makes interference visible: watch a bar rise on the first H,
   then watch it cancel back toward zero on the second H — that collapse is
   amplitudes with opposite signs adding to zero, not measurement or noise. 
*/
void render_amplitude_bars(const TwoQubit *s, int x, int y, int width, int height);

#endif