#ifndef DJ_H
#define DJ_H

#include "twoqubit.h"
#include "oracle.h"

/*
   resets s to |00>, then prepares and runs the full Deutsch circuit:
     X q1              (q1: |0> -> |1>)
     H q0, H q1         (both into superposition)
     Uf                 (the oracle, phase-kickback onto q0)
     H q0                (interference: collapses q0's phase into amplitude)

   after this call, s is ready to be measured with twoqubit_measure().
   q0's outcome answers the question: 0 = constant, 1 = balanced.
   (q1 is measurement garbage — the algorithm only reads q0.)
*/
void dj_prepare(TwoQubit *s, OracleType type);

#endif