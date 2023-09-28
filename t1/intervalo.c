#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h> // INFINITY, -INFINITY, nextaftert
#include <fenv.h>

// double nextafter(double x, double y); retorna o proximo double depois de x na direção de y
//     ou seja:
//     nextafter(x, INFINITY)  retorna o menor double maior que x
//     nextafter(x, -INFINITY) retorna o maior double menor que x

#define MIN2(a, b)       ((a) < (b) ? (a) : (b))
#define MIN4(a, b, c, d) (MIN2(MIN2(MIN2(a, b), c), d))
#define MAX2(a, b)       ((a) > (b) ? (a) : (b))
#define MAX4(a, b, c, d) (MAX2(MAX2(MAX2(a, b), c), d))

#define INTERFMT "[%1.8e , %1.8e]"
#define FMTINTER(intervalo) intervalo.lo, intervalo.up

#define INTREP(f) (*(int*)(&(f)))

typedef struct Intervalo {
    double lo, up;
} Inter_t;

// Teoricamente nao precisa dela pois o fesetround joga para baixo ja.
// Comparando com o resultado do professor ficou exatamente igual para a entrada:
// 2.347e-40 * 0.001 + 1.1e+10 - 0.75e-39 / 0.0
//
// double m(double a){
//     return nextafter(a, -INFINITY);
// }

// Pegar o maior em toda operação vai aumentando o erro e a distancia de
// ulps. Ver como resolver isso com o professor, pois vao ter diversas operações,
// logo erros gigantes.
double M(double a){
    return nextafter(a, INFINITY);
}

Inter_t soma_inter(Inter_t a, Inter_t b) {
    Inter_t soma;
    soma.lo = a.lo + b.lo;
    soma.up = M(a.up + b.up);
    // soma.lo = a.lo + b.lo;
    // soma.up = a.up + b.up;
    return soma;
}

Inter_t sub_inter(Inter_t a, Inter_t b) {
    Inter_t sub;
    sub.lo = a.lo - b.up;
    sub.up = M(a.up - b.lo);
    // sub.lo = a.lo - b.up;
    // sub.up = a.up - b.lo;
    return sub;
}

Inter_t mult_inter(Inter_t a, Inter_t b) {
    Inter_t mult;
    mult.lo = MIN4(a.lo*b.lo, a.lo*b.up, a.up*b.lo, a.up*b.up);
    mult.up = M(MAX4(a.lo*b.lo, a.lo*b.up, a.up*b.lo, a.up*b.up));
    // mult.lo = MIN4(a.lo*b.lo, a.lo*b.up, a.up*b.lo, a.up*b.up);
    // mult.up = MAX4(a.lo*b.lo, a.lo*b.up, a.up*b.lo, a.up*b.up);
    return mult;
}

Inter_t div_inter(Inter_t a, Inter_t b) {
    Inter_t div;
    if (b.lo <= 0 && 0 <= b.up) { // intervalo b contém 0
        div.lo = -INFINITY;
        div.up = INFINITY;
        return div;
    }
    double aux = b.lo;
    b.lo = 1/b.up;
    b.up = 1/aux;
    div = mult_inter(a, b);
    return div;
}

int calcula_ulps(Inter_t f) {
    if (INTREP(f.up) == INTREP(f.lo)) // são o mesmo número
        return 0;
    int lo = INTREP(f.lo) & ~(1<<31); // desligando MSB
    int up = INTREP(f.up) & ~(1<<31);
    if ((INTREP(f.up) >> 31) != (INTREP(f.lo) >> 31)) // um é positivo e o outro é negativo
        return up + lo - 1;
    return abs(up - lo) - 1;
}

int main() {
    // Alterar para usar struct de arrays, mais eficiente com a cache
    fesetround(FE_DOWNWARD);
    Inter_t operandos[1024] = {0};
    char operadores[1024] = {0};

    int i = 0;
    double input;
    while (scanf("%lf", &input) != EOF) {
        operandos[i].lo = input;
        operandos[i].up = M(input);
        // printf("ULPS: %d\n", calcula_ulps(operandos[i]));
        if (scanf(" %c", &(operadores[i++])) == EOF)
            break;
    }

    if (i < 2 || operadores[i-1] != '\0') {
        fprintf(stderr, "Digite um número apropriado de operandos e operadores.\n");
        return 1;
    }

    for (int j = 0; j < i-1; j++) {
        int op = operadores[j];
        Inter_t result;
        switch (op) {
            case '+':
                result = soma_inter(operandos[j], operandos[j+1]);
                break;
            case '-':
                result = sub_inter(operandos[j], operandos[j+1]);
                break;
            case '*':
                result = mult_inter(operandos[j], operandos[j+1]);
                break;
            case '/':
                result = div_inter(operandos[j], operandos[j+1]);
                break;
            default:
                fprintf(stderr, "Operador inválido inserido: %c.\n", op);
                return 1;
        }

        // se os extremos do resultado são iguais, ou seja, ele não é representável,
        // aumentamos o valor do extremo maior
        if (INTREP(result.lo) == INTREP(result.up)) 
            result.up = nextafter(result.up, INFINITY);

        printf("%d:\n", j+1);
        printf(INTERFMT" %c "INTERFMT" =\n", FMTINTER(operandos[j]), op, FMTINTER(operandos[j+1]));
        printf(INTERFMT"\n", FMTINTER(result));
        double ea = fabs(result.up - result.lo);
        double er = ea / result.lo;
        int ulps = calcula_ulps(result);
        printf("EA: %1.8e; ER: %1.8e, ULPs: %d\n\n", ea, er, ulps);

        operandos[j+1] = result;
    }

    return 0;
}
