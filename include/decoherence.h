#ifndef DECOHERENCE_H
#define DECOHERENCE_H

#include "twoqubit.h"

/* 
   applies one small step of simulated decoherence to qubit 0 of s, in place.
   strength in [0,1]: 0 = no effect, 1 = heavy noise per step.
   dt: time elapsed this frame (from GetFrameTime()), so the effect is
   frame-rate independent.

   models two effects together, both scaled by strength*dt:
     - dephasing: random small phase kick on the |1> amplitude (destroys
       phase coherence without changing populations)
     - amplitude damping: slowly pulls amplitude from |1> toward |0>,
       mimicking energy relaxation to the ground state
*/
void decoherence_step_q0(TwoQubit *s, double strength, double dt);

#endif