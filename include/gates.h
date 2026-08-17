#ifndef GATES_H
#define GATES_H

#include "complex_math.h"

#define SQRT1_2 0.70710678118654752440

// static gate matrices 
extern const Complex2x2 GATE_H;   // hadamard
extern const Complex2x2 GATE_X;   // pauli-X (bit flip) 
extern const Complex2x2 GATE_Y;   // pauli-Y 
extern const Complex2x2 GATE_Z;   // pauli-Z (phase flip) 
extern const Complex2x2 GATE_S;   // phase gate (sqrt Z) 
extern const Complex2x2 GATE_T;   // pi/8 gate 

// parametric rotation gates — built at call time, not extern constants 
Complex2x2 gate_rx(double theta);
Complex2x2 gate_ry(double theta);
Complex2x2 gate_rz(double theta);

#endif 