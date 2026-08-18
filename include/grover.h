#ifndef GROVER_H
#define GROVER_H

#include "twoqubit.h"

// index of the "marked" item, 0..3 (matches TwoQubit amp indexing: 00,01,10,11) 
typedef int GroverTarget;

// resets s to |00>, applies H to both qubits -> uniform superposition. call this before running iterations. 
void grover_init_superposition(TwoQubit *s);

/* 
   oracle: flips the sign of the marked amplitude, leaves others untouched.

   this is the "mark the answer" step — doesn't change any probabilities yet
   (flipping a sign doesn't change |amplitude|^2), it just sets up the
   diffusion step to amplify it. 
 */
void grover_apply_oracle(TwoQubit *s, GroverTarget target);

/* 
   diffusion operator: reflects every amplitude about their mean.
   this is the step that actually converts the oracle's sign-flip into a
   real probability boost for the marked state. 
*/
void grover_apply_diffusion(TwoQubit *s);

// for 2 qubits / 1 marked item, ONE iteration is enough to reach ~100%. */
void grover_iterate(TwoQubit *s, GroverTarget target);

#endif