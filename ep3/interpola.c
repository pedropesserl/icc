#include <stdio.h>
#include "utils.h"
#include <likwid.h>

#define MEM_ERR do { \
        fprintf(stderr, "Erro de alocação de memória: %s:%d (%s)\n", __FILE__, __LINE__, __func__);             \
        exit(1);     \
    } while (0)

struct Ponto {
    double x, fx;
};

void cria_matriz(size_t tam, double **m, double *data) {
    for (size_t k = 0; k < tam; k++)
        m[k] = &(data[k*tam]);
}

int esta_no_intervalo(double x, struct Ponto *tabela, size_t n) {
    double menor = tabela[0].x, maior = tabela[0].x;
    for (size_t i = 1; i < n; i++) {
        if (tabela[i].x > maior)
            maior = tabela[i].x;
        if (tabela[i].x < menor)
            menor = tabela[i].x;
    }
    return menor <= x && x <= maior;
}

// como é garantido que a entrada do programa nunca será um dos xi da tabela,
// podemos calcular o numerador dos li previamente e reutilizá-lo.
double interpolacao_lagrange(double x, struct Ponto *tabela, size_t n) {
    double pre_num = 1.0;
    for (size_t i = 0; i < n; i++)
        pre_num *= x - tabela[i].x;

    double den;
    double px = 0.0;
    for (size_t i = 0; i < n; i++) {
        double xi = tabela[i].x;
        den = x - xi;

        for (size_t j = 0; j < i; j++)
            den *= xi - tabela[j].x;
        for (size_t j = i+1; j < n; j++)
            den *= xi - tabela[j].x;

        double li = pre_num / den;
        px += li * tabela[i].fx;
    }

    return px;
}


double interpolacao_newton(double x, double **dif_div, struct Ponto *tabela, size_t n) {
    // preenche a matriz de diferenças divididas
    // a linha i contém as n-i diferenças divididas de ordem i
    // a diferença dividida de ordem 0 de xj (f[xj]) é igual a f(xj)
    for (size_t j = 0; j < n; j++)
        dif_div[0][j] = tabela[j].fx;

    for (size_t i = 1; i < n; i++)
        for (size_t j = 0; j < n-i; j++)
            dif_div[i][j] = (dif_div[i-1][j+1] - dif_div[i-1][j]) / (tabela[i+j].x - tabela[j].x);
    
    // calcula o valor de px usando a matriz
    double px = 0.0;
    double m = 1.0;
    for (size_t i = 0; i < n; i++) {
        px += dif_div[i][0] * m;
        m *= x - tabela[i].x;
    }

    return px;
}

int main(int argc, char **argv) {
    LIKWID_MARKER_INIT;

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <valor_a_ser_interpolado>\n", argv[0]);
        return 1;
    }

    double xe = strtod(argv[1], NULL);

    size_t n = 0;
    scanf("%ld", &n);

    struct Ponto *tabela = (struct Ponto*)calloc(n, sizeof(struct Ponto));
    if (!tabela) MEM_ERR;
    for (size_t i = 0; i < n; i++)
        scanf("%lf %lf", &(tabela[i].x), &(tabela[i].fx));

    if (!esta_no_intervalo(xe, tabela, n)) {
        fprintf(stderr, "Valor inserido não está no intervalo da tabela de pontos.\n");
        return 2;
    }

    LIKWID_MARKER_START("Metodo_Lagrange");
    real_t t = timestamp();
    double pxe_l = interpolacao_lagrange(xe, tabela, n);
    real_t t_l = timestamp() - t;
    LIKWID_MARKER_STOP("Metodo_Lagrange");
    printf("%1.8e\n", pxe_l);

    // cria matriz para armazenar diferenças divididas no método de newton
    double **dif_div = (double**)calloc(n, sizeof(double*));
    if (!dif_div) MEM_ERR;
    double *data = (double*)calloc(n*n, sizeof(double));
    if (!data) MEM_ERR;
    cria_matriz(n, dif_div, data);

    LIKWID_MARKER_START("Metodo_Newton");
    t = timestamp();
    double pxe_n = interpolacao_newton(xe, dif_div, tabela, n);
    real_t t_n = timestamp() - t;
    LIKWID_MARKER_STOP("Metodo_Newton");
    printf("%1.8e\n", pxe_n);

    printf("%1.8e\n", t_l);
    printf("%1.8e\n", t_n);

    free(data);
    free(dif_div);
    free(tabela);

    LIKWID_MARKER_CLOSE;
    return 0;
}
