#include "ghz.h"
#include "gates.h"

void ghz_prepare(ThreeQubit *s) {
    *s = threequbit_init(); // |000> 
    threequbit_apply_gate(s, 0, &GATE_H);   // q0 -> (|0>+|1>)/sqrt2 
    threequbit_apply_cnot(s, 0, 1);         // entangle q1 to q0 
    threequbit_apply_cnot(s, 0, 2);         // entangle q2 to q0 
}