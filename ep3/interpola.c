#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

#define MEM_ERR do { \
        fprintf(stderr, "Erro de alocação de memória: %s:%d (%s)\n", __FILE__, __LINE__, __func__);             \
        exit(1);     \
    } while (0)

struct Ponto {
    double x, fx;
};

// presume que os valores de x inseridos estão em ordem crescente
int esta_no_intervalo(double x, struct Ponto *tabela, size_t n) {
    return tabela[0].x <= x && x <= tabela[n-1].x;
}

// como é garantido que a entrada do programa nunca será um dos xi da tabela,
// podemos calcular o numerador dos li previamente e reutilizá-lo.
double interpolacao_lagrange(double x, struct Ponto *tabela, size_t n) {
    double pre_num = 1.0;
    for (size_t i = 0; i < n; i++)
        pre_num *= x - tabela[i].x;

    double px = 0.0;
    for (size_t i = 0; i < n; i++) {
        double xi = tabela[i].x;

        double li = pre_num / (x - xi);
        for (size_t j = 0; j < i; j++)
            li /= xi - tabela[j].x;
        for (size_t j = i+1; j < n; j++)
            li /= xi - tabela[j].x;

        px += li * tabela[i].fx;
    }

    return px;
}

double interpolacao_newton(double x, struct Ponto *tabela, size_t n) {
    double px = 0.0;
    


    return px;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Use exatamente um argumento (valor double).\n");
        return 1;
    }

    double xe = strtod(argv[1], NULL);

    size_t n = 0;
    scanf("%ld", &n);

    struct Ponto *tabela = (struct Ponto*)calloc(n, sizeof(struct Ponto));
    if (!tabela)
        MEM_ERR;
    for (size_t i = 0; i < n; i++)
        scanf("%lf %lf", &(tabela[i].x), &(tabela[i].fx));

    if (!esta_no_intervalo(xe, tabela, n)) {
        fprintf(stderr, "Valor inserido não está no intervalo da tabela de pontos.\n");
        return 2;
    }

    real_t t = timestamp();
    double pxe_l = interpolacao_lagrange(xe, tabela, n);
    real_t t_l = timestamp() - t;
    printf("%1.8e\n", pxe_l);

    t = timestamp();
    double pxe_n = interpolacao_newton(xe, tabela, n);
    real_t t_n = timestamp() - t;
    printf("%1.8e\n", pxe_n);

    printf("%lf\n", t_l);
    printf("%lf\n", t_n);

    free(tabela);

    return 0;
}
