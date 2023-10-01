#include <stdio.h>
#include <stdlib.h>
#include "../include/sistema_linear.h"
#include "../include/ajuste_polinomial.h"

struct Sistema_t cria_SL_MQ(size_t ordem, size_t npts,
                            struct Inter_t *xs, struct Inter_t *ys) {


    fprintf(stderr, "%s:%d NAO IMPLEMENTADA: %s\n", __FILE__, __LINE__, __func__);



    struct Sistema_t s = cria_sistema(ordem);


    for (size_t i = 0; i < npts; i++) {
        s.B[0] = soma_inter(s.B[0], ys[i]);
    }

    for (size_t i = 0; i < npts; i++) {
        s.B[1] = soma_inter(s.B[1], mult_inter(ys[i], xs[i]));
    }

    for (size_t i = 0; i < npts; i++) {
        s.B[2] = soma_inter(s.B[2], mult_inter(ys[i], xs[i]^2));
    }
    
    struct Inter_t *pots_x =  UM_INTER;
    for (size_t k = 0; k < ordem; k++) {
        for (size_t i = 0; i < npts; i++) {
            s.B[k] = soma_inter(s.B[k], mult_inter(ys[i], pots_x[i]));
        }
        pot_x = mult_inter(pot_x, xs[i]);
    }

    // vetor para fazer o L
    struct Inter_t *aux = (struct Inter_t*)calloc(2*ordem, sizeof(struct Inter_t));
    
    for (size_t i = 0; i < ordem; i++);

    free(aux);

    return s;
}
