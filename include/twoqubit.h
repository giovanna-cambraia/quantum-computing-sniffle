#ifndef TWOQUBIT_H
#define TWOQUBIT_H

#include "complex_math.h"
#include "raylib.h"

// amp index = q0*2 + q1, i.e. amp[0]=|00>, amp[1]=|01>, amp[2]=|10>, amp[3]=|11>

typedef struct {
    double complex amp[4];
} TwoQubit;

TwoQubit twoqubit_init(void); // starts at |00>

// apply a single-qubit gate to qubit 0 or qubit 1, one stays untouched 
void twoqubit_apply_gate_q0(TwoQubit *s, const Complex2x2 *gate);
void twoqubit_apply_gate_q1(TwoQubit *s, const Complex2x2 *gate);

// CNOT: flips target if control is |1>
void twoqubit_apply_cnot(TwoQubit *s, int control, int target);

double twoqubit_prob(const TwoQubit *s, int index);

// measure both qubits jointly, collapses state, return via out params 
void twoqubit_measure(TwoQubit *s, int *out_q0, int *out_q1);

// reduced single-qubit Bloch vector via partial trace (qubit = 0 or 1)
Vector3 twoqubit_reduced_bloch(const TwoQubit *s, int qubit);

// 0 = separable, 1 = maximally entagled, entaglement measurement 
double twoqubit_concurrence(const TwoQubit *s);

#endif