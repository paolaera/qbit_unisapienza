//
// Created by erapa on 21/06/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "threads.h"
#include "complex.h"

/*
 * Calcola le righe [start_row, end_row) di out per ogni thread.
 */
static void* worker_matvec(void *arg) {
    ThreadArgs *args = (ThreadArgs*)arg;
    Matrix *M = args->M;
    Complex *v = args->v;
    Complex *out = args->out;
    int start = args->start_row;
    int end   = args->end_row;
    int dim   = M->dim;

    for (int i = start; i < end; i++) {
        Complex sum = complex_new(0.0, 0.0);
        for (int j = 0; j < dim; j++) {
            sum = complex_add(sum, complex_mul(M->m[i][j], v[j]));
        }
        out[i] = sum;
    }

    return NULL;
}

/*
 * Prodotto matrice × vettore multi-thread.
 */
void matvec_mult_mt(Matrix *M, Complex *v, Complex *out, int num_threads) {
    int dim = M->dim;

    if (num_threads <= 1 || num_threads > dim) {
        for (int i = 0; i < dim; i++) {
            Complex sum = complex_new(0.0, 0.0);
            for (int j = 0; j < dim; j++) {
                sum = complex_add(sum, complex_mul(M->m[i][j], v[j]));
            }
            out[i] = sum;
        }
        return;
    }

    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    ThreadArgs *targs  = malloc(num_threads * sizeof(ThreadArgs));
    if (!threads || !targs) {
        fprintf(stderr, "Errore: allocazione thread\n");
        exit(1);
    }

    int base = dim / num_threads;
    int rem  = dim % num_threads;
    int row  = 0;

    for (int t = 0; t < num_threads; t++) {
        int chunk = base + (t < rem ? 1 : 0);
        targs[t].M = M;
        targs[t].v = v;
        targs[t].out = out;
        targs[t].start_row = row;
        targs[t].end_row = row + chunk;
        row += chunk;

        if (pthread_create(&threads[t], NULL, worker_matvec, &targs[t]) != 0) {
            fprintf(stderr, "Errore: pthread_create\n");
            exit(1);
        }
    }

    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }

    free(threads);
    free(targs);
}
