#include <stdio.h>
#include <stdlib.h>

#define MEM_ERR() do {                                                                              \
        fprintf(stderr, "Erro de alocação de memória: %s:%d (%s)\n", __FILE__, __LINE__, __func__); \
        exit(1);                                                                                    \
    } while (0)

struct Sistema {
    double *data;
    double **A;
    double *B;
    double *X;
    size_t tam;
};

double **cria_matriz(size_t tam, double *data) {
    double **m = (double**)calloc(tam, sizeof(double*));
    if (!m)
        MEM_ERR;
    for (size_t k = 0; k < tam; k++)
        m[k] = &(data[k*tam]);
    return m;
}

struct Sistema cria_sistema(size_t tam) {
    struct Sistema s;
    s.tam = tam;
    s.data = (double*)calloc(tam*tam, sizeof(double));
    if (!s.data)
        MEM_ERR;
    s.B = (double*)calloc(tam, sizeof(double));
    if (!s.B)
        MEM_ERR;
    s.X = (double*)calloc(tam, sizeof(double));
    if (!s.X)
        MEM_ERR;
    for (size_t i = 0; i < tam; i++) {
        for (size_t j = 0; j < tam; j++)
            scanf("%lf", &(s.data[i*tam+j]));
        scanf("%lf", &(s.B[i]));
    }
    s.A = cria_matriz(tam, s.data);
    return s;
}

void destroi_sistema(struct Sistema s) {
    free(s.data);
    free(s.A);
    free(s.B);
    free(s.X);
}

void retrosub(struct Sistema s) {
    for (size_t i = s.tam-1; i >= 0; i--) {
        s.X[i] = s.B[i];
        for (size_t j = i+1; j < s.tam; j++)
            s.X[i] -= s.A[i][j];
        perror("TERMINAR FUNÇÃO retrosub()");
    }
}

void pivoteamento(struct Sistema s) {

}

void pivoteamento_sem_mult(struct Sistema s) {

}

void sem_pivoteamento(struct Sistema s) {

}

int main() {
    size_t tam;
    scanf("%ld", &tam);
    
    struct Sistema s = cria_sistema(tam);


    destroi_sistema(s);
    return 0;
}
