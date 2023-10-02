#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h> // INFINITY, -INFINITY, nextafter, pow
#include <fenv.h>
#include "../include/intervalo.h"

// double nextafter(double x, double y); retorna o proximo double depois de x na direção de y
//     ou seja:
//     nextafter(x, INFINITY)  retorna o menor double maior que x
//     nextafter(x, -INFINITY) retorna o maior double menor que x

// Teoricamente nao precisa dela pois o fesetround joga para baixo ja.
// Comparando com o resultado do professor ficou exatamente igual para a entrada:
// 2.347e-40 * 0.001 + 1.1e+10 - 0.75e-39 / 0.0
//
// double m(double a){
//     return nextafter(a, -INFINITY);
// }

// Pegar o maior em toda operação vai aumentando o erro e a distancia de
// ulps. Ver como resolver isso com o professor, pois vao ter diversas operações,
// logo erros gigantes.
double M(double a){
    return nextafter(a, INFINITY);
}

struct Inter_t soma_inter(struct Inter_t a, struct Inter_t b) {
    struct Inter_t soma;
    soma.lo = a.lo + b.lo;
    soma.up = M(a.up + b.up);
    return soma;
}

struct Inter_t sub_inter(struct Inter_t a, struct Inter_t b) {
    struct Inter_t sub;
    sub.lo = a.lo - b.up;
    sub.up = M(a.up - b.lo);
    return sub;
}

struct Inter_t mult_inter(struct Inter_t a, struct Inter_t b) {
    struct Inter_t mult;
    mult.lo = MIN4(a.lo*b.lo, a.lo*b.up, a.up*b.lo, a.up*b.up);
    mult.up = M(MAX4(a.lo*b.lo, a.lo*b.up, a.up*b.lo, a.up*b.up));
    return mult;
}

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

/* struct Inter_t pot_inter(struct Inter_t a, size_t b) { */
/*     if (b == 0) */
/*         return UM_INTER; */

/*     struct Inter_t pot = a; */
/*     size_t i = b; */
/*     while (i > 1) { */
/*         pot.lo *= pot.lo; */
/*         pot.up *= pot.up; */
/*         i--; */
/*     } */

/*     if (b % 2 == 1 || a.lo >= 0) */
/*         return pot; */

/*     if (a.up < 0) { */
/*         double aux = pot.lo; */
/*         pot.lo = pot.up; */
/*         pot.up = aux; */
/*         return pot; */
/*     } */

/*     pot.up = MAX2(pot.lo, pot.up); */
/*     pot.lo = 0.0; */
/*     return pot; */
/* } */

long calcula_ulps(struct Inter_t f) {
    if (INTREP(f.up) == INTREP(f.lo)) // são o mesmo número
        return 0;
    long lo = INTREP(f.lo) & ~(1<<31); // desligando MSB
    long up = INTREP(f.up) & ~(1<<31);
    if ((INTREP(f.up) >> 31) != (INTREP(f.lo) >> 31)) // um é positivo e o outro é negativo
        return up + lo - 1;
    return labs(up - lo) - 1;
}

int compara_inter(struct Inter_t a, struct Inter_t b) {
    struct Inter_t cmp = sub_inter(a, b);
    // 0 in a - b       => a = b
    // (a - b).up < 0   => a < b
    // (a - b).lo > 0   => a > b
    if (cmp.lo <= 0.0 && 0.0 <= cmp.up)
        return INTER_IGUAL;
    if (cmp.up < 0.0)
        return INTER_MENOR;
    return INTER_MAIOR;
}

struct Inter_t fabs_inter(struct Inter_t a) {
    a.lo = fabs(a.lo);
    a.up = fabs(a.up);

    return a;
}
