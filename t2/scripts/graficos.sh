#!/bin/bash

set -e

DADOS="./resultados/dados"
GRAFS="./resultados/graficos"
if [ ! -d $GRAFS ]; then
    mkdir $GRAFS
fi

FUNCOES=("gera_sl" "resolve_sl" "calcula_residuos")
NOMES_FUNCOES=("gera\\\_sl" "resolve\\\_sl" "calcula\\\_residuos")

rm -f $GRAFS/*
echo "Gerando gráficos com gnuplot..."

# Tempo
for i in {0..2}; do
    gnuplot -persist <<- EOFMarker
        set logscale
        set ylabel "Tempo (ms)"
        set xlabel "Número de pontos"
        set terminal png size 900,500
        set output "$GRAFS/tempo_${FUNCOES[$i]}.png"
        plot "$DADOS/tempo_${FUNCOES[$i]}_v1.dat" title "${NOMES_FUNCOES[$i]} - v1" lt 7 lc 7 w lp, "$DADOS/tempo_${FUNCOES[$i]}_v2.dat" title "${NOMES_FUNCOES[$i]} - v2" lt 7 lc 6 w lp
EOFMarker
done

# Operações aritméticas
for i in {0..2}; do
    gnuplot -persist <<- EOFMarker
        set logscale x
        set ylabel "MFLOP/s"
        set xlabel "Número de pontos"
        set terminal png size 900,500
        set output "$GRAFS/operacoes_aritmeticas_${NOMES_FUNCOES[$i]}.png"
        plot "$DADOS/operacoes_aritmeticas_dp_${FUNCOES[$i]}_v1.dat" title "DP: ${NOMES_FUNCOES[$i]} - v1" lt 7 lc 7 w lp, "$DADOS/operacoes_aritmeticas_dp_${FUNCOES[$i]}_v2.dat" title "DP: ${NOMES_FUNCOES[$i]} - v2" lt 7 lc 6 w lp, "$DADOS/operacoes_aritmeticas_avx_dp_${FUNCOES[$i]}_v1.dat" title "AVX: ${NOMES_FUNCOES[$i]} - v1" lt 7 lc 2 w lp, "$DADOS/operacoes_aritmeticas_avx_dp_${FUNCOES[$i]}_v2.dat" title "AVX: ${NOMES_FUNCOES[$i]} - v2" lt 7 lc 1 w lp
EOFMarker
done

# Banda de Memória e Cache Miss L2
FUNCOES="gera_sl calcula_residuos"
NOMES_FUNCOES=("gera\\\_sl" "calcula\\\_residuos")
TESTES="banda_de_memoria cache_miss_l2"
YLABELS=("Banda de memória (MB/s)" "L2 miss ratio")
i=0
for teste in $TESTES; do
    for j in {0..1}; do
        gnuplot -persist <<- EOFMarker
            set logscale x
            set ylabel "${YLABELS[$i]}"
            set xlabel "Número de pontos"
            set terminal png size 900,500
            set output "$GRAFS/${teste}_${FUNCOES[$j]}.png"
            plot "$DADOS/${teste}_${FUNCOES[$j]}_v1.dat" title "${NOMES_FUNCOES[$j]} - v1" lt 7 lc 7 w lp, "$DADOS/${teste}_${FUNCOES[$j]}_v2.dat" title "${NOMES_FUNCOES[$j]} - v2" lt 7 lc 6 w lp
EOFMarker
    done
    i=$((i+1))
done

echo "Gráficos gerados no diretório $GRAFS."
