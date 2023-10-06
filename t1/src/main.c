// Autores: Gabriel Lisboa Conegero (GRR20221255) e Pedro Folloni Pesserl (GRR20220072)
#include <stdio.h>
#include <stdlib.h>
#include <fenv.h>
#include <likwid.h>
#include "utils.h"
#include "intervalo.h"
#include "sistema_linear.h"
#include "ajuste_polinomial.h"

int main(void) {
    LIKWID_MARKER_INIT;

    fesetround(FE_DOWNWARD);

    // Inicializando pontos da curva
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

    // Criando SL e inicializando
    rtime_t t_gera_SL = timestamp();
    LIKWID_MARKER_START("Gera_SL");
    // Tabela de potências utilizada na criação do SL e cálculo dos resíduos
    struct Inter_t **pots_xs = (struct Inter_t**)calloc(k, sizeof(struct Inter_t*));
    if (!pots_xs)
        MEM_ERR;
    struct Inter_t *data = tabela_potencias_xs(n, k, xs, pots_xs);
    struct Sistema_t sistema = cria_SL_MQ(n+1, k, pots_xs, ys);
    LIKWID_MARKER_STOP("Gera_SL");
    t_gera_SL = timestamp() - t_gera_SL;

    // Resolvendo SL
    rtime_t t_solu_SL = timestamp();
    LIKWID_MARKER_START("Resolve_SL");
    eliminacao_gauss(&sistema);
    LIKWID_MARKER_STOP("Resolve_SL");
    t_solu_SL = timestamp() - t_solu_SL;

    // Calculando resíduos
    struct Inter_t *residuos = calcula_residuo(&sistema, k, pots_xs, ys);

    for (size_t i = 0; i < n+1; i++)
        printf(INTERFMT" ", FMTINTER(sistema.X[i]));
    printf("\n");
    for (size_t i = 0; i < k; i++)
        printf(INTERFMT" ", FMTINTER(residuos[i]));
    printf("\n");

    printf("%1.8e\n", t_gera_SL);
    printf("%1.8e\n", t_solu_SL);

    // Liberando memória
    destroi_sistema(&sistema);
    free(pots_xs);
    free(data);
    free(residuos);
    free(xs);
    free(ys);

    LIKWID_MARKER_CLOSE;
    return 0;
}
