#include "teleport.h"
#include "gates.h"

void teleport_run(ThreeQubit *s, int *out_m0, int *out_m1) {
    // bell pair between q1 (Alice) and q2 (Bob) 
    threequbit_apply_gate(s, 1, &GATE_H);
    threequbit_apply_cnot(s, 1, 2);

    // bell-basis measurement gadget on q0 (message) and q1 (Alice's half) 
    threequbit_apply_cnot(s, 0, 1);
    threequbit_apply_gate(s, 0, &GATE_H);

    // measure Alice's two qubits — collapses them, q2 is left holding the (possibly X/Z-twisted) teleported state 
    int m0 = threequbit_measure_single(s, 0);
    int m1 = threequbit_measure_single(s, 1);

    // classically-conditioned correction on Bob's qubit 
    if (m1 == 1) threequbit_apply_gate(s, 2, &GATE_X);
    if (m0 == 1) threequbit_apply_gate(s, 2, &GATE_Z);

    if (out_m0) *out_m0 = m0;
    if (out_m1) *out_m1 = m1;
}