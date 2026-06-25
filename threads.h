//
// Created by erapa on 21/06/2026.
//

#ifndef QBIT_ERA_THREADS_H
#define QBIT_ERA_THREADS_H


#include <pthread.h>
#include "matrix.h"
#include "complex.h"

/*
 * Struttura passata a ogni thread per calcolare
 * un sottoinsieme di righe della matrice.
 */
typedef struct {
    Matrix *M;          // matrice 2^n x 2^n
    Complex *v;         // vettore di input
    Complex *out;       // vettore di output
    int start_row;      // riga iniziale (inclusa)
    int end_row;        // riga finale (esclusa)
} ThreadArgs;

/*
 * Prodotto matrice × vettore multi-thread.
 *
 * Divide le righe tra 'num_threads' thread.
 * out[i] = sum_j M[i][j] * v[j]
 */
void matvec_mult_mt(Matrix *M, Complex *v, Complex *out, int num_threads);

#endif //QBIT_ERA_THREADS_H