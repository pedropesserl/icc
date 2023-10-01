#include <stdio.h>
#include <stdlib.h>
#include "../include/sistema_linear.h"
#include "../include/ajuste_polinomial.h"

struct Sistema_t cria_SL_MQ(size_t ordem, size_t npts,
                            struct Inter_t *xs, struct Inter_t *ys) {
    struct Sistema_t s = cria_sistema(ordem);
    

    return s;
}
