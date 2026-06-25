//
// Created by erapa on 21/06/2026.
//

#ifndef QBIT_ERA_MATRIX_H
#define QBIT_ERA_MATRIX_H
#include "complex.h"

typedef struct {
    int dim;        // dimensione: 2^n
    Complex **m;    // matrice [dim][dim]
} Matrix;

/*
 * Alloca una matrice dim x dim di numeri complessi.
 * I valori sono inizializzati a 0+0i
 */
Matrix* matrix_alloc(int dim);

/*
 * Libera la memoria della matrice.
 */
void matrix_free(Matrix *mat);

/*
 * Alloca un vettore di numeri complessi di dimensione dim.
 * Inizializzato a 0+0i.
 */
Complex* vector_alloc(int dim);

/*
 * Copia un vettore complesso in un altro.
 */
void vector_copy(Complex *dest, Complex *src, int dim);

/*
 * Prodotto matrice × vettore:
 * out[i] = sum_j M[i][j] * v[j]
 */
void matvec_mult_single(Matrix *M, Complex *v, Complex *out);
#endif //QBIT_ERA_MATRIX_H