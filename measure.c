//
// Created by erapa on 21/06/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "measure.h"
#include "complex.h"

/*
 * Ritorna un double uniforme in [0,1).
 */
static double urand() {
    return (double)rand() / (double)RAND_MAX;
}

/*
 * Esegue una singola misura sullo stato.
 */
int measure_once(Complex *state, int dim) {
    // costruiamo distribuzione cumulativa
    double *cdf = malloc(dim * sizeof(double));
    if (!cdf) {
        fprintf(stderr, "Errore: allocazione cdf\n");
        exit(1);
    }

    double sum = 0.0;
    for (int i = 0; i < dim; i++) {
        sum += complex_abs2(state[i]);
        cdf[i] = sum;
    }

    // normalizza (per sicurezza, in caso di piccoli errori numerici)
    for (int i = 0; i < dim; i++) {
        cdf[i] /= sum;
    }

    double r = urand();
    int result = dim - 1;
    for (int i = 0; i < dim; i++) {
        if (r <= cdf[i]) {
            result = i;
            break;
        }
    }

    free(cdf);
    return result;
}

/*
 * Esegue 'repeats' misure e aggiorna counts.
 */
void measure_repeated(Complex *state, int dim, int repeats, int *counts) {
    // inizializza RNG una sola volta (puoi spostarlo in main se preferisci)
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }

    for (int i = 0; i < repeats; i++) {
        int j = measure_once(state, dim);
        counts[j]++;
    }
}

/*
 * Converte un indice j in stringa di bit di lunghezza n_qubits.
 */
static void index_to_bitstring(int j, int n_qubits, char *buf) {
    for (int k = n_qubits - 1; k >= 0; k--) {
        buf[n_qubits - 1 - k] = ((j & (1 << k)) ? '1' : '0');
    }
    buf[n_qubits] = '\0';
}

/*
 * Stampa la distribuzione empirica.
 */
void print_distribution(int *counts, int dim, int repeats, int n_qubits) {
    char bits[64];

    for (int j = 0; j < dim; j++) {
        if (counts[j] == 0) continue; // opzionale: puoi anche stampare tutti

        index_to_bitstring(j, n_qubits, bits);
        double p = (double)counts[j] / (double)repeats;
        printf("%s @ %.6f\n", bits, p);
    }
}
