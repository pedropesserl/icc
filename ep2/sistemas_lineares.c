#include <stdio.h>
#include <stdlib.h>

double **cria_matriz(size_t tam, double *data) {
    double **m = (double**)calloc(tam, sizeof(double*));
    if (!m) {
        fprintf(stderr, "Erro de alocação de memória (cria_matriz())\n");
        exit(1);
    }
    for (size_t k = 0; k < tam; k++)
        m[k] = &(data[k*tam]);
    return m;
}

int main() {
    size_t tam;
    scanf("%ld", &tam);

    double *entradas = (double*)calloc(tam*tam, sizeof(double));
    double *B = (double*)calloc(tam, sizeof(double));
    double *X = (double*)calloc(tam, sizeof(double));
    for (size_t i = 0; i < tam; i++) {
        for (size_t j = 0; j < tam; j++)
            scanf("%lf", &(entradas[i*tam+j]));
        scanf("%lf", &(B[i]));
    }

    double **A = cria_matriz(tam, entradas);

    free(entradas);
    free(A);
    free(B);
    free(X);
    return 0;
}
