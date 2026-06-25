//
// Created by erapa on 21/06/2026.
//

#ifndef QBIT_ERA_MEASURE_H
#define QBIT_ERA_MEASURE_H

#include "complex.h"

/*
 * Esegue una singola misura sullo stato finale:
 * - state: vettore di ampiezze complesse (dimensione dim)
 * - dim: 2^n
 * Ritorna un intero j in [0, dim-1] con probabilità |a_j|^2.
 */
int measure_once(Complex *state, int dim);

/*
 * Esegue 'repeats' misure e costruisce una distribuzione empirica.
 * - counts: array di dim interi, inizializzato a 0 dal chiamante
 */
void measure_repeated(Complex *state, int dim, int repeats, int *counts);

/*
 * Stampa la distribuzione di probabilità stimata:
 * per ogni stato j stampa:
 *   <bitstring> @ <freq/repeats>
 */
void print_distribution(int *counts, int dim, int repeats, int n_qubits);
#endif //QBIT_ERA_MEASURE_H