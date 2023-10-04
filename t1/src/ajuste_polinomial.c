# Authos: Gabriel Lisboa Conegero (GRR20221255) e Pedro Folloni Pesserl (GRR20220072)
#include <stdio.h>
#include <stdlib.h>
#include "sistema_linear.h"
#include "ajuste_polinomial.h"

struct Inter_t *tabela_potencias_xs(size_t m, size_t npts, struct Inter_t *xs, struct Inter_t **pots_xs) {
    struct Inter_t *data = (struct Inter_t*)calloc((2*m+1)*npts, sizeof(struct Inter_t));
    if (!data)
        MEM_ERR;
    cria_matriz(npts, 2*m+1, pots_xs, data);
    for (size_t i = 0; i < npts; i++) {
        struct Inter_t pot_atual = UM_INTER;
        for (size_t j = 0; j <= 2*m; j++) {
            pots_xs[i][j] = pot_atual;
            pot_atual = mult_inter(pot_atual, xs[i]);
        }
    }
    return data;
}

struct Sistema_t cria_SL_MQ(size_t ordem, size_t npts,
                            struct Inter_t **pots_xs, struct Inter_t *ys) {
    struct Sistema_t s = cria_sistema(ordem);
    
    // preenchendo primeira linha da matriz do sistema e coluna de termos independentes
    for (size_t k = 0; k < s.ordem; k++) {
        struct Inter_t soma_xs = ZERO_INTER;
        struct Inter_t soma_ys_xs = ZERO_INTER;
        for (size_t i = 0; i < npts; i++) {
            struct Inter_t pot_xi_k = pots_xs[i][k];
            soma_xs = soma_inter(soma_xs, pot_xi_k);
            soma_ys_xs = soma_inter(soma_ys_xs, mult_inter(ys[i], pot_xi_k));
        }
        s.A[0][k] = soma_xs;
        s.B[k] = soma_ys_xs;
    }

    // completando última coluna da matriz do sistema
    for (size_t k = 1; k < s.ordem; k++) {
        struct Inter_t soma_xs = ZERO_INTER;
        for (size_t i = 0; i < npts; i++)
            soma_xs = soma_inter(soma_xs, pots_xs[i][k+s.ordem-1]);
        s.A[k][s.ordem-1] = soma_xs;
    }
    
    // completando valores simétricos do sistema
    for (size_t i = 1; i < s.ordem; i++)
        for (size_t j = 0; j < s.ordem-1; j++)
            s.A[i][j] = s.A[i-1][j+1];

    return s;
}

struct Inter_t *calcula_residuo(struct Sistema_t *s, size_t npts,
                                struct Inter_t **pots_xs, struct Inter_t *ys) {
    struct Inter_t *r = (struct Inter_t*)calloc(npts, sizeof(struct Inter_t));
    for (size_t i = 0; i < npts; i++) {
        struct Inter_t res = ZERO_INTER;
        for (size_t j = 0; j < s->ordem; j++)
            res = soma_inter(res, mult_inter(s->X[j], pots_xs[i][j]));
        r[i] = sub_inter(res, ys[i]);
    }
    return r;
}
