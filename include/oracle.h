#ifndef ORACLE_H
#define ORACLE_H

#include "twoqubit.h"

// the 4 possible 1-bit boolean functions f: {0,1} -> {0,1}
// implemented as Uf: |x>|y> -> |x>|y XOR f(x)>, acting on q0=x (control), q1=y (target)

/*
   CONSTANT_0: f(x) = 0        -> Uf is identity, does nothing
   CONSTANT_1: f(x) = 1        -> Uf = X on q1 (always flips target)
   BALANCED_ID: f(x) = x       -> Uf = CNOT(control=q0, target=q1)
   BALANCED_NOT: f(x) = 1 - x  -> Uf = CNOT(control=q0, target=q1) then X on q1
*/

typedef enum {
    ORACLE_CONSTANT_0 = 0,
    ORACLE_CONSTANT_1,
    ORACLE_BALANCED_ID,
    ORACLE_BALANCED_NOT,
    ORACLE_COUNT
} OracleType;

// true for CONSTANT_0 / CONSTANT_1 false for the balanced ones. 
int oracle_is_constant(OracleType type);

// readable name for hud display
const char *oracle_name(OracleType type);

// (q0=control/x, q1=target/y)
void oracle_apply(TwoQubit *s, OracleType type);

// picks uniformly random oracle type (uses rand())
OracleType oracle_random(void);

#endif