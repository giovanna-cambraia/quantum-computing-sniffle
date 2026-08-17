#include "twoqubit.h"
#include <stdlib.h>

TwoQubit twoqubit_init(void) {
    TwoQubit s;
    s.amp[0] = 1.0; // |00> 
    s.amp[1] = 0.0;
    s.amp[2] = 0.0;
    s.amp[3] = 0.0;
    return s;
}

void twoqubit_apply_gate_q0(TwoQubit *s, const Complex2x2 *g) {
    double complex new_amp[4];
    for (int q1 = 0; q1 < 2; q1++) {
        double complex in0 = s->amp[0 * 2 + q1];
        double complex in1 = s->amp[1 * 2 + q1];
        double complex out0, out1;
        c2x2_apply(g, in0, in1, &out0, &out1);
        new_amp[0 * 2 + q1] = out0;
        new_amp[1 * 2 + q1] = out1;
    }
    for (int i = 0; i < 4; i++) s->amp[i] = new_amp[i];
}

void twoqubit_apply_gate_q1(TwoQubit *s, const Complex2x2 *g) {
    double complex new_amp[4];
    for (int q0 = 0; q0 < 2; q0++) {
        double complex in0 = s->amp[q0 * 2 + 0];
        double complex in1 = s->amp[q0 * 2 + 1];
        double complex out0, out1;
        c2x2_apply(g, in0, in1, &out0, &out1);
        new_amp[q0 * 2 + 0] = out0;
        new_amp[q0 * 2 + 1] = out1;
    }
    for (int i = 0; i < 4; i++) s->amp[i] = new_amp[i];
}

void twoqubit_apply_cnot(TwoQubit *s, int control, int target) {
    double complex new_amp[4];
    for (int i = 0; i < 4; i++) new_amp[i] = s->amp[i];

    for (int q0 = 0; q0 < 2; q0++) {
        for (int q1 = 0; q1 < 2; q1++) {
            int bits[2] = { q0, q1 };
            if (bits[control] == 1) {
                bits[target] ^= 1;
                new_amp[bits[0] * 2 + bits[1]] = s->amp[q0 * 2 + q1];
            }
        }
    }
    for (int i = 0; i < 4; i++) s->amp[i] = new_amp[i];
}

double twoqubit_prob(const TwoQubit *s, int index) {
    return c_prob(s->amp[index]);
}

void twoqubit_measure(TwoQubit *s, int *out_q0, int *out_q1) {
    double probs[4];
    double cumulative = 0.0;
    for (int i = 0; i < 4; i++) probs[i] = c_prob(s->amp[i]);

    double r = (double)rand() / (double)RAND_MAX;
    int chosen = 3;
    for (int i = 0; i < 4; i++) {
        cumulative += probs[i];
        if (r < cumulative) { chosen = i; break; }
    }

    for (int i = 0; i < 4; i++) s->amp[i] = (i == chosen) ? 1.0 : 0.0;

    *out_q0 = chosen / 2;
    *out_q1 = chosen % 2;
}

Vector3 twoqubit_reduced_bloch(const TwoQubit *s, int qubit) {
    double complex c00 = s->amp[0], c01 = s->amp[1], c10 = s->amp[2], c11 = s->amp[3];
    double complex rho01;
    double p0, p1;

    if (qubit == 0) {
        p0 = c_prob(c00) + c_prob(c01); // q0 = 0 
        p1 = c_prob(c10) + c_prob(c11); // q0 = 1 
        rho01 = c00 * conj(c10) + c01 * conj(c11);
    } else {
        p0 = c_prob(c00) + c_prob(c10); // q1 = 0 
        p1 = c_prob(c01) + c_prob(c11); // q1 = 1 
        rho01 = c00 * conj(c01) + c10 * conj(c11);
    }

    Vector3 v;
    v.x = (float)(2.0 * creal(rho01));
    v.y = (float)(-2.0 * cimag(rho01));
    v.z = (float)(p0 - p1);
    return v;
}

double twoqubit_concurrence(const TwoQubit *s) {
    double complex term = s->amp[0] * s->amp[3] - s->amp[1] * s->amp[2];
    return 2.0 * cabs(term);
}