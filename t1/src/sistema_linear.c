// Autores: Gabriel Lisboa Conegero (GRR20221255) e Pedro Folloni Pesserl (GRR20220072)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "sistema_linear.h"

void cria_matriz(size_t nlin, size_t ncol, struct Inter_t **m, struct Inter_t *data) {
    for (size_t k = 0; k < nlin; k++)
        m[k] = &(data[k*ncol]);
}

void cria_matriz_quadrada(size_t tam, struct Inter_t **m, struct Inter_t *data) {
    for (size_t k = 0; k < tam; k++)
        m[k] = &(data[k*tam]);
}

struct Sistema_t cria_sistema(size_t ordem) {
    struct Sistema_t s;
    s.ordem = ordem;

    // Aloca memória para os coeficientes e termos independentes
    s.data = (struct Inter_t*)calloc(ordem*(ordem + 1), sizeof(struct Inter_t));
    if (!s.data)
        MEM_ERR;
    s.X = (struct Inter_t*)calloc(ordem, sizeof(struct Inter_t));
    if (!s.X)
        MEM_ERR;
    s.A = (struct Inter_t**)calloc(ordem, sizeof(struct Inter_t*));
    if (!s.A)
        MEM_ERR;

    cria_matriz_quadrada(ordem, s.A, s.data);
    s.B = &(s.data[ordem*ordem]);
    return s;
}

void destroi_sistema(struct Sistema_t *s) {
    free(s->data);
    free(s->A);
    free(s->X);
}

void retrosub(struct Sistema_t *s) {
    for (int i = s->ordem-1; i >= 0; i--) {
        s->X[i] = s->B[i];
        for (int j = i+1; (size_t)j < s->ordem; j++)
            s->X[i] = sub_inter(s->X[i], mult_inter(s->A[i][j], s->X[j]));
        s->X[i] = div_inter(s->X[i], s->A[i][i]);
    }
}

size_t find_max(struct Sistema_t *s, size_t linha_pivo) {
    struct Inter_t max = ZERO_INTER;
    size_t id_max = linha_pivo;
    if (!s || linha_pivo > s->ordem)
        return 0;
       
    if (linha_pivo == s->ordem - 1) // Se tiver apenas um elemento nas
        return linha_pivo;          // linhas de baixo de linha_pivo

    for (size_t i = linha_pivo; i < s->ordem; i++) {
        // if (|s->A[i][linha_pivo] > max|) then
        if (compara_inter(fabs_inter(s->A[i][linha_pivo]), max) == INTER_MAIOR ) {
            max = fabs_inter(s->A[i][linha_pivo]);
            id_max = i;
        }
    }

    return id_max;
}

int troca_linha(struct Sistema_t *s, size_t linha1, size_t linha2) {
    struct Inter_t *tmp;

    if (!s || linha1 > s->ordem || linha2 > s->ordem)
        return 0;
    
    if (linha1 == linha2) // não trocar se for a mesma linha
        return 1;

    tmp = s->A[linha1];
    s->A[linha1] = s->A[linha2];
    s->A[linha2] = tmp;

    struct Inter_t tmp2;
    tmp2 = s->B[linha1];
    s->B[linha1] = s->B[linha2];
    s->B[linha2] = tmp2;

    return 1;
}

void imprime_solucao(struct Sistema_t *s) {
    printf("X = { ");
    for (size_t i = 0; i < s->ordem; i++) {
        printf(INTERFMT" ", FMTINTER(s->X[i]));
    }
    printf("}\n");
}

void imprime_sistema(struct Sistema_t *s) {
    printf("    ┏\n");
    for (size_t i = 0; i < s->ordem; i++) {
        if (i == s->ordem/2)
            printf("A = ┃ ");
        else
            printf("    ┃ ");
        
        for (size_t j = 0; j < s->ordem; j++)
            printf(INTERFMT" ", FMTINTER(s->A[i][j]));
        printf("   "INTERFMT"\n", FMTINTER(s->B[i]));
    }
    printf("    ┗\n");
}

void eliminacao_gauss(struct Sistema_t *s) {
    struct Inter_t m;

    if (!s)
        return;

    for (size_t i = 0; i < s->ordem; i++) {
        size_t linha_pivo = find_max(s, i);
        troca_linha(s, linha_pivo, i);
        if (s->A[i][i].lo <= 0 && 0 <= s->A[i][i].up)
            continue;

        for (size_t k = i+1; k < s->ordem; k++) {
            m = div_inter(s->A[k][i], s->A[i][i]);
            s->A[k][i] = ZERO_INTER;
            for (size_t j = i+1; j < s->ordem; j++ )
                s->A[k][j] = sub_inter(s->A[k][j], mult_inter(s->A[i][j], m));

            s->B[k] = sub_inter(s->B[k], mult_inter(s->B[i], m));
        }
    }

    retrosub(s);
}
