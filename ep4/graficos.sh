#!/bin/bash

# set -x

DADOS="./resultados/dados"
LOGS="./resultados/likwid-logs"
GRAFS="./resultados/graficos"

GRUPOS=("" "MEM" "L2CACHE" "ENERGY" "FLOPS_DP")
TESTES=("tempo" "banda_de_memoria" "cache_miss_l2" "energia" "operacoes_aritmeticas_dp" "operacoes_aritmeticas_avx_dp")
CAMPOS=("" "Memory bandwidth [MBytes/s]" "L2 miss ratio" "Energy [J]" "DP [MFLOP/s]" "AVX DP [MFLOP/s]")
FUNCOES="matvet matmat otimizado_matvet otimizado_matmat"
N="64 100 128 200 256 512 600 900 1024 2000 2048 3000 4000"

echo "Gerando logs do Likwid..."
for grupo in ${GRUPOS[@]}; do
    for n in $N; do
        make purge > /dev/null
        make > /dev/null
        ./perfctr $grupo ./matmult $i > $LOGS/$grupo"_"$i".log"
    done
    echo "Logs de $grupo gerados no diretorio $LOGS."
done

############################################################################

echo 'Realizando teste "tempo"...'
rm -f $DADOS/tempo*
make purge > /dev/null
make print_tempos > /dev/null
for n in $N; do
    ./matmult $n > $DADOS/temp
    j=1
    for funcao in $FUNCOES; do
        echo -n "$n " >> $DADOS/tempo_$funcao.dat
        sed "${j}q;d" $DADOS/temp >> $DADOS/tempo_$funcao.dat
        ((j++))
    done
done
rm -f $DADOS/temp
echo "Pronto."

for i in {1..4}; do
    rm $DADOS/${TESTES[$i]}*
    echo "Formatando dados do teste \"${TESTES[$i]}\"..."
    for n in $N; do
        k=1
        for funcao in $FUNCOES; do
            echo -n "$n " >> $DADOS/${TESTES[$i]}_$funcao.dat
            grep -F "${CAMPOS[$i]}" $LOGS/${GRUPOS[$i]}_$n.log | cut -d, -f2 > $DADOS/temp
            sed "${k}q;d" $DADOS/temp >> $DADOS/${TESTES[$i]}_$funcao.dat
            ((k++))
        done
    done
    echo "Pronto."
done
# O teste de FLOPS_DP é especial por precisarmos de dois de seus campos; buscando o segundo
rm -f $DADOS/operacoes_aritmeticas_avx_dp*
echo 'Formatando dados do teste "operacoes_aritmeticas_avx_dp"...'
for n in $N; do
    k=1
    for funcao in $FUNCOES; do
        echo -n "$n " >> $DADOS/operacoes_aritmeticas_avx_dp_$funcao.dat
        grep -F "AVX DP [MFLOP/s]" $LOGS/FLOPS_DP_$n.log | cut -d, -f2 > $DADOS/temp
        sed "${k}q;d" $DADOS/temp >> $DADOS/operacoes_aritmeticas_avx_dp_$funcao.dat
        ((k++))
    done
done
echo "Pronto."
rm -f $DADOS/temp

############################################################################

echo "Gerando gráficos com gnuplot..."

rm -f $GRAFS/*
F="matvet matmat"
YLABELS=("Tempo (ms)" "Banda de memória (MB/s)" "L2 miss ratio" "Energia (J)")
for i in {0..3}; do
    for funcao in $F; do
        gnuplot -persist <<- EOFMarker
            set ylabel "${YLABELS[$i]}"
            set xlabel "Ordem da matriz"
            set terminal png size 800,600
            set output "$GRAFS/${TESTES[$i]}_$funcao.png"
            plot "$DADOS/${TESTES[$i]}_$funcao.dat" title "$funcao" lt 7 lc 7 w lp, "$DADOS/${TESTES[$i]}_otimizado_$funcao.dat" title "$funcao - otimizado" lt 7 lc 6 w lp
EOFMarker
    done
done

# Caso especial: Operações aritméticas
for funcao in $F; do
    gnuplot -persist <<- EOFMarker
        set ylabel "MFLOP/s"
        set xlabel "Ordem da matriz"
        set terminal png size 800,600
        set output "$GRAFS/operacoes_aritmeticas_$funcao.png"
        plot "$DADOS/operacoes_aritmeticas_dp_$funcao.dat" title "DP: $funcao" lt 7 lc 7 w lp, "$DADOS/operacoes_aritmeticas_dp_otimizado_$funcao.dat" title "DP: $funcao - otimizado" lt 7 lc 6 w lp, "$DADOS/operacoes_aritmeticas_avx_dp_$funcao.dat" title "AVX: $funcao" lt 7 lc 2 w lp, "$DADOS/operacoes_aritmeticas_avx_dp_otimizado_$funcao.dat" title "AVX: $funcao - otimizado" lt 7 lc 1 w lp
EOFMarker
done

echo "Gráficos gerados no diretório $GRAFS."