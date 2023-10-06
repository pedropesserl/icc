// Autores: Gabriel Lisboa Conegero (GRR20221255) e Pedro Folloni Pesserl (GRR20220072)
#ifndef AJUSTE_POLINOMIAL_H_
#define AJUSTE_POLINOMIAL_H_

#include "sistema_linear.h"

// Cria tabela de lookup para as potências de 0 a 2m de todos os x's,
// e armazena em pots_xs (espera-se que já esteja alocado).
//
// [ [    1,    x0,  x0^2, ..., x0^2m],
//   [    1,    x1,  x1^2, ..., x1^2m],
//   [    1,    x2,  x2^2, ..., x2^2m],
//   ...
//   [    1,    xn,  xn^2, ..., xn^2m] ]
//
// Retorna vetor de dados alocado.
// Dados os valores:
//  m: grau_polinomio
//  npts: Número de x's
//  xs: Os x's
//  pots_xs: A tabela de potências para preencher.
// !! A função chamadora deve liberar a memória do vetor !!
struct Inter_t *tabela_potencias_xs(size_t m, size_t npts,
                                    struct Inter_t *xs, struct Inter_t **pots_xs);

// Cria o sistema linear para o método dos Mínimos Quadrados.
// Dados os valores:
//  ordem: grau_polinomio + 1
//  npts: número de pontos
//  pots_xs: Tabela da potências dos x's
//  ys: Vetor dos x's
struct Sistema_t cria_SL_MQ(size_t ordem, size_t npts,
                            struct Inter_t **pots_xs, struct Inter_t *ys);

// Calcula o resíduo relativo ao polinômio gerado e à tabela original de pontos
// Dados os valores:
//  s: Estrutura que guarda informações do SL
//  npts: número de pontos
//  pots_xs: Tabela da potências dos x's
//  ys: Vetor dos x's
struct Inter_t *calcula_residuo(struct Sistema_t *s, size_t npts,
                                struct Inter_t **pots_xs, struct Inter_t *ys);

#endif // AJUSTE_POLINOMIAL_H_
