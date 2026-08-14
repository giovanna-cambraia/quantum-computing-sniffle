#ifndef QUBIT_H
#define QUBIT_H

#include "complex_math.h"
#include "raylib.h"

typedef struct {
    double complex alpha; // amplitude of |0>
    double complex beta; // amplitude of |1>
} Qubit;

// intialize to |0>
Qubit qubit_init(void);

// apply an arbitrary 2x2 gate
void qubit_apply_gate(Qubit *q, const Complex2x2 *gate);

// collapse state via measurement in the computational basis
// returns 0 or 1, mutates q to the post-measurement basis state
int qubit_measure(Qubit *q);

// map current amplitude to a point on the Bloch sphere
Vector3 qubit_to_bloch(const Qubit *q);

// probability of measuring |0> & |1>
double qubit_prob0(const Qubit *q);
double qubit_prob1(const Qubit *q);

#endif
