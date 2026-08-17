#ifndef RENDER_H
#define RENDER_H

#include "raylib.h"
#include "qubit.h"

// draws the wireframe bloch sphere + axes, no state marker
void render_bloch_frame(void);

// draws a state marker at an explicit position 
void render_bloch_marker(Vector3 pos);

// draws hud text
void render_hud(const Qubit *q, int last_measurement);

// spherical interpolation between two units vectors by t in [0,1]
Vector3 bloch_slerp(Vector3 from, Vector3 to, float t);

#endif