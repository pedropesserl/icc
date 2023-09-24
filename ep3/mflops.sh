#!/bin/bash

METRICA="FLOPS_DP"
CPU=3

xe=$1
entrada=$2
if [[ -z $xe || ! -f $entrada ]]; then
    echo "Usage: ./mflops.sh <xe> <arquivo_entrada> [arquivo_saida]"
    echo "  Comportamento default:"
    echo "      Arquivo arquivo_saida=teste.out se não for passado arquivo_saida"
    exit 0
fi
saida=${3:-"teste.out"}

echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

echo "Compilando programa (sem otimização)."
make purge
make sem_otimizacao

echo "-------------- (sem otimização) --------------" > $saida
echo "Saida da execução:" >> $saida
likwid-perfctr -C ${CPU} -g $METRICA -m ./interpola $xe < $entrada > tmp
awk 'BEGIN {RS="--------------------------------------------------------------------------------\n"} NR==3{print}' tmp >> $saida
echo "Análise LIKWID:" >> $saida
grep -e '| *DP MFLOP' -e '^Region' tmp | \
    awk 'BEGIN{a=""} NR%2==1{sub(",", "", $2); a=a""$2} NR%2==0{print a": "$5" MFLOP/s"; a=""}' >> $saida
echo "-------------- (sem otimização) --------------" >> $saida

echo "Compilando programa (com otimização)."
make purge
make

echo "-------------- (com otimização) --------------" >> $saida
echo "Saida da execução:" >> $saida
likwid-perfctr -C ${CPU} -g $METRICA -m ./interpola $xe < $entrada > tmp
awk 'BEGIN {RS="--------------------------------------------------------------------------------\n"} NR==3{print}' tmp >> $saida
echo "Análise LIKWID:" >> $saida
grep -e '| *DP MFLOP' -e '^Region' tmp | \
    awk 'BEGIN{a=""} NR%2==1{sub(",", "", $2); a=a""$2} NR%2==0{print a": "$5" MFLOP/s"; a=""}' >> $saida
echo "-------------- (com otimização) --------------" >> $saida

rm -rf tmp

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor


