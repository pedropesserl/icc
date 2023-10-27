#include <stdio.h>
#include <stdlib.h>    /* exit, malloc, calloc, etc. */
#include <string.h>
#include <getopt.h>    /* getopt */
#include <time.h>
#include <likwid.h>

#include "matriz.h"

/**
 * Exibe mensagem de erro indicando forma de uso do programa e termina
 * o programa.
 */

static void usage(char *progname) {
    fprintf(stderr, "Forma de uso: %s [ <ordem> ] \n", progname);
    exit(1);
}

/**
 * Programa principal
 * Forma de uso: matmult [ -n <ordem> ]
 * -n <ordem>: ordem da matriz quadrada e dos vetores
 *
 */

int main (int argc, char *argv[]) {
    LIKWID_MARKER_INIT;
    int n=DEF_SIZE;

    MatRow mRow_1, mRow_2, resMat;
    Vetor vet, res;

    /* =============== TRATAMENTO DE LINHA DE COMANDO =============== */

    if (argc < 2)
        usage(argv[0]);

    n = atoi(argv[1]);

    /* ================ FIM DO TRATAMENTO DE LINHA DE COMANDO ========= */

    srandom(20232);

    res = geraVetor(n, 1); // (real_t *) malloc (n*sizeof(real_t));
    resMat = geraMatRow(n, n, 1);

    mRow_1 = geraMatRow(n, n, 0);
    mRow_2 = geraMatRow(n, n, 0);

    vet = geraVetor (n, 0);

    if (!res || !resMat || !mRow_1 || !mRow_2 || !vet) {
        fprintf(stderr, "Falha em alocação de memória !!\n");
        liberaVetor ((void*) mRow_1);
        liberaVetor ((void*) mRow_2);
        liberaVetor ((void*) resMat);
        liberaVetor ((void*) vet);
        liberaVetor ((void*) res);
        exit(2);
    }

    LIKWID_MARKER_START("MatVet_otm");
    multMatVet_otimizado(mRow_1, vet, n, n, res);
    LIKWID_MARKER_STOP("MatVet_otm");
    // multMatMat(mRow_1, mRow_2, n, resMat);

#ifdef _DEBUG_
    // printf("Sem otimização\n");
    prnVetor(res, n);
#endif 

    memset(res,0,n*sizeof(real_t));
    LIKWID_MARKER_START("MatVet");
    multMatVet(mRow_1, vet, n, n, res);
    LIKWID_MARKER_STOP("MatVet");

#ifdef _DEBUG_
    // printf("Com otimização\n");
    prnVetor(res, n);
#endif 

    liberaVetor ((void*) mRow_1);
    liberaVetor ((void*) mRow_2);
    liberaVetor ((void*) resMat);
    liberaVetor ((void*) vet);
    liberaVetor ((void*) res);

    LIKWID_MARKER_CLOSE;
    return 0;
}

