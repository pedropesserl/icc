#ifndef INTERVALO_H_
#define INTERVALO_H_

#define MIN2(a, b)       ((a) < (b) ? (a) : (b))
#define MIN4(a, b, c, d) (MIN2(MIN2(MIN2(a, b), c), d))
#define MAX2(a, b)       ((a) > (b) ? (a) : (b))
#define MAX4(a, b, c, d) (MAX2(MAX2(MAX2(a, b), c), d))

#define INTERFMT "[%1.8e , %1.8e]"
#define FMTINTER(intervalo) intervalo.lo, intervalo.up
// Uso: printf("Teste: "INTERFMT" final", FMTINTER(intervalo));

#define INTREP(f) (*(int*)(&(f)))

#define INTER_IGUAL 0
#define INTER_MENOR -1
#define INTER_MAIOR 1

#define ZERO_INTER (struct Inter_t){.lo=0.0, .up=0.0}

struct Inter_t {
    double lo, up;
};

double M(double a);

struct Inter_t soma_inter(struct Inter_t a, struct Inter_t b);

struct Inter_t sub_inter(struct Inter_t a, struct Inter_t b); 

struct Inter_t mult_inter(struct Inter_t a, struct Inter_t b); 

struct Inter_t div_inter(struct Inter_t a, struct Inter_t b); 

int calcula_ulps(struct Inter_t f); 

// retorna INTER_IGUAL se a ~= b
// retorna INTER_MENOR se a < b
// retorna INTER_MAIOR se a > b
//
int compara_inter(struct Inter_t a, struct Inter_t b);

struct Inter_t fabs_inter(struct Inter_t a);
#endif
