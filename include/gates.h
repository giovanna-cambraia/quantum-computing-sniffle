#ifndef GATES_H
#define GATES_H

#include "complex_math.h"

#define SQRT1_2 0.70710678118654752440

// Static gate matrices
extern const Complex2x2 GATE_H; // Hadamard
extern const Complex2x2 GATE_X; // Pauli-X (bit flip)
extern const Complex2x2 GATE_Y; // Pauli-Y
extern const Complex2x2 GATE_Z; // Pauli-Z (phase flip)
extern const Complex2x2 GATE_S; // Phase gate (sqrt Z)
extern const Complex2x2 GATE_T; // pi/8 gate

// Parametric rotation gates - built at call time, not extern constants 
Complex2x2 gate_rx(double theta);
Complex2x2 gate_ry(double theta);
Complex2x2 gate_rz(double theta)

#endif