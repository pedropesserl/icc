// Autores: Gabriel Lisboa Conegero (GRR20221255) e Pedro Folloni Pesserl (GRR20220072)
#include <stdio.h>
#include <stdlib.h>
#include "sistema_linear.h"
#include "ajuste_polinomial.h"

struct Sistema_t cria_SL_MQ(size_t ordem, size_t npts, struct Inter_t *ys,
                                            struct Inter_t *xs) {
    struct Sistema_t s = cria_sistema(ordem);
    
    // Calculando somatorios, iterando pelo vetor de pontos pois ele
    // é muito maior que a matriz que sempre vai ser 5x5
    s.A[0][0] = INTER_CONSTRUCT(npts);
    for (size_t k = 0; k < npts; k++) {
        struct Inter_t pot_xk = xs[k];

        s.B[0] = soma_inter(s.B[0], ys[k]);
        // Primeira linha da matriz e termos independetes
        for (size_t j = 1; j < s.ordem; j++) {
            s.A[0][j] = soma_inter(s.A[0][j], pot_xk);
            s.B[j] = soma_inter(s.B[j], mult_inter(ys[k], pot_xk));

            pot_xk = mult_inter(pot_xk, xs[k]);
        }
        // Ultima coluna da matriz
        for (size_t i = 1; i < s.ordem; i++) {
            s.A[i][s.ordem-1] = soma_inter(s.A[i][s.ordem-1], pot_xk);
            pot_xk = mult_inter(pot_xk, xs[k]);
        }
    }


    // Matriz pequena e um vetor contiguo na memoria
    for (size_t i = 1; i < s.ordem; i++) {
        for (size_t j = 0; j < s.ordem-1; j++) {
            s.A[i][j] = s.A[i-1][j+1];
        }
    }

    return s;
}

struct Inter_t *calcula_residuo(struct Sistema_t *s, size_t npts,
                                struct Inter_t *xs, struct Inter_t *ys) {
    struct Inter_t *r = (struct Inter_t*)calloc(npts, sizeof(struct Inter_t));
    for (size_t i = 0; i < npts; i++) {
        struct Inter_t res = ZERO_INTER;
        struct Inter_t pot_y = UM_INTER;
        for (size_t j = 0; j < s->ordem; j++) {
            res = soma_inter(res, mult_inter(s->X[j], pot_y));
            pot_y = mult_inter(pot_y, xs[i]);
        }
        r[i] = sub_inter(ys[i], res);
    }
    return r;
}
