Otimizaões, em ordem do slides
    - [x] Alocar memória contígua.
        - Feito em matriz.c:geraMatRow e matriz.c:geraVetor
    - [ ] Layout de dados.
        - Apenas vetores, logo não tem muito o que fazer. Não são usadas structs.
    - [x] Acesso sem stride, row major order (linha a linha).
        - Feito em matriz.c:geraMatRow (iniciar random), matriz.c:multMatMat,
          matriz.c:multMatVet e matriz.c:prnMat
    - [ ] Tamanho da array 2^k, usar i<<k + j. >
    - [x] Usar memset.
    - [ ] Eliminar subexpressões.
        - Não tem subexpressão que possa ocorre a eliminação.
    - [ ] Eliminar sobre cargass, (sqrt(x) < a => x < a * a)
        - Não tem sobrecargas que possa ocorre a eliminação.
    - [ ] Usar tabela lookup, pequena.
        - Não precisa de tabela.
    - [ ] Evitar desvios em laços.
        - Não tem desvios nos laços.
    - [ ] Usar instruções SIMD.
    - [ ] Evitar dependência de dados em laços.
        - Não há dependência de dados.
    - [ ] Usar restrict.
    - [ ] Usar inline.
        - Feito em matriz.c:generateRandomA e matriz.c:generateRandomB. Pouco
          cache miss de instruções pois ambas as funções são inseridas no código
          apenas uma vez.
    - [ ] Evitar potências de 2.
    - [ ] Loop unrool, jam.
    - [ ] Blocking.

