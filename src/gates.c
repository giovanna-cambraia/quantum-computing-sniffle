#include "gates.h"

const Complex2x2 GATE_H = {
    SQRT1_2,  SQRT1_2,
    SQRT1_2, -SQRT1_2
};

const Complex2x2 GATE_X = {
    0, 1,
    1, 0
};

const Complex2x2 GATE_Y = {
    0, -I,
    I,  0
};

const Complex2x2 GATE_Z = {
    1,  0,
    0, -1
};

const Complex2x2 GATE_S = {
    1, 0,
    0, I
};

const Complex2x2 GATE_T = {
    1, 0,
    0, (SQRT1_2 + SQRT1_2 * I) /* e^{i*pi/4} */
};

Complex2x2 gate_rx(double theta) {
    double c = cos(theta / 2.0);
    double s = sin(theta / 2.0);
    Complex2x2 m = { c, -I * s, -I * s, c };
    return m;
}

Complex2x2 gate_ry(double theta) {
    double c = cos(theta / 2.0);
    double s = sin(theta / 2.0);
    Complex2x2 m = { c, -s, s, c };
    return m;
}

Complex2x2 gate_rz(double theta) {
    double complex e_neg = cexp(-I * theta / 2.0);
    double complex e_pos = cexp( I * theta / 2.0);
    Complex2x2 m = { e_neg, 0, 0, e_pos };
    return m;
}