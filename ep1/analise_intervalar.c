#include <stdio.h>
#include <stdlib.h>
#include <math.h> // INFINITY, -INFINITY, nextaftert

// float nextafterf(float x, float y); retorna o proximo float depois de x na direção de y
//     ou seja:
//     nextafterf(x, INFINITY)  retorna o menor float maior que x
//     nextafterf(x, -INFINITY) retorna o maior float menor que x

#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MIN4(a, b, c, d) (MIN2(MIN2(MIN2(a, b), c), d))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define MAX4(a, b, c, d) (MAX2(MAX2(MAX2(a, b), c), d))

#define INTERFMT "[%1.8e , %1.8e]"
#define FMTINTER(intervalo) intervalo.lo, intervalo.up

#define INTREP(f) (*(int*)(&(f)))

typedef struct Intervalo {
    float lo, up;
} Inter_t;

Inter_t soma_inter(Inter_t a, Inter_t b) {
    Inter_t soma;
    soma.lo = a.lo + b.lo;
    soma.up = a.up + b.up;
    return soma;
}

Inter_t sub_inter(Inter_t a, Inter_t b) {
    Inter_t sub;
    sub.lo = a.lo - b.up;
    sub.up = a.up - b.lo;
    return sub;
}

Inter_t mult_inter(Inter_t a, Inter_t b) {
    Inter_t mult;
    /* printf("a.lo: %x\n", INTREP(a.lo)); */
    /* printf("a.up: %x\n", INTREP(a.up)); */
    /* printf("b.lo: %x\n", INTREP(b.lo)); */
    /* printf("b.up: %x\n", INTREP(b.up)); */
    /* float ll = a.lo*b.lo, lu = a.lo*b.up, ul = a.up*b.lo, uu = a.up*b.up; */
    /* printf("%1.8e, %x\n", ll, INTREP(ll)); */
    /* printf("%1.8e, %x\n", lu, INTREP(lu)); */
    /* printf("%1.8e, %x\n", ul, INTREP(ul)); */
    /* printf("%1.8e, %x\n", uu, INTREP(uu)); */
    mult.lo = MIN4(a.lo*b.lo, a.lo*b.up, a.up*b.lo, a.up*b.up);
    mult.up = MAX4(a.lo*b.lo, a.lo*b.up, a.up*b.lo, a.up*b.up);
    return mult;
}

Inter_t div_inter(Inter_t a, Inter_t b) {
    Inter_t div;
    if (b.lo <= 0 && 0 <= b.up) { // intervalo b contém 0
        div.lo = -INFINITY;
        div.up = INFINITY;
        return div;
    }
    float aux = b.lo;
    b.lo = 1/b.up;
    b.up = 1/aux;
    div = mult_inter(a, b);
    return div;
}

int main() {
    Inter_t operandos[1024] = {0};
    char operadores[1024] = {0};

    int i = 0;
    float input;
    while (scanf("%f", &input) != EOF) {
        operandos[i].lo = nextafterf(input, -INFINITY);
        operandos[i].up = nextafterf(input, INFINITY);
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
        printf("%d:\n", j+1);
        printf(INTERFMT" %c "INTERFMT" =\n", FMTINTER(operandos[j]), op, FMTINTER(operandos[j+1]));
        printf(INTERFMT"\n", FMTINTER(result));
        float ea = fabs(result.up - result.lo);
        float er = ea / result.lo;
        int ulps = abs(INTREP(result.up) - INTREP(result.lo));
        printf("EA: %1.8e; ER: %1.8e, ULPs: %d\n\n", ea, er, ulps);

        operandos[j+1] = result;
    }

    return 0;
}
