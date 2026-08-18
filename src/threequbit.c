#include "threequbit.h"
#include "stdlib.h"
#include <math.h>

ThreeQubit threequbit_init(void) {
    ThreeQubit s;
    for (int i = 0; i < 8; i++) s.amp[i] = 0.0;
    s.amp[0] = 1.0; // |000>
    return s;
}

// bit weight for qubit index: q0 is the MSB (weight 4), q2 is the LSB (weight 1)
static int qbit_weight(int qubit) {
    return 4 >> qubit; // qubit 0 -> 4, qubit 1 -> 2, qubit 2 -> 1
}

void threequbit_apply_gate(ThreeQubit *s, int qubit, const Complex2x2 *gate) {
    int weight = qbit_weight(qubit);
    double complex new_amp[8];
    for (int i = 0; i < 8; i++) new_amp[i] = s->amp[i]; 
 
    for (int i = 0; i < 8; i++) {
        if ((i & weight) == 0) {
            int i0 = i;
            int i1 = i | weight;
            double complex out0, out1;
            c2x2_apply(gate, s->amp[i0], s->amp[i1], &out0, &out1);
            new_amp[i0] = out0;
            new_amp[i1] = out1;
        }
    }
    for (int i = 0; i < 8; i++) s->amp[i] = new_amp[i];
}
 
void threequbit_apply_cnot(ThreeQubit *s, int control, int target) {
    int wc = qbit_weight(control);
    int wt = qbit_weight(target);
    double complex new_amp[8];
 
    for (int i = 0; i < 8; i++) {
        if (i & wc) {
            new_amp[i] = s->amp[i ^ wt];
        } else {
            new_amp[i] = s->amp[i];
        }
    }
    for (int i = 0; i < 8; i++) s->amp[i] = new_amp[i];
}

void threequbit_apply_toffoli(ThreeQubit *s, int control1, int control2, int target) {
    int wc1 = qbit_weight(control1);
    int wc2 = qbit_weight(control2);
    int wt  = qbit_weight(target);
    double complex new_amp[8];

    for (int i = 0; i < 8; i++) {
        if ((i & wc1) && (i & wc2)) {
            new_amp[i] = s->amp[i ^ wt];
        } else {
            new_amp[i] = s->amp[i];
        }
    }
    for (int i = 0; i < 8; i++) s->amp[i] = new_amp[i];
}
 
double threequbit_prob(const ThreeQubit *s, int index) {
    return c_prob(s->amp[index]);
}
 
int threequbit_measure_single(ThreeQubit *s, int qubit) {
    int weight = qbit_weight(qubit);
 
    double p1 = 0.0;
    for (int i = 0; i < 8; i++) {
        if (i & weight) p1 += c_prob(s->amp[i]);
    }
    double p0 = 1.0 - p1;
 
    double r = (double)rand() / (double)RAND_MAX;
    int result = (r < p0) ? 0 : 1;
 
    double norm = sqrt(result == 0 ? p0 : p1);
    if (norm < 1e-12) norm = 1.0;
 
    for (int i = 0; i < 8; i++) {
        int bit = (i & weight) ? 1 : 0;
        if (bit != result) {
            s->amp[i] = 0.0;
        } else {
            s->amp[i] = s->amp[i] / norm;
        }
    }
    return result;
}
 
Vector3 threequbit_reduced_bloch(const ThreeQubit *s, int qubit) {
    int weight = qbit_weight(qubit);
    double rho00 = 0.0, rho11 = 0.0;
    double complex rho01 = 0.0;
 
    for (int i = 0; i < 8; i++) {
        if ((i & weight) == 0) {
            int i0 = i;
            int i1 = i | weight;
            rho00 += c_prob(s->amp[i0]);
            rho11 += c_prob(s->amp[i1]);
            rho01 += s->amp[i0] * conj(s->amp[i1]);
        }
    }
 
    Vector3 v;
    v.x = (float)(2.0 * creal(rho01));
    v.y = (float)(-2.0 * cimag(rho01));
    v.z = (float)(rho00 - rho11);
    return v;
}