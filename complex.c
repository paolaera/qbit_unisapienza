#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "complex.h"

/*
 * Somma complessa
 */
Complex complex_add(Complex a, Complex b) {
    return complex_new(a.re + b.re, a.im + b.im);
}

/*
 * Prodotto complesso
 */
Complex complex_mul(Complex a, Complex b) {
    double re = a.re * b.re - a.im * b.im;
    double im = a.re * b.im + a.im * b.re;
    return complex_new(re, im);
}

/*
 */
double complex_abs2(Complex c) {
    return c.re * c.re + c.im * c.im;
}

/*
 * Parsing di stringhe complesse:
 *
 *  "3.2+1.0i"
 *  "3.2-1.0i"
 *  "5"
 *  "i"
 *  "-i"
 *  "2+i"
 *  "2-i"
 */
Complex parse_complex(const char* s) {
    Complex c = {0.0, 0.0};

    char *copy = strdup(s);
    if (!copy) {
        perror("Errore allocazione parse_complex");
        return c;
    }

    /* Rimuove spazi */
    char *p = copy;
    while (*p == ' ' || *p == '\t') p++;

    /* Caso: solo "i" o "-i" */
    if (strcmp(p, "i") == 0) {
        c.re = 0.0;
        c.im = 1.0;
        free(copy);
        return c;
    }
    if (strcmp(p, "-i") == 0) {
        c.re = 0.0;
        c.im = -1.0;
        free(copy);
        return c;
    }

    /* Cerca la parte immaginaria */
    char *i_ptr = strchr(p, 'i');

    if (!i_ptr) {
        /* Solo parte reale */
        c.re = strtod(p, NULL);
        c.im = 0.0;
        free(copy);
        return c;
    }

    /* Parte immaginaria presente */
    *i_ptr = '\0';  /* tronca la 'i' */

    /* Cerca + o - che separa reale e immaginaria */
    char *plus  = strrchr(p, '+');
    char *minus = strrchr(p, '-');

    char *sep = plus;
    if (!sep || (minus && minus > plus))
        sep = minus;

    if (!sep || sep == p) {
        /* Formati tipo "i", "-i", "3i" */
        c.re = 0.0;
        c.im = strtod(p, NULL);
        free(copy);
        return c;
    }

    /* Separazione reale / immaginaria */
    char real_buf[128];
    char imag_buf[128];

    size_t real_len = sep - p;
    strncpy(real_buf, p, real_len);
    real_buf[real_len] = '\0';

    strcpy(imag_buf, sep);  /* include il segno */

    c.re = strtod(real_buf, NULL);
    c.im = strtod(imag_buf, NULL);

    free(copy);
    return c;
}

