//
// Created by erapa on 21/06/2026.
//

#ifndef QBIT_ERA_COMPLEX_H
#define QBIT_ERA_COMPLEX_H
typedef struct {
    double re;   // parte reale
    double im;   // parte immaginaria
} Complex;

/*
 * Crea un numero complesso a + ib
 */
Complex complex_new(double re, double im);

/*
 * Somma tra due numeri complessi
 */
Complex complex_add(Complex a, Complex b);

/*
 * Prodotto tra due numeri complessi
 * (ax - by) + i(ay + bx)
 */
Complex complex_mul(Complex a, Complex b);

/*
 * Modulo al quadrato |z|^2 = a^2 + b^2
 * Utile per la fase di misura
 */
double complex_abs2(Complex a);

/*
 * Parsing di un numero complesso nel formato:
 *   0.123+i0.456
 *   1.0
 *   -0.5-i0.2
 *   0+i1
 * La parte immaginaria può mancare.
 */
Complex complex_parse(const char *s);
#endif //QBIT_ERA_COMPLEX_H