//
// Created by erapa on 21/06/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "complex.h"

/*
 * Alloca una matrice dim x dim di numeri complessi.
 */
Matrix* matrix_alloc(int dim) {
    Matrix *matrix = malloc(sizeof(Matrix));
    if (!matrix) {
        fprintf(stderr, "Errore: impossibile allocare Matrix\n");
        exit(1);
    }

    matrix->dim = dim;
    matrix->m = malloc(dim * sizeof(Complex*));
    if (!matrix->m) {
        fprintf(stderr, "Errore: impossibile allocare righe Matrix\n");
        exit(1);
    }

    for (int i = 0; i < dim; i++) {
        matrix->m[i] = calloc(dim, sizeof(Complex));
        if (!matrix->m[i]) {
            fprintf(stderr, "Errore: impossibile allocare riga Matrix\n");
            exit(1);
        }
    }

    return matrix;
}

/*
 * Libera la memoria della matrice.
 */
void matrix_free(Matrix *mat) {
    if (!mat) return;

    for (int i = 0; i < mat->dim; i++) {
        free(mat->m[i]);
    }
    free(mat->m);
    free(mat);
}

/*
 * Alloca un vettore complesso di dimensione dim.
 */
Complex* vector_alloc(int dim) {
    Complex *vect = calloc(dim, sizeof(Complex));
    if (!vect) {
        fprintf(stderr, "Errore: impossibile allocare vettore\n");
        exit(1);
    }
    return vect;
}

/*
 * Copia un vettore complesso.
 */
void vector_copy(Complex *dest, Complex *src, int dim) {
    memcpy(dest, src, dim * sizeof(Complex));
}

/*
 * Prodotto matrice × vettore.
 *
 * out[i] = sum_j M[i][j] * v[j]
 */
void matvec_mult_single(Matrix *M, Complex *v, Complex *out) {
    int dim = M->dim;

    for (int i = 0; i < dim; i++) {
        Complex sum = complex_new(0.0, 0.0);

        for (int j = 0; j < dim; j++) {
            sum = complex_add(sum, complex_mul(M->m[i][j], v[j]));
        }

        out[i] = sum;
    }
}
