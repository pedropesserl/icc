#include <stdio.h>
#include <stdlib.h>

float **cria_matriz(size_t tam, float *data) {
    float **m = (float**)calloc(tam, sizeof(float*));
    if (!m) {
        fprintf(stderr, "Erro de alocação de memória (cria_matriz())\n");
        exit(1);
    }
    for (int k = 0; k < tam; k++)
        m[k] = &(data[k*tam]);
    return m;
}

int main() {
    float data[N*N];

    return 0;
}
