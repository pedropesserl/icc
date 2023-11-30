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
    struct Ponto_t *pts = (struct Ponto_t*)calloc(k, sizeof(struct Ponto_t));
    if (!pts)
        MEM_ERR;

    for (size_t i = 0; i < k; i++) {
        double inx, iny;
        scanf("%lf %lf", &inx, &iny);
        pts[i].x.lo = inx; pts[i].x.up = M(inx);
        pts[i].y.lo = iny; pts[i].y.up = M(iny);
    }

    // Criando SL e inicializando
    rtime_t t_gera_SL = timestamp();
    LIKWID_MARKER_START("Gera_SL");
    struct Sistema_t sistema = cria_SL_MQ(n+1, k, pts);
    LIKWID_MARKER_STOP("Gera_SL");
    t_gera_SL = timestamp() - t_gera_SL;

    // Resolvendo SL
    rtime_t t_solu_SL = timestamp();
    LIKWID_MARKER_START("Resolve_SL");
    eliminacao_gauss(&sistema);
    LIKWID_MARKER_STOP("Resolve_SL");
    t_solu_SL = timestamp() - t_solu_SL;

    // Calculando resíduos
    rtime_t t_residuos = timestamp();
    LIKWID_MARKER_START("Calcula_Residuos");
    struct Inter_t *residuos = calcula_residuo(&sistema, k, pts);
    LIKWID_MARKER_STOP("Calcula_Residuos");
    t_residuos = timestamp() - t_residuos;

    for (size_t i = 0; i < n+1; i++)
        printf(INTERFMT" ", FMTINTER(sistema.X[i]));
    printf("\n");

    printf("%1.8e\n", t_gera_SL);
    printf("%1.8e\n", t_solu_SL);
    printf("%1.8e\n", t_residuos);

    // Liberando memória
    destroi_sistema(&sistema);
    free(residuos);
    free(pts);

    LIKWID_MARKER_CLOSE;
    return 0;
}
