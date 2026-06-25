//
// Created by erapa on 21/06/2026.
//

#ifndef QBIT_ERA_PARSER_H
#define QBIT_ERA_PARSER_H

#include "matrix.h"
#include "circuit.h"
#include "complex.h"

/*
 * Risultato del parsing dei file di input.
 */
typedef struct {
    int n_qubits;       // numero di qubit
    int dim;            // 2^n
    Complex *init_state;
    Circuit *circuit;
    int measure_count;  // 0 se non c'è "measure"
} ParsedInput;

/*
 * Parsing del file con #qubits e #init
 */
void parse_state_file(const char *filename, ParsedInput *out);

/*
 * Parsing del file con #define e #circ
 */
void parse_circuit_file(const char *filename, ParsedInput *out);
#endif //QBIT_ERA_PARSER_H