#ifndef RENDER_H
#define RENDER_H

#include "raylib.h"
#include "qubit.h"

// draws the wireframe bloch sphere + axes, no state marker
void render_bloch_frame(void);

// draws current state vector as a point + line from origin
void render_bloch_state(const Qubit *q);

// draws HUD text: amplitudes, probabilites, key hints
void render_hud(const Qubit *q, int last_measurement);

#endif