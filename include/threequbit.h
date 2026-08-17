#ifndef THREEQUBIT_H
#define THREEQUBIT_H

#include "complex_math.h"
#include "raylib.h"

// amp index = q0*4 + q1*2 + q2, i.e. amp[0]=|000>, amp[1]=|001>, ..., amp[7]=|111>

typedef struct {
    double complex amp[8];
} ThreeQubit;

ThreeQubit threequbit_init(void); // starts at |000>

// apply single-qubit gate to qubit 0, 1 or 2; the other two stay untouched 
void threequbit_apply_gate(ThreeQubit *s, int qubit, const Complex2x2 *gate);

// CNOT: flips target if control is |1>, both in {0,1,2}
void threequbit_apply_cnot(ThreeQubit *s, int control, int target);

double threequbit_prob(const ThreeQubit *s, int index);

// measures a SINGLE qubit, collapsing only that qubit (the other two stay entangled/in-superposition as appropriate). 
// renormalizes the surviving amplitudes. Returns the measured bit (0 or 1).
int threequbit_measure_single(ThreeQubit *s, int qubit);

// reduced single-qubit Bloch vector via partial trace over the other two qubits (qubit = 0, 1, or 2). same idea as twoqubit_reduced_bloch — use
// this to visually compare q0's vector before teleportation to q2's vector after: they should match once the classical corrections are applied.
Vector3 threequbit_reduced_bloch(const ThreeQubit *s, int qubit);

#endif