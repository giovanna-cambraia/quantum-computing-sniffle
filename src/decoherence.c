#include "decoherence.h"
#include <stdlib.h>
#include <math.h>

static double rand_signed(void) {
    return ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
}

void decoherence_step_q0(TwoQubit *s, double strength, double dt) {
    if (strength <= 0.0) return;

    double kickScale = strength * dt * 3.0; 

    for (int q1 = 0; q1 < 2; q1++) {
        double complex *amp1 = &s->amp[1 * 2 + q1]; // q0=1 branch 

        // dephasing: random small phase jitter 
        double phaseKick = rand_signed() * kickScale;
        *amp1 = *amp1 * cexp(I * phaseKick);

        /// amplitude damping: shrink the |q0=1> branch toward 0,
        // pushing that lost probability into the |q0=0> branch 
        double damp = strength * dt * 0.5;
        if (damp > 1.0) damp = 1.0;

        double complex *amp0 = &s->amp[0 * 2 + q1];
        double complex lost = *amp1 * damp;
        *amp1 -= lost;
        *amp0 += lost; 
    }

    double total = 0.0;
    for (int i = 0; i < 4; i++) total += c_prob(s->amp[i]);
    if (total > 1e-12) {
        double norm = sqrt(total);
        for (int i = 0; i < 4; i++) s->amp[i] = s->amp[i] / norm;
    }
}