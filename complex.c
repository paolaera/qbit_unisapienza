#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "complex.h"

/*
 * Crea un numero complesso a + ib
 */
Complex complex_new(double re, double im) {
    Complex z;
    z.re = re;
    z.im = im;
    return z;
}

/*
 * Somma tra due numeri complessi
 */
Complex complex_add(Complex a, Complex b) {
    Complex r;
    r.re = a.re + b.re;
    r.im = a.im + b.im;
    return r;
}

/*
 * Prodotto tra due numeri complessi:
 * (ax - by) + i(ay + bx)
 */
Complex complex_mul(Complex a, Complex b) {
    Complex r;
    r.re = a.re * b.re - a.im * b.im;
    r.im = a.re * b.im + a.im * b.re;
    return r;
}

/*
 * Modulo al quadrato |z|^2 = a^2 + b^2
 */
double complex_abs2(Complex a) {
    return a.re * a.re + a.im * a.im;
}

/*
 * Parsing di un numero complesso
 */
Complex complex_parse(const char *s) {
    double re = 0.0, im = 0.0;
    char *plus = NULL;
    char *minus = NULL;
    char *i_ptr = NULL;

    // Copia locale modificabile
    char buf[128];
    strncpy(buf, s, 127);
    buf[127] = '\0';

    // Rimuovi spazi
    for (int i = 0; buf[i]; i++) {
        if (isspace((unsigned char)buf[i])) {
            memmove(&buf[i], &buf[i+1], strlen(buf) - i);
            i--;
        }
    }

    // Cerca la parte immaginaria
    i_ptr = strchr(buf, 'i');

    if (!i_ptr) {
        // Solo parte reale
        re = atof(buf);
        return complex_new(re, 0.0);
    }

    // Se c'è 'i', separiamo le parti
    *i_ptr = '\0'; // tronca prima della 'i'

    // Cerca + o - dopo la parte reale
    plus = strrchr(buf, '+');
    minus = strrchr(buf, '-');

    char *sep = NULL;

    if (plus && plus != buf) sep = plus;
    else if (minus && minus != buf) sep = minus;

    if (!sep) {
        // Caso tipo: "i0.5" oppure "-i0.2"
        if (buf[0] == '\0') {
            // "i0.5"
            im = atof(i_ptr + 1);
            return complex_new(0.0, im);
        } else {
            // "1.0i"
            re = atof(buf);
            return complex_new(re, 0.0);
        }
    }

    // Separiamo reale e immaginaria
    *sep = '\0';
    re = atof(buf);
    im = atof(sep);

    return complex_new(re, im);
}
