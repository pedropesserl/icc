#!/bin/bash

set -e

DADOS="./resultados/dados"
GRAFS="./resultados/graficos"
if [ ! -d $GRAFS ]; then
    mkdir $GRAFS
fi

FUNCOES="gera_sl resolve_sl calcula_residuos"

echo "Gerando gráficos com gnuplot..."

# Tempo
for funcao in $FUNCOES; do
    gnuplot -persist <<- EOFMarker
        set logscale
        set ylabel "Tempo (ms)"
        set xlabel "Número de pontos"
        set terminal png size 900,700
        set output "$GRAFS/tempo_$funcao.png"
        plot "$DADOS/tempo_${funcao}_v1.dat" title "$funcao - v1" lt 7 lc 7 w lp, "$DADOS/tempo_${funcao}_v2.dat" title "$funcao - v2" lt 7 lc 6 w lp
EOFMarker
done

# Operações aritméticas
for funcao in $FUNCOES; do
    gnuplot -persist <<- EOFMarker
        set logscale x
        set ylabel "MFLOP/s"
        set xlabel "Número de pontos"
        set terminal png size 900,500
        set output "$GRAFS/operacoes_aritmeticas_$funcao.png"
        plot "$DADOS/operacoes_aritmeticas_dp_${funcao}_v1.dat" title "DP: $funcao - v1" lt 7 lc 7 w lp, "$DADOS/operacoes_aritmeticas_dp_${funcao}_v2.dat" title "DP: $funcao - v2" lt 7 lc 6 w lp, "$DADOS/operacoes_aritmeticas_avx_dp_${funcao}_v1.dat" title "AVX: $funcao - v1" lt 7 lc 2 w lp, "$DADOS/operacoes_aritmeticas_avx_dp_${funcao}_v2.dat" title "AVX: $funcao - v2" lt 7 lc 1 w lp
EOFMarker
done

# Banda de Memória e Cache Miss L2
FUNCOES="gera_sl calcula_residuos"
TESTES="banda_de_memoria cache_miss_l2"
YLABELS=("Banda de memória (MB/s)" "L2 miss ratio")
i=0
for teste in $TESTES; do
    for funcao in $FUNCOES; do
        gnuplot -persist <<- EOFMarker
            set logscale x
            set ylabel "${YLABELS[$i]}"
            set xlabel "Número de pontos"
            set terminal png size 900,500
            set output "$GRAFS/${teste}_${funcao}.png"
            plot "$DADOS/${teste}_${funcao}_v1.dat" title "$funcao - v1" lt 7 lc 7 w lp, "$DADOS/${teste}_${funcao}_v2.dat" title "$funcao - v2" lt 7 lc 7 w lp
EOFMarker
    done
    ((i++))
done

echo "Gráficos gerados no diretório $GRAFS."
