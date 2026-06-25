//
// Created by erapa on 21/06/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include "circuit.h"
#include "threads.h"
#include "matrix.h"
#include "complex.h"

/*
 * Alloca un circuito con k porte.
 */
Circuit* circuit_alloc(int k) {
    Circuit *c = malloc(sizeof(Circuit));
    if (!c) {
        fprintf(stderr, "Errore: impossibile allocare Circuit\n");
        exit(1);
    }

    c->num_gates = k;
    c->gates = malloc(k * sizeof(Matrix*));
    if (!c->gates) {
        fprintf(stderr, "Errore: impossibile allocare gates\n");
        exit(1);
    }

    return c;
}

/*
 * Libera la memoria del circuito.
 */
void circuit_free(Circuit *c) {
    if (!c) return;

    for (int i = 0; i < c->num_gates; i++) {
        matrix_free(c->gates[i]);
    }

    free(c->gates);
    free(c);
}

/*
 * Applica tutte le porte al vettore di stato.
 *
 * Per ogni porta:
 *   out = G * state
 *   state = out
 *
 * Usa matvec_mult_mt() per parallelizzare.
 */
void circuit_apply(Circuit *c, Complex *state, int num_threads) {
    if (!c || c->num_gates == 0) return;

    int dim = c->gates[0]->dim;

    // vettore temporaneo per i risultati intermedi
    Complex *tmp = vector_alloc(dim);

    for (int g = 0; g < c->num_gates; g++) {
        Matrix *G = c->gates[g];

        // prodotto matrice × vettore multi-thread
        matvec_mult_mt(G, state, tmp, num_threads);

        // copia tmp → state per la porta successiva
        vector_copy(state, tmp, dim);
    }

    free(tmp);
}
