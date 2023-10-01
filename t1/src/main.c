#include <stdio.h>
#include <stdlib.h>
#include <likwid.h>
#include "../include/utils.h"
#include "../include/intervalo.h"
#include "../include/ajuste_polinomial.h"

int main(void) {
    size_t n, k;
    scanf("%ld %ld", &n, &k);
    struct Inter_t *xs = (struct Inter_t*)calloc(k, sizeof(struct Inter_t));
    struct Inter_t *ys = (struct Inter_t*)calloc(k, sizeof(struct Inter_t));
    for (size_t i = 0; i < k; i++)
        scanf("%ld %ld", xs+i, ys+i);

    rtime_t t_gera_SL = timestamp();
    struct Sistema_t sistema = cria_SL_MQ(n+1);
    t_gera_SL = timestamp() - t_gera_SL;

    rtime_t t_solu_SL = timestamp();
    eliminacao_gauss(&sistema);
    t_solu_SL = timestamp() - t_solu_SL;

    calcula_residuo(&sistema);

    for (size_t i = 0; i < k; i++)
        printf(INTERFMT" ", FMTINTER(sistema.X[i]));
    for (size_t i = 0; i < k; i++)
        printf(INTERFMT" ", FMTINTER(sistema.R[i]));
    printf("%1.8e\n", t_gera_SL);
    printf("%1.8e\n", t_solu_SL);

    destroi_sistema(&sistema);
    free(xs);
    free(ys);
    free(polinomio);
    return 0;
}
