#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"

#define DIFF 0.0

#define NRAND    ((double) rand() / RAND_MAX)  // drand48() 
#define SRAND(a) srand(a) // srand48(a)

// Retorna o valor da função Styblinksi-Tang aplicada num vetor x de 1 dimensão multiplicado por 2.
inline double styblinski_tang(double x) {
    double xiq = x*x;
    return (xiq*xiq - 16*xiq + 5*x);
}

// Integral Monte Carlo da função Styblinski-Tang de n dimensões
double monte_carlo(double a, double b, int n_amostras, int n_dimensoes) {
    double resultado = 0.0;

    printf("Metodo de Monte Carlo.\n");
    printf("a = %lf, b = %lf, n = %d, variaveis = %d\n", a, b, n_amostras, n_dimensoes);

    double t_inicial = timestamp();

    // como a função é uma soma, podemos calcular os valores de cada parâmetro
    // da função independentemente.
    for (int i = 0; i < n_amostras * n_dimensoes; i++) {
        double x = a + ((double)random() / RAND_MAX)*(b - a);
        resultado += styblinski_tang(x);
    }
    // O intervalo é (b-a)^n_dimensões
    for (int d = 0; d < n_dimensoes; d++)
        resultado *= (b-a);
    resultado /= (2*n_amostras);

    double t_final = timestamp();
    printf("Tempo decorrido: %lf ms.\n", t_final - t_inicial);

    return resultado;
}

// Integral pelo método dos retângulos da função Styblinsky-Tang de 2 dimensões
double retangulos_xy(double a, double b, int n_amostras, int n_dimensoes) {
    double h = (b - a)/n_amostras;
    double resultado = 0.0;

    printf("Metodo dos Retangulos.\n");
    printf("a = %lf, b = %lf, n = %d, h = %lg, variaveis = %d\n", a, b, n_amostras, h, n_dimensoes);

    double t_inicial = timestamp();

    // como a função é uma soma idêntica para todas as variáveis, e as variáveis assumem
    // exatamente os mesmos valores nas suas iterações (todas começam em a e são incrementadas
    // de h a cada iteração), podemos apenas calcular a integral para uma variável e
    // multiplicar pelo número de variáveis.
    // método dos retângulos para dimensões além de 2
    double x = a + h/2;
    for (int i = 0; i < n_amostras; i++, x += h)
        resultado += styblinski_tang(x);

    for (int i = 0; i < n_dimensoes-1; i++)
        resultado *= b - a;       // multiplicando pelo tamanho do intervalo elevado ao,
                                  // número de dimensões - 1, para conseguir o "hipervolume"
    resultado *= h * n_dimensoes; // da figura n-dimensional que estamos calculando.
    resultado /= 2.0;

    double t_final = timestamp();
    printf("Tempo decorrido: %lf ms.\n", t_final - t_inicial);

    return resultado;
}


int main(int argc, char **argv) {
    if (argc < 5) {
        fprintf(stderr, "Uso: %s <inicial> <final> <n_amostras> <n_dimensoes>\n", argv[0]);
        return 1;
    }

    double a, b;
    int n_amostras, n_dimensoes;
    sscanf(argv[1], "%lf", &a);
    sscanf(argv[2], "%lf", &b);
    sscanf(argv[3], "%d", &n_amostras);
    sscanf(argv[4], "%d", &n_dimensoes);

    SRAND(20232);

    printf("Resultado da integral pelo método dos retângulos: %lf\n", retangulos_xy(a, b, n_amostras, n_dimensoes));

    printf("Resultado da integral pelo método de Monte Carlo: %lf\n", monte_carlo(a, b, n_amostras, n_dimensoes));

    // CHAMAR FUNÇÕES DE INTEGRAÇÃO E EXIBIR RESULTADOS

    return 0;
}

