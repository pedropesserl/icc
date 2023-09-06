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

// retorna linha com maior valor em módulo na coluna linha_pivo na qual i <= linha_pivo
size_t find_max(struct Sistema *s, size_t linha_pivo) {
    double max = 0;
    size_t id_max = 0;
    if (!s || linha_pivo > s->ordem)
        return 0;
       
    if (linha_pivo == s->ordem - 1) // Se tiver apenas um elemento nas
        return linha_pivo;          //linhas de baixos de linha_pivo

    for (size_t i = linha_pivo; i < s->ordem; i++) {
        if (fabs(s->A[i][linha_pivo]) > max) {
            max = fabs(s->A[i][linha_pivo]);
            id_max = i;
        }
    }

    return id_max;
}

// retorna 1 se tudo ocorrer corretamente
int troca_linha(struct Sistema *s, size_t linha1, size_t linha2) {
    double *tmp;

    if (!s || linha1 > s->ordem || linha2 > s->ordem)
        return 0;
    
    if (linha1 == linha2) // nao trocar se for a memsa linha
        return 1;

    tmp = s->A[linha1];
    s->A[linha1] = s->A[linha2];
    s->A[linha2] = tmp;

    double tmp2;
    tmp2 = s->B[linha1];
    s->B[linha1] = s->B[linha2];
    s->B[linha2] = tmp2;

    return 1;
}

void imprime_solucao(struct Sistema *s) {
    if (s->solucao_unica != 1) {
        printf("O sistema não possui solução única.\n");
        return;
    }
    printf("X = [ ");
    for (size_t i = 0; i < s->ordem; i++) {
        printf("%.5lf ", s->X[i]);
    }
    printf("]\n");
}

void imprime_sistema(struct Sistema *s) {
    printf("    ┏\n");
    for (size_t i = 0; i < s->ordem; i++) {
        if (i == s->ordem/2)
            printf("A = ┃ ");
        else
            printf("    ┃ ");
        
        for (size_t j = 0; j < s->ordem; j++) {
            printf("%.5lf ", s->A[i][j]);
        }
        printf("   %.5lf\n", s->B[i]);
    }
    printf("    ┗\n");
}

// retorna 1 se conseguiu fazer a eliminação
void pivoteamento(struct Sistema *s) {
    double m;

    if (!s)
        return;

    for (size_t i = 0; i < s->ordem; i++) {
        size_t linha_pivo = find_max(s, i);
        troca_linha(s, linha_pivo, i);

        for (size_t k = i+1; k < s->ordem; k++) {
            // Se a maior for zero, pula e continua
            // É dito se o Sistema tem solução ou não na retrosub
            if (s->A[i][i] == 0)
                continue;
            m = s->A[k][i] / s->A[i][i];
            s->A[k][i] = 0.0;
            for (size_t j = i+1; j < s->ordem; j++ )
                s->A[k][j] -= s->A[i][j] * m;

            s->B[k] -= s->B[i] * m;
        }
    }
}

void pivoteamento_sem_mult(struct Sistema *s) {
    if (!s)
        return;

    for (size_t i = 0; i < s->ordem; i++) {
        size_t linha_pivo = find_max(s, i);
        troca_linha(s, linha_pivo, i);

        for (size_t k = i+1; k < s->ordem; k++) {
            // Se a maior for zero, pula e continua
            // É dito se o Sistema tem solução ou não na retrosub
            for (size_t j = i+1; j < s->ordem; j++ )
                s->A[k][j] = s->A[k][j] * s->A[i][i] - s->A[i][j] * s->A[k][i];

            s->B[k] = s->B[k] * s->A[i][i] - s->B[i] * s->A[k][i];
            s->A[k][i] = 0.0;
        }
    }
}

void sem_pivoteamento(struct Sistema *s) {
    (void)s;
    fprintf(stderr, "%s:%d %s: NÃO IMPLEMENTADA\n", __FILE__, __LINE__, __func__);
    exit(1);
}

int main() {
    size_t ordem;
    scanf("%ld", &ordem);
    
    struct Sistema s = cria_sistema(ordem);

    // imprime_sistema(&s);
    pivoteamento_sem_mult(&s);
    imprime_sistema(&s);
    retrosub(&s);
    imprime_solucao(&s);

    destroi_sistema(&s);
    return 0;
}
