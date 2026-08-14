#ifndef COMPLEX_MATH_H
#define COMPLEX_MATH_H

#include <complex.h>
#include <math.h>

// 2x2 matrix, row major: [ [a, b], [c, d] ]

typedef struct
{
    double complex a, b;
    double complex c, d;
} Complex2x2;

// apply matrix to 2-element vector (in-place safe)
static inline void c2x2_apply(const Complex2x2 *m,
                              double complex in0, double complex in1,
                              double complex *out0, double complex *out1)
{
    *out0 = m->a * in0 + m->b * in1;
    *out1 = m->c * in0 + m->d * in1;
}

// Multiply 2x2 matrices: result = lhs * rhs 
static inline Complex2x2 c2x2_mul(const Complex2x2 *lhs, const Complex2x2 *rhs) {
    Complex2x2 out;
    out.a = lhs->a * rhs->a + lhs->b * rhs->c;
    out.b = lhs->a * rhs->b + lhs->b * rhs->d;
    out.c = lhs->c * rhs->a + lhs->d * rhs->c;
    out.d = lhs->c * rhs->b + lhs->d * rhs->d;
    return out;
}

static inline double c_prob(double complex amp) {
    return creal(amp) * creal(amp) + cimag(amp) * cimag(amp);
}

#endif 
