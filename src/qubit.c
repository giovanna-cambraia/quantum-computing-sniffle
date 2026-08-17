#include "qubit.h"
#include <stdlib.h>

Qubit qubit_init(void) {
    Qubit q;
    q.alpha = 1.0 + 0.0 * I;
    q.beta  = 0.0 + 0.0 * I;
    return q;
}

void qubit_apply_gate(Qubit *q, const Complex2x2 *gate) {
    double complex new_alpha, new_beta;
    c2x2_apply(gate, q->alpha, q->beta, &new_alpha, &new_beta);
    q->alpha = new_alpha;
    q->beta  = new_beta;
}

double qubit_prob0(const Qubit *q) { return c_prob(q->alpha); }
double qubit_prob1(const Qubit *q) { return c_prob(q->beta); }

int qubit_measure(Qubit *q) {
    double p0 = qubit_prob0(q);
    double r = (double)rand() / (double)RAND_MAX;
    int result = (r < p0) ? 0 : 1;

    if (result == 0) {
        q->alpha = 1.0;
        q->beta  = 0.0;
    } else {
        q->alpha = 0.0;
        q->beta  = 1.0;
    }
    return result;
}

/* 
   standard Bloch sphere mapping:
   psi> = cos(theta/2)|0> + e^{i*phi} sin(theta/2)|1>
   x = sin(theta)cos(phi), y = sin(theta)sin(phi), z = cos(theta)

   recover theta/phi from alpha, beta directly rather than assuming
   alpha is already real & positive, using relative phase.
 */
Vector3 qubit_to_bloch(const Qubit *q) {
    double complex a = q->alpha;
    double complex b = q->beta;

    // global phase doesn't matter; factor it out using alpha's phase 
    double alpha_mag = cabs(a);
    double beta_mag  = cabs(b);

    double theta = 2.0 * atan2(beta_mag, alpha_mag);
    double phi = carg(b) - carg(a);

    Vector3 v;
    v.x = (float)(sin(theta) * cos(phi));
    v.y = (float)(sin(theta) * sin(phi));
    v.z = (float)(cos(theta));
    return v;
}