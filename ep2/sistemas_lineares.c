#include <stdio.h>
#include <stdlib.h>

double **cria_matriz(size_t tam, double *data) {
    double **m = (double**)calloc(tam, sizeof(double*));
    if (!m) {
        fprintf(stderr, "Erro de alocação de memória (cria_matriz())\n");
        exit(1);
    }
    for (int k = 0; k < tam; k++)
        m[k] = &(data[k*tam]);
    return m;
}

int main() {
    size_t tam;
    double *entradas = (double*)calloc(tam*tam, sizeof(double));
    double *B = (double*)calloc(tam, sizeof(double));
    double *X = (double*)calloc(tam, sizeof(double));

    scanf("%ld", &tam);
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++)
            scanf("%lf", &(entradas[i*tam+j]));
        scanf("%lf", &(B[i]));
    }

    double **A = cria_matriz(tam, entradas);
    printf("A:\n");
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++)
            printf("%lf ", A[i][j]);
        printf("\n");
    }

    printf("B:\n");
    for (int i = 0; i < tam; i++)
        printf("%lf ", A[i][j]);
    printf("\n");

    free(entradas);
    free(A);
    free(B);
    free(X);
    return 0;
}
