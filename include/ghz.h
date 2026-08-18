#ifndef GHZ_H
#define GHZ_H

#include "threequbit.h"

/* 
   prepares the GHZ state (|000> + |111>)/sqrt(2) on a fresh 3-qubit register.
   unlike a Bell pair, this correlates THREE parties: measuring any one qubit
   instantly fixes the other two, and all three always agree (000 or 111,
   never a mix) — a genuinely different flavor of correlation than pairwise
   entanglement, since no two of the three qubits are entangled on their own.
*/
void ghz_prepare(ThreeQubit *s);

#endif