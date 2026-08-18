#ifndef ECC_H
#define ECC_H

#include "threequbit.h"

/* 
   3-qubit bit-flip code. protects an arbitrary q0 state against a single
   bit-flip (X) error on any ONE of the three physical qubits, without ever
   measuring the data directly — that's what makes it real error correction
   instead of just "measure and hope."
  
   encode: spreads q0's state across all three qubits:
   alpha|000> + beta|111>   (structurally identical to a GHZ prep, but
   starting from whatever state the caller already put on q0, not |0>+|1>)
 */
void ecc_encode(ThreeQubit *s);

int ecc_inject_error(ThreeQubit *s);

/* 
   decode + correct circuit: CNOT(0,1); CNOT(0,2); Toffoli(1,2 -> 0).
   restores q0 to its original state regardless of which single qubit (if
   any) was hit by ecc_inject_error, and returns q1/q2 to |0> as ancilla. 
 */
void ecc_correct(ThreeQubit *s);

#endif