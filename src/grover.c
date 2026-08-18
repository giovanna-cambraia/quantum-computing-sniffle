#include "grover.h"
#include "gates.h"

void grover_init_superposition(TwoQubit *s) {
    *s = twoqubit_init();  // |00>
    twoqubit_apply_gate_q0(s, &GATE_H);
    twoqubit_apply_gate_q1(s, &GATE_H);
}

void grover_apply_oracle(TwoQubit *s, GroverTarget target) {
    s->amp[target] = -s->amp[target];
}

void grover_apply_diffusion(TwoQubit *s) {
    double complex mean = (s->amp[0] + s->amp[1] + s->amp[2] + s->amp[3]) / 4.0;
    for (int i = 0; i < 4; i++) {
        s->amp[i] = 2.0 * mean - s->amp[i];
    }
}

void grover_iterate(TwoQubit *s, GroverTarget target) {
    grover_apply_oracle(s, target);
    grover_apply_diffusion(s);
}