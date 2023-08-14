#include <stdio.h>
#include <stdlib.h>
#include <math.h> // INFINITY, -INFINITY

// float nextafterf(float x, float y); retorna o proximo float depois de x na direção de y
//     ou seja:
//     nextafterf(x, INFINITY)  retorna o menor float maior que x
//     nextafterf(x, -INFINITY) retorna o maior float menor que x

typedef struct Intervalo {
    float a, b;
} Intervalo;

int main() {
    float operandos[1024] = {0};
    char operadores[1024] = {0};

    int i = 0;
    while (scanf("%f", &(operandos[i])) != EOF) {
        if (scanf(" %c", &(operadores[i++])) == EOF)
            break;
    }

    if (operadores[i-1] != '\0') {
        fprintf(stderr, "Digite um número apropriado de operandos e operadores.\n");
        return 1;
    }

    return 0;
}
