#include <stdio.h>
#include <stdlib.h>
#include <math.h> // INFINITY, -INFINITY

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
