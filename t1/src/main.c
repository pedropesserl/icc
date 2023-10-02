#include <stdio.h>
#include <stdlib.h>
#include <fenv.h>
#include <likwid.h>
#include "../include/utils.h"
#include "../include/intervalo.h"
#include "../include/sistema_linear.h"
#include "../include/ajuste_polinomial.h"

int main(void) {
    fesetround(FE_DOWNWARD);
    size_t n, k;
    scanf("%ld %ld", &n, &k);
    struct Inter_t *xs = (struct Inter_t*)calloc(k, sizeof(struct Inter_t));
    if (!xs)
        MEM_ERR;
    struct Inter_t *ys = (struct Inter_t*)calloc(k, sizeof(struct Inter_t));
    if (!ys)
        MEM_ERR;
    for (size_t i = 0; i < k; i++) {
        double inx, iny;
        scanf("%lf %lf", &inx, &iny);
        xs[i].lo = inx; xs[i].up = M(inx);
        ys[i].lo = iny; ys[i].up = M(iny);
    }
    
    struct Sistema_t sistema = cria_sistema(n+1);
    rtime_t t_gera_SL = timestamp();
    preenche_SL_MQ(&sistema, k, xs, ys);
    t_gera_SL = timestamp() - t_gera_SL;

    printf("sistema gerado em cria_SL_MQ (REMOVER ISSO DEPOIS):\n");
    imprime_sistema(&sistema);

    rtime_t t_solu_SL = timestamp();
    eliminacao_gauss(&sistema);
    t_solu_SL = timestamp() - t_solu_SL;

    calcula_residuo(&sistema);

    for (size_t i = 0; i < n+1; i++)
        printf(INTERFMT" ", FMTINTER(sistema.X[i]));
    printf("\n");
    for (size_t i = 0; i < n+1; i++)
        printf(INTERFMT" ", FMTINTER(sistema.R[i]));
    printf("\n");
    printf("%1.8e\n", t_gera_SL);
    printf("%1.8e\n", t_solu_SL);

    destroi_sistema(&sistema);
    free(xs);
    free(ys);
    return 0;
}
