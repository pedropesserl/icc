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
GRUPOS=("FLOPS_DP" "L2CACHE" "L3")
CAMPOS=("^DP" "L2 miss ratio" "L3 bandwidth")
TESTES=("operacoes_aritmeticas_dp" "cache_miss_l2" "banda_de_memoria")
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

for i in {0..2}; do
    rm -f $DADOS/${TESTES[$i]}*
    echo "Formatando dados do teste \"${TESTES[$i]}\"..."
    for n in $N; do
        for versao in $VERSOES; do
            k=1
            for funcao in $FUNCOES; do
                echo -n "$n " >> $DADOS/${TESTES[$i]}"_$funcao"_"$versao.dat"
                grep "${CAMPOS[$i]}" $LOGS/${GRUPOS[$i]}_$n"_$versao.log" | \
                    cut -d, f2 > $DADOS/temp
                sed "${k}q;d" $DADOS/temp >> $DADOS/${TESTES[$i]}_$funcao"_$versao.dat"
                ((k++))
            done
        done
    done
    echo "Pronto."
done

# O teste de FLOPS_DP é especial por precisarmos de dois de seus campos; buscando o segundo
rm -f $DADOS/operacoes_aritmeticas_avx_dp*
echo 'Formatando dados do teste "operacoes_aritmeticas_avx_dp"...'
for n in $N; do
    for versao in $VERSOES; do
        k=1
        for funcao in $FUNCOES; do
            echo -n "$n " >> $DADOS/operacoes_aritmeticas_avx_dp_$funcao"_$versao.dat"
            grep "AVX DP" $LOGS/FLOPS_DP_$n"_$versao.log" | cut -d, -f2 > $DADOS/temp
            sed "${k}q;d" $DADOS/temp >> \
                $DADOS/operacoes_aritmeticas_avx_dp_$funcao"_versao".dat
            ((k++))
        done
    done
done
echo "Pronto."
rm -f $DADOS/temp
