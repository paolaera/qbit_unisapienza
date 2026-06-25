//
// Created by erapa on 21/06/2026.
//

#ifndef QBIT_ERA_CIRCUIT_H
#define QBIT_ERA_CIRCUIT_H

#include "matrix.h"
#include "complex.h"

/*
 * Struttura che rappresenta un circuito:
 * - k porte
 * - ogni porta è una matrice 2^n x 2^n
 */
typedef struct {
    int num_gates;   // numero di porte
    Matrix **gates;  // array di matrici
} Circuit;

/*
 * Alloca un circuito con k porte.
 */
Circuit* circuit_alloc(int k);

/*
 * Libera la memoria del circuito.
 */
void circuit_free(Circuit *c);

/*
 * Applica tutte le porte al vettore di stato.
 * Usa il numero di thread specificato.
 *
 * Vfin = G_{k-1} ... G1 (G0 Vinit)
 */
void circuit_apply(Circuit *c, Complex *state, int num_threads);
#endif //QBIT_ERA_CIRCUIT_H