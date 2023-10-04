# Authos: Gabriel Lisboa Conegero (GRR20221255) e Pedro Folloni Pesserl (GRR20220072)
#ifndef AJUSTE_POLINOMIAL_H_
#define AJUSTE_POLINOMIAL_H_

#include "sistema_linear.h"

// Cria tabela de lookup para as potências de 0 a 2m de todos os xs (m = ordem - 1)
//
// [ [    1,    x0,  x0^2, ..., x0^2m],
//   [    1,    x1,  x1^2, ..., x1^2m],
//   [    1,    x2,  x2^2, ..., x2^2m],
//   ...
//   [    1,    xn,  xn^2, ..., xn^2m] ]
//
// Retorna vetor de dados alocado
// !! A função caller deve liberar a memória !!
struct Inter_t *tabela_potencias_xs(size_t m, size_t npts, struct Inter_t *xs, struct Inter_t **pots_xs);

// Cria o sistema linear para o método dos Mínimos Quadrados
// dada a tabela de pontos e a ordem do sistema (ordem = grau_polinomio + 1)
struct Sistema_t cria_SL_MQ(size_t ordem, size_t npts,
                            struct Inter_t **pots_xs, struct Inter_t *ys);

// Calcula o resíduo relativo ao polinômio gerado e à tabela original de pontos
struct Inter_t *calcula_residuo(struct Sistema_t *s, size_t npts,
                                struct Inter_t **pots_xs, struct Inter_t *ys);

#endif // AJUSTE_POLINOMIAL_H_
