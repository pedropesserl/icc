#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <time.h>

#define MEM_ERR do {                                                                                \
        fprintf(stderr, "Erro de alocação de memória: %s:%d (%s)\n", __FILE__, __LINE__, __func__); \
        exit(1);                                                                                    \
    } while (0)

struct Sistema {
    double *data;
    double **A;
    double *B;
    double *X;
    size_t ordem;
    int solucao_unica;
};

double **cria_matriz(size_t tam, double *data) {
    double **m = (double**)calloc(tam, sizeof(double*));
    if (!m)
        MEM_ERR;
    for (size_t k = 0; k < tam; k++)
        m[k] = &(data[k*tam]);
    return m;
}

struct Sistema cria_sistema(size_t ordem) {
    struct Sistema s;
    s.solucao_unica = -1;
    s.ordem = ordem;
    s.data = (double*)calloc(ordem*ordem, sizeof(double));
    if (!s.data)
        MEM_ERR;
    s.B = (double*)calloc(ordem, sizeof(double));
    if (!s.B)
        MEM_ERR;
    s.X = (double*)calloc(ordem, sizeof(double));
    if (!s.X)
        MEM_ERR;
    for (size_t i = 0; i < ordem; i++) {
        for (size_t j = 0; j < ordem; j++)
            scanf("%lf", &(s.data[i*ordem+j]));
        scanf("%lf", &(s.B[i]));
    }
    s.A = cria_matriz(ordem, s.data);
    return s;
}

void destroi_sistema(struct Sistema *s) {
    free(s->data);
    free(s->A);
    free(s->B);
    free(s->X);
}

void retrosub(struct Sistema *s) {
    for (int i = s->ordem-1; i >= 0; i--) {
        s->X[i] = s->B[i];
        for (int j = i+1; (size_t)j < s->ordem; j++)
            s->X[i] -= s->A[i][j] * s->X[j];
        if (fabs(s->A[i][i]) >= DBL_EPSILON) { // s->A[i][i] != 0
            s->X[i] /= s->A[i][i];
        } else {
            s->solucao_unica = 0;
            return;
        }
    }
    s->solucao_unica = 1;
}

void pivoteamento(struct Sistema *s) {
    (void)s;
    fprintf(stderr, "%s:%d %s: NÃO IMPLEMENTADA\n", __FILE__, __LINE__, __func__);
    exit(1);
}

void pivoteamento_sem_mult(struct Sistema *s) {
    (void)s;
    fprintf(stderr, "%s:%d %s: NÃO IMPLEMENTADA\n", __FILE__, __LINE__, __func__);
    exit(1);
}

void sem_pivoteamento(struct Sistema *s) {
    (void)s;
    fprintf(stderr, "%s:%d %s: NÃO IMPLEMENTADA\n", __FILE__, __LINE__, __func__);
    exit(1);
}

void imprime_solucao(struct Sistema s) {
    if (s.solucao_unica != 1) {
        printf("O sistema não possui solução única.\n");
        return;
    }
    printf("X = [ ");
    for (size_t i = 0; i < s.ordem; i++) {
        printf("%.5lf ", s.X[i]);
    }
    printf("]\n");
}

int main() {
    size_t ordem;
    scanf("%ld", &ordem);
    
    struct Sistema s = cria_sistema(ordem);

    retrosub(&s);
    imprime_solucao(s);

    destroi_sistema(&s);
    return 0;
}
