#ifndef RENDER_H
#define RENDER_H

#include "raylib.h"
#include "twoqubit.h"

// draws a wireframe Bloch sphere + axes centered at `center` 
void render_bloch_frame(Vector3 center);

// draws a state marker at an explicit position, sphere centered at `center` 
void render_bloch_marker(Vector3 center, Vector3 localPos);

// draws hud text
void render_hud(const TwoQubit *s, int last_q0, int last_q1, double concurrence);

/* 
   spherical interpolation between two vectors by t in [0,1].
   handles vectors shorter than unit length (used for entangled/mixed states),
   interpolating length and direction separately. 
*/
Vector3 bloch_slerp(Vector3 from, Vector3 to, float t);

#endif 