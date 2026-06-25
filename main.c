#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "circuit.h"
#include "measure.h"
#include "complex.h"

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr,
            "Uso: %s <file_stato> <file_circuito> <num_thread>\n",
            argv[0]);
        return 1;
    }

    const char *state_file   = argv[1];
    const char *circuit_file = argv[2];
    int num_threads = atoi(argv[3]);

    ParsedInput input;
    memset(&input, 0, sizeof(ParsedInput));

    parse_state_file(state_file, &input);

    parse_circuit_file(circuit_file, &input);

    circuit_apply(input.circuit, input.init_state, num_threads);

    if (input.measure_count == 0) {
        printf("#final [");
        for (int i = 0; i < input.dim; i++) {
            printf("%.6f+i%.6f", input.init_state[i].re, input.init_state[i].im);
            if (i < input.dim - 1) printf(", ");
        }
        printf("]\n");
    }

    else {
        int *counts = calloc(input.dim, sizeof(int));
        if (!counts) {
            fprintf(stderr, "Errore: allocazione counts\n");
            exit(1);
        }

        measure_repeated(input.init_state, input.dim, input.measure_count, counts);
        print_distribution(counts, input.dim, input.measure_count, input.n_qubits);

        free(counts);
    }

    circuit_free(input.circuit);
    free(input.init_state);

    return 0;
}