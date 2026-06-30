//
// Created by erapa on 21/06/2026.
//

#ifndef QBIT_ERA_COMPLEX_H
#define QBIT_ERA_COMPLEX_H


/*
 * Numero complesso usato nel parser (#init, #define)
 */
typedef struct {
    double re;
    double im;
} ComplexNumber;

/*
 * Numero complesso usato nel simulatore (matrici, vettori)
 */
typedef struct {
    double re;
    double im;
} Complex;

/* Costruttore */
static inline Complex complex_new(double re, double im) {
    Complex c;
    c.re = re;
    c.im = im;
    return c;
}

/* Operazioni */
Complex complex_add(Complex a, Complex b);
Complex complex_mul(Complex a, Complex b);

/* Parsing stringhe tipo "1.0+2.0i", "3.5-0.2i", "2", "-i", "i" */
Complex parse_complex(const char* s);

double complex_abs2(Complex c);
#endif //QBIT_ERA_COMPLEX_H