#include <stdio.h>
#include <stdlib.h>
#include "../include/sistema_linear.h"
#include "../include/ajuste_polinomial.h"

void preenche_SL_MQ(struct Sistema_t *s, size_t npts,
                    struct Inter_t *xs, struct Inter_t *ys) {
    // Criando tabela de lookup para as potências de 0 a 2m de todos os xs (m = ordem-1)
    //
    // [ [    1,    x0,  x0^2, ..., x0^2m],
    //   [    1,    x1,  x1^2, ..., x1^2m],
    //   [    1,    x2,  x2^2, ..., x2^2m],
    //   ...
    //   [    1,    xn,  xn^2, ..., xn^2m] ]
    //
    //  ver se é melhor usar a matriz desse jeito ou com as colunas e linhas trocadas
    //
    size_t m = s->ordem - 1;
    struct Inter_t **pots_xs = (struct Inter_t**)calloc(npts, sizeof(struct Inter_t*));
    if (!pots_xs)
        MEM_ERR;
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

    printf("Matriz de potencias gerada em %s (REMOVER ISSO DEPOIS):\n", __func__);
    for (size_t i = 0; i < npts; i++) {
        for (size_t j = 0; j <= 2*m; j++)
            printf(INTERFMT" ", FMTINTER(pots_xs[i][j]));
        printf("\n");
    }

    // preenchendo primeira linha da matriz do sistema e coluna de termos independentes
    for (size_t k = 0; k < s->ordem; k++) {
        struct Inter_t soma_xs = ZERO_INTER;
        struct Inter_t soma_ys_xs = ZERO_INTER;
        for (size_t i = 0; i < npts; i++) {
            struct Inter_t pot_xi_k = pots_xs[i][k];
            soma_xs = soma_inter(soma_xs, pot_xi_k);
            soma_ys_xs = soma_inter(soma_ys_xs, mult_inter(ys[i], pot_xi_k));
        }
        s->A[0][k] = soma_xs;
        s->B[k] = soma_ys_xs;
    }
    //
    // ver qual implementação é mais eficiente
    //
    // for (size_t i = 0; i < npts; i++) {
    //     struct Intervalo_t pot_xi = UM_INTER;
    //     for (size_t k = 0; k < s->ordem; k++) {
    //         s.A[0][k] = soma_inter(s.A[0][k], pot_xi);
    //         s.B[k] = soma_inter(s.B[k], mult_inter(ys[i], pot_xi));
    //         pot_xi = mult_inter(pot_xi, xs[i]);
    //     }
    // }

    // completando última coluna da matriz do sistema
    for (size_t k = 1; k < s->ordem; k++) {
        struct Inter_t soma_xs = ZERO_INTER;
        for (size_t i = 0; i < npts; i++)
            soma_xs = soma_inter(soma_xs, pots_xs[i][k]);
        s->A[k][s->ordem-1] = soma_xs;
    }
    
    // completando valores simétricos do sistema
    for (size_t i = 1; i < s->ordem; i++)
        for (size_t j = 0; j < s->ordem-1; j++)
            s->A[i][j] = s->A[i-1][j+1];

    free(data);
    free(pots_xs);

    /* return s; */
}
