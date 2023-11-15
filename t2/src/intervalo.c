// Autores: Gabriel Lisboa Conegero (GRR20221255) e Pedro Folloni Pesserl (GRR20220072)
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h> // INFINITY, -INFINITY, nextafter, pow
#include <fenv.h>
#include "intervalo.h"

struct Inter_t div_inter(struct Inter_t a, struct Inter_t b) {
    struct Inter_t div;
    if (b.lo <= 0 && 0 <= b.up) { // intervalo b contém 0
        div.lo = -INFINITY;
        div.up = INFINITY;
        return div;
    }
    double aux = b.lo;
    b.lo = 1/b.up;
    b.up = 1/aux;
    div = mult_inter(a, b);
    return div;
}

int compara_inter(struct Inter_t a, struct Inter_t b) {
    struct Inter_t cmp = sub_inter(a, b);
    // 0 in (a - b)     => a = b
    if (cmp.lo <= 0.0 && 0.0 <= cmp.up)
        return INTER_IGUAL;

    // (a - b).up < 0   => a < b
    if (cmp.up < 0.0)
        return INTER_MENOR;

    // (a - b).lo > 0   => a > b
    return INTER_MAIOR;
}
