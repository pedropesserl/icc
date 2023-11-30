// Autores: Gabriel Lisboa Conegero (GRR20221255) e Pedro Folloni Pesserl (GRR20220072)
#ifndef AJUSTE_POLINOMIAL_H_
#define AJUSTE_POLINOMIAL_H_

#include "sistema_linear.h"

struct Ponto_t {
    struct Inter_t x, y;
};

// Cria o sistema linear para o método dos Mínimos Quadrados.
// Dados os valores:
//  ordem: grau_polinomio + 1
//  npts: número de pontos
//  pts: vetor de pontos
struct Sistema_t cria_SL_MQ(size_t ordem, size_t npts, struct Ponto_t *pts);

// Calcula o resíduo relativo ao polinômio gerado e à tabela original de pontos
// Dados os valores:
//  s: Estrutura que guarda informações do SL
//  npts: número de pontos
//  pts: vetor de pontos
struct Inter_t *calcula_residuo(struct Sistema_t *s, size_t npts, struct Ponto_t *pts);

#endif // AJUSTE_POLINOMIAL_H_
