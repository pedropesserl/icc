/* Constantes */

#define DBL_FIELD "%15.10lg"
#define SEP_RES "\n\n\n"

#define DEF_SIZE 128
#define BASE 32

#define ABS(num)  ((num) < 0.0 ? -(num) : (num))
#define isPot2(n) (n && !(n & (n - 1)))

#define UF  4
#define BLK 8
/* Tipos para matrizes e vetores */

typedef double real_t;

typedef real_t * MatRow;
typedef real_t * Vetor;

/* ----------- FUNÇÕES ---------------- */

MatRow geraMatRow (int m, int n, int zerar);
Vetor geraVetor (int n, int zerar);

void liberaVetor (void *vet);

void multMatVet (MatRow mat, Vetor v, int m, int n, Vetor res);
void multMatVet_otimizado(MatRow __restrict__ mat, Vetor __restrict__ v, int m, int n, Vetor __restrict__ res);

void multMatMat(MatRow A, MatRow B, int n, MatRow C);
void multMatMat_otimizado(MatRow __restrict__ A, MatRow __restrict__ B, int n, MatRow __restrict__ C);

void prnMat (MatRow mat, int m, int n);
void prnVetor (Vetor vet, int n);

