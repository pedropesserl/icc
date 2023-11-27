// Autores: Gabriel Lisboa Conegero (GRR20221255) e Pedro Folloni Pesserl (GRR20220072)
#ifndef INTERVALO_H_
#define INTERVALO_H_
#include <stdlib.h>
#include <float.h>
#include <math.h> // INFINITY, -INFINITY, nextafter, pow
#include <fenv.h>

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

#define INTER_CONSTRUCT(val) (struct Inter_t){.lo = (val), .up = (val)}
#define ZERO_INTER INTER_CONSTRUCT(0.0)
#define UM_INTER INTER_CONSTRUCT(1.0)

// Strutura que armazena menor(lo) e amior(up) valor de um intervalo
struct Inter_t {
    double lo, up;
};

// Retorna o próximo valor, depois de a, representado por um double
inline double M(double a);

// Soma dois intervalos
inline struct Inter_t soma_inter(struct Inter_t a, struct Inter_t b);

// Subtrai dois intervalos
inline struct Inter_t sub_inter(struct Inter_t a, struct Inter_t b); 

// Multiplica dois intervalos
inline struct Inter_t mult_inter(struct Inter_t a, struct Inter_t b); 

// Divide dois intervalos
struct Inter_t div_inter(struct Inter_t a, struct Inter_t b); 

// Compara dois intervalos
//   Retorna INTER_IGUAL se a ~= b
//   Retorna INTER_MENOR se a < b
//   Retorna INTER_MAIOR se a > b
int compara_inter(struct Inter_t a, struct Inter_t b);

// Retorna o intervalo a porém com valor absoluto
inline struct Inter_t fabs_inter(struct Inter_t a);

// ----------------------- IMPLEMNTAÇÃO INLINE -----------------------
// double nextafter(double x, double y); retorna o proximo double depois de x na direção de y
//     ou seja:
//     nextafter(x, INFINITY)  retorna o menor double maior que x
//     nextafter(x, -INFINITY) retorna o maior double menor que x

inline double M(double a){
    return nextafter(a, INFINITY);
}

inline struct Inter_t soma_inter(struct Inter_t a, struct Inter_t b) {
    return (struct Inter_t){.lo = a.lo + b.lo, .up = M(a.up + b.up)};
}

inline struct Inter_t sub_inter(struct Inter_t a, struct Inter_t b) {
    return (struct Inter_t){
        .lo = a.lo - b.up,
        .up = M(a.up - b.lo)
    };
}

inline struct Inter_t mult_inter(struct Inter_t a, struct Inter_t b) {
    double ll = a.lo * b.lo;
    double lu = a.lo * b.up;
    double ul = a.up * b.lo;
    double uu = a.up * b.up;

    return (struct Inter_t){
        .lo = MIN4(ll, lu, ul, uu),
        .up = M(MAX4(ll, lu, ul, uu))
    };
}

inline struct Inter_t fabs_inter(struct Inter_t a) {
    return (struct Inter_t){
        .lo = fabs(a.lo),
        .up = fabs(a.up)
    };
}

#endif // INTERVALO_H_
