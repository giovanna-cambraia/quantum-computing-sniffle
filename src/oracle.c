#include "oracle.h"
#include "gates.h"
#include <stdlib.h>

int oracle_is_constant(OracleType type)
{
    return type == ORACLE_CONSTANT_0 || type == ORACLE_CONSTANT_1;
}

const char *oracle_name(OracleType type)
{
    switch (type)
    {
    case ORACLE_CONSTANT_0:
        return "f(x)=0 (constant)";

    case ORACLE_CONSTANT_1:
        return "f(x)=1 (constant)";

    case ORACLE_BALANCED_ID:
        return "f(x)=x (balanced)";

    case ORACLE_BALANCED_NOT:
        return "f(x)=1-x (balanced)";

    default:
        return "unknown";
    }
}

void oracle_apply(TwoQubit *s, OracleType type)
{
    switch (type)
    {
    case ORACLE_CONSTANT_0:
        // identity: do nothing
        break;
    case ORACLE_CONSTANT_1:
        twoqubit_apply_gate_q1(s, &GATE_X);
        break;
    case ORACLE_BALANCED_ID:
        twoqubit_apply_cnot(s, 0, 1);
        break;
    case ORACLE_BALANCED_NOT:
        twoqubit_apply_cnot(s, 0, 1);
        twoqubit_apply_gate_q1(s, &GATE_X);
        break;
    default:
        break;
    }
}

OracleType oracle_random(void)
{
    return (OracleType)(rand() % ORACLE_COUNT);
}