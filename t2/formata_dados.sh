#!/bin/bash

set -e

DADOS="./resultados/dados"
if [ ! -d $DADOS ]; then
    mkdir $DADOS
fi
LOGS="./resultados/likwid-logs"
if [ ! -d $LOGS ]; then
    mkdir $LOGS
fi

N="64"
FUNCOES="gera_sl resolve_sl calcula_residuos"
CAMPOS=("^DP" "L2 miss ratio" "L3 bandwidth")
VERSOES="v1 v2"

echo 'Formatando dados do teste "tempo"...'
rm -f $DADOS/tempo*
for n in $N; do
    for versao in $VERSOES; do
        # Nos arquivos .log, o tempo da geração do SL está na linha 7, da solução
        # na linha 8 e do cálculo dos resíduos na linha 9
        j=7
        for funcao in $FUNCOES; do
            echo -n "$n " >> $DADOS/tempo_$funcao"_$versao.dat"
            # Escolhi pegar a saída contida nos arquivos L3 arbitrariamente
            sed "${j}q;d" $LOGS/L3_$n"_"$versao.log >> $DADOS/tempo_$funcao"_$versao.dat"
            ((j++))
        done
    done
done
echo "Pronto." 

