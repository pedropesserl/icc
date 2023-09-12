#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <likwid.h>

#define MEM_ERR do {                                                                                \
        fprintf(stderr, "Erro de alocação de memória: %s:%d (%s)\n", __FILE__, __LINE__, __func__); \
        exit(1);                                                                                    \
    } while (0)

struct Sistema {
    // backup é uma copia do sistema para poder retomar
    // o sistema ao estado inicial após executarum dos metodos
    double *data;
    double *backup;
    double **A;
    double *B;
    double *X;
    double *R;
    size_t ordem;
    int solucao_unica;
};

double timestamp() {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
    return (double)tp.tv_sec*1.0e3 + (double)tp.tv_nsec*1.0e-6;
}

void cria_matriz(size_t tam, double **m, double *data) {
    for (size_t k = 0; k < tam; k++)
        m[k] = &(data[k*tam]);
}

struct Sistema cria_sistema(size_t ordem) {
    struct Sistema s;
    s.solucao_unica = 0;
    s.ordem = ordem;
    double val;

    s.data = (double*)calloc(ordem*(ordem + 1), sizeof(double));
    if (!s.data)
        MEM_ERR;
    s.backup = (double*)calloc(ordem*(ordem + 1), sizeof(double));
    if (!s.backup)
        MEM_ERR;
    s.X = (double*)calloc(ordem, sizeof(double));
    if (!s.X)
        MEM_ERR;
    s.R = (double*)calloc(ordem, sizeof(double));
    if (!s.R)
        MEM_ERR;
    s.A = (double**)calloc(ordem, sizeof(double*));
    if (!s.A)
        MEM_ERR;

    for (size_t i = 0; i < ordem; i++) {
        for (size_t j = 0; j < ordem; j++) {
            scanf("%lf", &val);
            s.data[i*ordem+j] = val;
            s.backup[i*ordem+j] = val;
        }
        scanf("%lf", &val);
        s.data[ordem*ordem+i] = val;
        s.backup[ordem*ordem+i] = val;
    }
    cria_matriz(ordem, s.A, s.data);
    s.B = &(s.data[ordem*ordem]);
    return s;
}

void destroi_sistema(struct Sistema *s) {
    free(s->data);
    free(s->backup);
    free(s->A);
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

void calcula_residuo(struct Sistema *s) {
    double acc_linha;

    if (!s || !s->solucao_unica)
        return;

    for (size_t i = 0; i < s->ordem; i++) {
        acc_linha = 0.0;
        for (size_t j = 0; j < s->ordem; j++)
            acc_linha += s->A[i][j] * s->X[j];

        s->R[i] = acc_linha - s->B[i];
    }
}

// retorna linha com maior valor em módulo na coluna linha_pivo na qual i <= linha_pivo
size_t find_max(struct Sistema *s, size_t linha_pivo) {
    double max = 0;
    size_t id_max = linha_pivo;
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
    if (!s->solucao_unica) {
        printf("O sistema não possui solução única.\n");
        return;
    }
    printf("X = [ ");
    for (size_t i = 0; i < s->ordem; i++) {
        printf("%1.8e ", s->X[i]);
    }
    printf("]\n");
}

void imprime_residuo(struct Sistema *s) {
    if (!s->solucao_unica) {
        printf("O sistema não possui solução única.\n");
        return;
    }

    printf("R = [ ");
    for (size_t i = 0; i < s->ordem; i++) {
        printf("%1.8e ", s->R[i]);
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
            printf("%1.8e ", s->A[i][j]);
        }
        printf("   %1.8e\n", s->B[i]);
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
        // Se a maior for zero, pula e continua
        // É dito se o Sistema tem solução ou não na retrosub
        if (s->A[i][i] == 0)
            continue;

        for (size_t k = i+1; k < s->ordem; k++) {
            m = s->A[k][i] / s->A[i][i];
            s->A[k][i] = 0.0;
            for (size_t j = i+1; j < s->ordem; j++ )
                s->A[k][j] -= s->A[i][j] * m;

            s->B[k] -= s->B[i] * m;
        }
    }

    retrosub(s);
}

void pivoteamento_sem_mult(struct Sistema *s) {
    if (!s)
        return;

    for (size_t i = 0; i < s->ordem; i++) {
        size_t linha_pivo = find_max(s, i);
        troca_linha(s, i, linha_pivo);

        for (size_t k = i+1; k < s->ordem; k++) {
            for (size_t j = i+1; j < s->ordem; j++ )
                s->A[k][j] = s->A[k][j] * s->A[i][i] - s->A[i][j] * s->A[k][i];

            s->B[k] = s->B[k] * s->A[i][i] - s->B[i] * s->A[k][i];
            s->A[k][i] = 0.0;
        }
    }

    retrosub(s);
}

void sem_pivoteamento(struct Sistema *s) {
    size_t linha_pivo;
    if (!s)
        return;

    for (size_t i = 0; i <s->ordem; i++) {
        // Foi feito pivoteamento quando o pivo era 0, pois como conversado
        // com o professor guilherme (06/09) não está especificado o que fazer
        // então fica da escolha do aluno
        if (fabs(s->A[i][i]) < DBL_EPSILON) { // A[i][i] == 0
            linha_pivo = find_max(s, i);
            troca_linha(s, linha_pivo, i);
            // Se a maior for zero, pula e continua
            // É dito se o Sistema tem solução ou não na retrosub
            if (s->A[i][i] == 0)
                continue;
        }

        // eq_i = eq_i / pivo
        for (size_t j = i+1;j< s->ordem; j++) {
                s->A[i][j] /= s->A[i][i];
        }
        s->B[i] /= s->A[i][i];
        s->A[i][i] = 1.0;

        // eq_k = eq_k - eq_i * m
        for (size_t k = i+1; k < s->ordem; k++) {
            for (size_t j = i+1; j< s->ordem; j++) {
                s->A[k][j] -= s->A[i][j] * s->A[k][i]; 
            }
            s->B[k] -= s->B[i]* s->A[k][i];
            s->A[k][i] = 0.0;
        }
    }

    retrosub(s);
}

int main() {
    LIKWID_MARKER_INIT;

    size_t ordem;
    double t;
    scanf("%ld", &ordem);
    
    struct Sistema s = cria_sistema(ordem);

    printf("Solução com pivoteamento:\n");
    LIKWID_MARKER_START("Análise LIKWID: com pivoteamento");
    t = timestamp();
    pivoteamento(&s);
    t = timestamp() - t;
    LIKWID_MARKER_STOP("Análise LIKWID: com pivoteamento");
    imprime_solucao(&s);
    printf("\nTempo da solução: %1.8e ms\n", t);
    // restaurando matriz do sistema
    memcpy(s.data, s.backup, s.ordem*(s.ordem + 1)*sizeof(double));
    cria_matriz(ordem, s.A, s.data);
    // depois de restaurado, calcula o resíduo
    calcula_residuo(&s);
    printf("\nResíduo:\n");
    imprime_residuo(&s);
    printf("\n");

    printf("Solução com pivoteamento, sem multiplicadores:\n");
    LIKWID_MARKER_START("Análise LIKWID: com pivoteamento, sem multiplicadores");
    t = timestamp();
    pivoteamento_sem_mult(&s);
    t = timestamp() - t;
    LIKWID_MARKER_STOP("Análise LIKWID: com pivoteamento, sem multiplicadores");
    imprime_solucao(&s);
    printf("\nTempo da solução: %1.8e ms\n", t);
    // restaurando matriz do sistema
    memcpy(s.data, s.backup, s.ordem*(s.ordem + 1)*sizeof(double));
    cria_matriz(ordem, s.A, s.data);
    // depois de restaurado, calcula o resíduo
    calcula_residuo(&s);
    printf("\nResíduo:\n");
    imprime_residuo(&s);
    printf("\n");

    printf("Solução sem pivoteamento:\n");
    LIKWID_MARKER_START("Análise LIKWID: sem pivoteamento");
    t = timestamp();
    sem_pivoteamento(&s);
    t = timestamp() - t;
    LIKWID_MARKER_STOP("Análise LIKWID: sem pivoteamento");
    imprime_solucao(&s);
    printf("\nTempo da solução: %1.8e ms\n", t);
    // restaurando matriz do sistema
    memcpy(s.data, s.backup, s.ordem*(s.ordem + 1)*sizeof(double));
    cria_matriz(ordem, s.A, s.data);
    // depois de restaurado, calcula o resíduo
    calcula_residuo(&s);
    printf("\nResíduo:\n");
    imprime_residuo(&s);
    printf("\n");

    destroi_sistema(&s);
    
    LIKWID_MARKER_CLOSE;
    return 0;
}
