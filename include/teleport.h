#ifndef TELEPORT_H
#define TELEPORT_H

#include "threequbit.h"

/*
   runs the teleportation protocol on a 3-qubit state where:
     q0 = the message qubit — caller must have already prepared its state
          (e.g. apply gates to q0 while q1=q2=|0>, BEFORE calling this)
     q1 = Alice's half of the Bell pair (must be |0> on entry)
     q2 = bob's half of the Bell pair (must be |0> on entry) — receives
          the teleported state when this function returns

   steps performed:
     1. entangle q1,q2 into a Bell pair:      H q1; CNOT(1,2)
     2. bell-basis measurement gadget:        CNOT(0,1); H q0
     3. measure q0 -> m0, measure q1 -> m1    (both collapse, both classical now)
     4. classically-conditioned correction on q2:
          if m1==1: X q2
          if m0==1: Z q2

   after the call, q2's amplitudes equal the ORIGINAL q0 state (up to the
   corrections already applied — no further action needed by the caller).
   m0/m1 are returned only so it can display "what Alice measured" in the hud;
   they carry no information about the teleported state on their own.
*/
void teleport_run(ThreeQubit *s, int *out_m0, int *out_m1);

#endif