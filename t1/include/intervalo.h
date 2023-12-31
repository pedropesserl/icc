// Autores: Gabriel Lisboa Conegero (GRR20221255) e Pedro Folloni Pesserl (GRR20220072)
#ifndef INTERVALO_H_
#define INTERVALO_H_

#define MIN2(a, b)       ((a) < (b) ? (a) : (b))
#define MIN4(a, b, c, d) (MIN2(MIN2(MIN2(a, b), c), d))
#define MAX2(a, b)       ((a) > (b) ? (a) : (b))
#define MAX4(a, b, c, d) (MAX2(MAX2(MAX2(a, b), c), d))

#define INTERFMT "[%1.8e , %1.8e]"
#define FMTINTER(intervalo) intervalo.lo, intervalo.up
// Uso: printf("Teste: "INTERFMT" final", FMTINTER(intervalo));

#define INTER_IGUAL 0
#define INTER_MENOR -1
#define INTER_MAIOR 1

#define ZERO_INTER (struct Inter_t){.lo = 0.0, .up = 0.0}
#define UM_INTER (struct Inter_t){.lo = 1.0, .up = 1.0}

// Strutura que armazena menor(lo) e amior(up) valor de um intervalo
struct Inter_t {
    double lo, up;
};

// Retorna o próximo valor, depois de a, representado por um double
double M(double a);

// Soma dois intervalos
struct Inter_t soma_inter(struct Inter_t a, struct Inter_t b);

// Subtrai dois intervalos
struct Inter_t sub_inter(struct Inter_t a, struct Inter_t b); 

// Multiplica dois intervalos
struct Inter_t mult_inter(struct Inter_t a, struct Inter_t b); 

// Divide dois intervalos
struct Inter_t div_inter(struct Inter_t a, struct Inter_t b); 

// Compara dois intervalos
//   Retorna INTER_IGUAL se a ~= b
//   Retorna INTER_MENOR se a < b
//   Retorna INTER_MAIOR se a > b
int compara_inter(struct Inter_t a, struct Inter_t b);

// Retorna o intervalo a porém com valor absoluto
struct Inter_t fabs_inter(struct Inter_t a);

#endif // INTERVALO_H_
