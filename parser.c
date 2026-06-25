//
// Created by erapa on 21/06/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "complex.h"
#include "matrix.h"
#include "circuit.h"

/*
 * Rimuove whitespace da una stringa (in place)
 */
static void strip_spaces(char *s) {
    int w = 0;
    for (int r = 0; s[r]; r++) {
        if (!isspace((unsigned char)s[r])) {
            s[w++] = s[r];
        }
    }
    s[w] = '\0';
}

/*
 * Parsing del file con #qubits e #init
 */
void parse_state_file(const char *filename, ParsedInput *out) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Errore: impossibile aprire %s\n", filename);
        exit(1);
    }

    char line[512];

    out->n_qubits = -1;
    out->init_state = NULL;

    while (fgets(line, sizeof(line), f)) {
        strip_spaces(line);

        if (strncmp(line, "#qubits", 7) == 0) {
            out->n_qubits = atoi(line + 7);
            out->dim = 1 << out->n_qubits;
        }

        else if (strncmp(line, "#init[", 6) == 0) {
            if (out->n_qubits < 0) {
                fprintf(stderr, "Errore: #init prima di #qubits\n");
                exit(1);
            }

            out->init_state = vector_alloc(out->dim);

            char *p = strchr(line, '[');
            char *q = strchr(line, ']');
            if (!p || !q) {
                fprintf(stderr, "Errore: formato #init non valido\n");
                exit(1);
            }

            *q = '\0';
            p++;

            int idx = 0;
            char *tok = strtok(p, ",");
            while (tok && idx < out->dim) {
                out->init_state[idx++] = complex_parse(tok);
                tok = strtok(NULL, ",");
            }

            if (idx != out->dim) {
                fprintf(stderr, "Errore: numero di coefficienti #init errato\n");
                exit(1);
            }
        }
    }

    fclose(f);

    if (out->n_qubits < 0 || !out->init_state) {
        fprintf(stderr, "Errore: file stato incompleto\n");
        exit(1);
    }
}

/*
 * Parsing del file con #define e #circ
 */
void parse_circuit_file(const char *filename, ParsedInput *out) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Errore: impossibile aprire %s\n", filename);
        exit(1);
    }

    char line[1024];
    int dim = out->dim;

    // Per memorizzare le matrici definite
    typedef struct {
        char name[64];
        Matrix *mat;
    } Def;

    Def defs[128];
    int def_count = 0;

    out->measure_count = 0;

    while (fgets(line, sizeof(line), f)) {
        strip_spaces(line);

        if (strncmp(line, "#define", 7) == 0) {
            char *p = line + 7;

            // nome matrice
            char name[64];
            int i = 0;
            while (*p && !isspace((unsigned char)*p)) {
                name[i++] = *p++;
            }
            name[i] = '\0';

            // cerca '['
            char *lb = strchr(line, '[');
            char *rb = strrchr(line, ']');
            if (!lb || !rb) {
                fprintf(stderr, "Errore: formato #define non valido\n");
                exit(1);
            }

            *rb = '\0';
            lb++;

            Matrix *M = matrix_alloc(dim);

            int idx = 0;
            char *tok = strtok(lb, ")(");
            while (tok && idx < dim * dim) {
                char *comma = strchr(tok, ',');
                if (!comma) {
                    fprintf(stderr, "Errore: elemento matrice non valido\n");
                    exit(1);
                }
                *comma = '\0';

                Complex c = complex_new(atof(tok), atof(comma + 1));
                int r = idx / dim;
                int cidx = idx % dim;
                M->m[r][cidx] = c;

                idx++;
                tok = strtok(NULL, ")(");
            }

            if (idx != dim * dim) {
                fprintf(stderr, "Errore: numero elementi matrice errato\n");
                exit(1);
            }

            strcpy(defs[def_count].name, name);
            defs[def_count].mat = M;
            def_count++;
        }

        else if (strncmp(line, "#circ", 5) == 0) {
            char *p = line + 5;

            char *tokens[128];
            int tcount = 0;

            char *tok = strtok(p, " ");
            while (tok) {
                tokens[tcount++] = tok;
                tok = strtok(NULL, " ");
            }

            // misura?
            int measure_pos = -1;
            for (int i = 0; i < tcount; i++) {
                if (strcmp(tokens[i], "measure") == 0) {
                    measure_pos = i;
                    break;
                }
            }

            int num_gates = (measure_pos == -1 ? tcount : measure_pos);

            Circuit *c = circuit_alloc(num_gates);

            for (int i = 0; i < num_gates; i++) {
                int found = 0;
                for (int d = 0; d < def_count; d++) {
                    if (strcmp(tokens[i], defs[d].name) == 0) {
                        c->gates[i] = defs[d].mat;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    fprintf(stderr, "Errore: porta %s non definita\n", tokens[i]);
                    exit(1);
                }
            }

            out->circuit = c;

            if (measure_pos != -1) {
                out->measure_count = atoi(tokens[measure_pos + 1]);
            }
        }
    }

    fclose(f);

    if (!out->circuit) {
        fprintf(stderr, "Errore: nessuna direttiva #circ trovata\n");
        exit(1);
    }
}

