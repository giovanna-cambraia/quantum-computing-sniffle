#include "ecc.h"
#include "gates.h"
#include <stdlib.h>

void ecc_encode(ThreeQubit *s) {
    threequbit_apply_cnot(s, 0, 1);
    threequbit_apply_cnot(s, 0, 2);
}

int ecc_inject_error(ThreeQubit *s) {
    int choice = rand() % 4; // 0,1,2 = flip that qubit, 3 = no error 
    if (choice == 3) return -1;

    threequbit_apply_gate(s, choice, &GATE_X);
    return choice;
}

void ecc_correct(ThreeQubit *s) {
    threequbit_apply_cnot(s, 0, 1);
    threequbit_apply_cnot(s, 0, 2);
    threequbit_apply_toffoli(s, 1, 2, 0);
}