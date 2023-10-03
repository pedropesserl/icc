#include <stdio.h>
#include <stdlib.h>
#include <fenv.h>
/* #include <likwid.h> */
#include "utils.h"
#include "intervalo.h"
#include "sistema_linear.h"
#include "ajuste_polinomial.h"

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
    
    rtime_t t_gera_SL = timestamp();
    struct Sistema_t sistema = cria_SL_MQ(n+1, k, xs, ys);
    t_gera_SL = timestamp() - t_gera_SL;

    printf("sistema gerado em preenche_SL_MQ (REMOVER ISSO DEPOIS):\n");
    imprime_sistema(&sistema);

    rtime_t t_solu_SL = timestamp();
    eliminacao_gauss(&sistema);
    t_solu_SL = timestamp() - t_solu_SL;


    struct Inter_t *residuos = (struct Inter_t*)calloc(n+1, sizeof(struct Inter_t));

    for (size_t i = 0; i < n+1; i++)
        printf(INTERFMT" ", FMTINTER(sistema.X[i]));
    printf("\n");
    for (size_t i = 0; i < n+1; i++)
        printf(INTERFMT" ", FMTINTER(residuos[i]));

    printf("%1.8e\n", t_gera_SL);
    printf("%1.8e\n", t_solu_SL);

    destroi_sistema(&sistema);
    free(residuos);
    free(xs);
    free(ys);
    return 0;
}
