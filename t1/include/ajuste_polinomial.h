#ifndef AJUSTE_POLINOMIAL_H_
#define AJUSTE_POLINOMIAL_H_

#include "intervalo.h"

// Cria o sistema linear para o método dos Mínimos Quadrados
// dada a tabela de pontos e a ordem do sistema (ordem = grau_polinomio + 1)
struct Inter_t *cria_SL_MQ(size_t ordem, size_t npts,
                           struct Inter_t *xs, struct Inter_t *ys);

#endif // AJUSTE_POLINOMIAL_H_
