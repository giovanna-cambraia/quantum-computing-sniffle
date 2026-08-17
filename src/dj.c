#include "dj.h"
#include "gates.h"

void dj_prepare(TwoQubit *s, OracleType type) {
    *s = twoqubit_init();                  // |00> 
    twoqubit_apply_gate_q1(s, &GATE_X);    // |01> : q1 = |1> 
    twoqubit_apply_gate_q0(s, &GATE_H);    // superpose q0 
    twoqubit_apply_gate_q1(s, &GATE_H);    // superpose q1 
    oracle_apply(s, type);                 // Uf, phase-kickback onto q0 
    twoqubit_apply_gate_q0(s, &GATE_H);    // interference on q0 
}