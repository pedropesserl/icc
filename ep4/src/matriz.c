#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> // Para uso de função 'memset()'

#include "matriz.h"

/**
 * Função que gera valores para para ser usado em uma matriz
 * @param i,j coordenadas do elemento a ser calculado (0<=i,j<n)
*  @return valor gerado para a posição i,j
  */
static inline real_t generateRandomA( unsigned int i, unsigned int j) {
    static real_t invRandMax = 1.0 / (real_t)RAND_MAX;
    return ( (i==j) ? (real_t)(BASE<<1) : 1.0 )  * (real_t)random() * invRandMax;
}

/**
 * Função que gera valores aleatórios para ser usado em um vetor
 * @return valor gerado
 *
 */
static inline real_t generateRandomB() {
    static real_t invRandMax = 1.0 / (real_t)RAND_MAX;
    return (real_t)(BASE<<2) * (real_t)random() * invRandMax;
}



/* ----------- FUNÇÕES ---------------- */

/**
 *  Funcao geraMatRow: gera matriz como vetor único, 'row-oriented'
 * 
 *  @param m     número de linhas da matriz
 *  @param n     número de colunas da matriz
 *  @param zerar se 0, matriz  tem valores aleatórios, caso contrário,
 *               matriz tem valores todos nulos
 *  @return  ponteiro para a matriz gerada
 *
 */

MatRow geraMatRow (int m, int n, int zerar) {
    // Adicionando padding
    int padding = isPot2(n);
    MatRow matriz = (real_t *) malloc(m * (n + padding) * sizeof(real_t));

    if (matriz) {
        if (zerar)
            memset(matriz, 0, m * (n + padding) * sizeof(real_t));
        else
            for (int i=0; i < m; ++i)
                for (int j=0; j < n; ++j)
                    matriz[i*n + j] = generateRandomA(i, j);
    }

    return (matriz);
}


/**
 *  Funcao geraVetor: gera vetor de tamanho 'n'
 * 
 *  @param n  número de elementos do vetor
 *  @param zerar se 0, vetor  tem valores aleatórios, caso contrário,
 *               vetor tem valores todos nulos
 *  @return  ponteiro para vetor gerado
 *
 */

Vetor geraVetor (int n, int zerar) {
    Vetor vetor = (real_t *) malloc(n*sizeof(real_t));

    if (vetor) {
        if (zerar)
            memset(vetor,0,n*sizeof(real_t));
        else
            for (int i=0; i < n; ++i)
                vetor[i] = generateRandomB();
    }

    return (vetor);
}

/**
 *  \brief: libera vetor
 * 
 *  @param  ponteiro para vetor
 *
 */
void liberaVetor (void *vet) {
    if (!vet)
        return;
    free(vet);
}


/**
 *  Funcao multMatVet:  Efetua multiplicacao entre matriz 'mxn' por vetor
 *                       de 'n' elementos
 *  @param mat matriz 'mxn'
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *  @param res vetor que guarda o resultado. Deve estar previamente alocado e com
 *             seus elementos inicializados em 0.0 (zero)
 *  @return vetor de 'm' elementos
 *
 */

void multMatVet(MatRow __restrict__ mat, Vetor __restrict__ v, int m, int n, Vetor __restrict__ res) {
    if (res) {
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j)
                res[i] += mat[i*n + j] * v[j];
    }
}

void multMatVet_otimizado(MatRow __restrict__ mat, Vetor __restrict__ v, int m, int n, Vetor __restrict__ res) {
    int istart, iend, jstart, jend;
    if (!res)
        return;

    for (int ii = 0; ii < m/BLK; ii++) {
        istart = ii*BLK; iend = istart+BLK;
        for (int jj = 0; jj < n/BLK; jj++) {
            jstart = jj*BLK; jend = jstart+BLK;
            // Unroll and Jam
            for (int i=istart; i < iend; i+=UF) {
                for (int j=jstart; j < jend; ++j) {
                    for (int u=0; u < UF; u++){
                        // printf("%d %c\n", i+u+1, 'A'+j);
                        res[i+u] += mat[n*(i+u) + j] * v[j];
                    }
                    // res[i+0] += mat[n*(i+0) + j] * v[j];
                    // res[i+1] += mat[n*(i+1) + j] * v[j];
                    // res[i+2] += mat[n*(i+2) + j] * v[j];
                    // res[i+3] += mat[n*(i+3) + j] * v[j];        
                }
            }
        }
    }

    if (m%BLK != 0){
        for (int i=0; i < m-m%BLK; i+=UF){
            for (int j=n-n%BLK; j < n; j++){
                for (int u=0; u < UF; u++){
                    // printf("%d %c\n", i+u+1, 'A'+j);
                    res[i+u] += mat[n*(i+u) + j] * v[j];
                }
            }
        }
    } // end if(m%BLK)

    if (n%BLK != 0){
        for (int i=m-m%BLK; i < m-m%UF; i+=UF){
            for (int j=0; j < n; j++){
                for (int u=0; u < UF; u++){
                    // printf("%d %c\n", i+u+1, 'A'+j);
                    res[i+u] += mat[n*(i+u) + j] * v[j];
                }
            }
        }
    } // end if(n%BLK)
}


/**
 *  Funcao multMatMat: Efetua multiplicacao de duas matrizes 'n x n' 
 *  @param A matriz 'n x n'
 *  @param B matriz 'n x n'
 *  @param n ordem da matriz quadrada
 *  @param C   matriz que guarda o resultado. Deve ser previamente gerada com 'geraMatRow()'
 *             e com seus elementos inicializados em 0.0 (zero)
 *
 */

void multMatMat (MatRow __restrict__ A, MatRow __restrict__ B, int n, MatRow __restrict__ C) {

    /* Efetua a multiplicação */
    for (int i=0; i < n; ++i)
        for (int j=0; j < n; ++j)
            for (int k=0; k < n; ++k)
                C[i*n+j] += A[i*n+k] * B[k*n+j];
}

void multMatMat_otimizado(MatRow __restrict__ A, MatRow __restrict__ B, int n, MatRow __restrict__ C) {
    int istart, iend, jstart, jend, kstart, kend;

    for (int ii=0; ii < n/BLK; ++ii) {
        istart = ii*BLK; iend = istart+BLK;

        for (int jj=0; jj < n/BLK; ++jj) {
            jstart = jj*BLK; jend = jstart+BLK;

            for (int kk=0; kk < n/BLK; ++kk) {
                kstart = kk*BLK; kend = kstart+BLK;

                for (int i=istart; i < iend; ++i)
                    for (int j=jstart; j < jend; j+=UF)
                        for (int k=kstart; k < kend; ++k){
                            for (int u=0; u < UF; u++)
                                C[i*n+j+u] += A[i*n+k] * B[k*n+j+u];
                            // C[i*n+j+0] += A[i*n+k] * B[k*n+j+0];
                            // C[i*n+j+1] += A[i*n+k] * B[k*n+j+1];
                            // C[i*n+j+2] += A[i*n+k] * B[k*n+j+2];
                            // C[i*n+j+3] += A[i*n+k] * B[k*n+j+3];
                        }
            }
        }
    }

    if (n%BLK != 0) {
        for (int i = 0; i < n; i++) {
            for (int j = n-n%BLK; j < n; j+=UF) {
                for (int k = 0; k < n; k++) {
                    for (int u = 0; u < UF; u++) {
                        C[i*n+j+u] += A[i*n+k] * B[k*n+j+u];
                    }
                }
            }
        }

        for (int i = 0; i < n-n%BLK; i++) {
            for (int j = 0; j < n; j+=UF) {
                for (int k = n-n%BLK; k < n; k++) {
                    for (int u = 0; u < UF; u++) {
                        C[i*n+j+u] += A[i*n+k] * B[k*n+j+u];
                    }
                }
            }
        }

        for (int i = n-n%BLK; i < n; i++) {
            for (int j = 0; j < n; j+=UF) {
                for (int k = 0; k < n; k++) {
                    for (int u = 0; u < UF; u++) {
                        C[i*n+j+u] += A[i*n+k] * B[k*n+j+u];
                    }
                }
            }
        }
    }
}


/**
 *  Funcao prnMat:  Imprime o conteudo de uma matriz em stdout
 *  @param mat matriz
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *
 */

void prnMat (MatRow mat, int m, int n) {
  for (int i=0; i < m; ++i) {
      for (int j=0; j < n; ++j)
          printf(DBL_FIELD, mat[n*i + j]);
      printf("\n");
  }
  printf(SEP_RES);
}

/**
 *  Funcao prnVetor:  Imprime o conteudo de vetor em stdout
 *  @param vet vetor com 'n' elementos
 *  @param n número de elementos do vetor
 *
 */

void prnVetor (Vetor vet, int n) {
    for (int i=0; i < n; ++i)
        printf(DBL_FIELD, vet[i]);
    printf(SEP_RES);
}

