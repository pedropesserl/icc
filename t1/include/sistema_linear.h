// Autores: Gabriel Lisboa Conegero (GRR20221255) e Pedro Folloni Pesserl (GRR20220072)
#ifndef SISTEMA_LINEAR_H_
#define SISTEMA_LINEAR_H_

#include "intervalo.h"

#define MEM_ERR do {                                                                                \
        fprintf(stderr, "Erro de alocação de memória: %s:%d (%s)\n", __FILE__, __LINE__, __func__); \
        exit(1);                                                                                    \
    } while (0)


struct Sistema_t {
    struct Inter_t *data;
    // Todo o sistema, incluindo o termos independentes (B).
    struct Inter_t **A;
    struct Inter_t *B;
    struct Inter_t *X;
    size_t ordem;
};

// Cria referência dos ponteiros de m ao vetor data, para uma matriz nlin*ncol.
void cria_matriz(size_t nlin, size_t ncol, struct Inter_t **m, struct Inter_t *data);

// Cria referência dos ponteiros de m ao vetor data, para uma matriz tam*tam.
void cria_matriz_quadrada(size_t tam, struct Inter_t **m, struct Inter_t *data);

// Aloca memória para um sistema vazio
struct Sistema_t cria_sistema(size_t ordem);

// Libera memória do sistema s
void destroi_sistema(struct Sistema_t *s);

// Aplica a retrossubstituição no sistema s (presume-se que já triangularizado)
void retrosub(struct Sistema_t *s);

// Retorna linha com maior valor em módulo na coluna linha_pivo na qual i <= linha_pivo
size_t find_max(struct Sistema_t *s, size_t linha_pivo);

// Troca duas linhas da matriz do sistema.
// Retorna 1 se tudo ocorrer corretamente, e 0 caso contrário.
int troca_linha(struct Sistema_t *s, size_t linha1, size_t linha2);

void imprime_solucao(struct Sistema_t *s);

void imprime_sistema(struct Sistema_t *s);

// Resolve o sistema pelo método da eliminação de Gauss com pivoteamento parcial.
void eliminacao_gauss(struct Sistema_t *s);

#endif // SISTEMA_LINEAR_H_
