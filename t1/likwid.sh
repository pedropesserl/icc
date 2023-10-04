#!/bin/bash

METRICA="FLOPS_DP ENERGY"
CPU=3
program=./ajustePol

entrada=$1
if [[ ! -f $entrada ]]; then
    echo "Usage: ./mflops.sh <arquivo_entrada>"
    exit 0
fi

echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

for mtr in $METRICA; do
    make purge > /dev/null
    make sem_otimizacao > /dev/null

    echo "-------------- (sem otimização / Grupo $mtr) --------------"
    echo "Saida da execução:"
    likwid-perfctr -C ${CPU} -g $mtr -m $program < $entrada > tmp
    awk 'BEGIN {RS="--------------------------------------------------------------------------------\n"} NR==3{print}' tmp
    echo "Análise LIKWID:"
    if [[ $mtr == "FLOPS_DP" ]]; then
        grep -e '|.*MFLOP' -e '^Region' tmp | \
            awk 'BEGIN{dl=FS} NR%3==1{print $2;FS="|"} NR%3==0{FS=dl} NR%3!=1{gsub(/^[ \t]+|[ \t]+$/, "", $2);gsub(/^[ \t]+|[ \t]+$/, "", $3);print "\t"$2" = "$3}'
    else
        grep -e '| *Energy \[J\]' -e '^Region' tmp | \
            awk 'BEGIN{dl=FS} NR%2==1{print $2; FS="|"} NR%2==0{FS=dl;gsub(/^[ \t]+|[ \t]+$/, "", $2);gsub(/^[ \t]+|[ \t]+$/, "", $3);print "\t"$2" = "$3}'
    fi
    echo "-------------- (sem otimização / Grupo $mtr) --------------"

    make purge > /dev/null
    make > /dev/null

    echo "-------------- (com otimização / Grupo $mtr) --------------"
    echo "Saida da execução:"
    likwid-perfctr -C ${CPU} -g $mtr -m $program $xe < $entrada > tmp
    awk 'BEGIN {RS="--------------------------------------------------------------------------------\n"} NR==3{print}' tmp
    echo "Análise LIKWID:"
    if [[ $mtr == "FLOPS_DP" ]]; then
        grep -e '|.*MFLOP' -e '^Region' tmp | \
            awk 'BEGIN{dl=FS} NR%3==1{print $2;FS="|"} NR%3==0{FS=dl} NR%3!=1{gsub(/^[ \t]+|[ \t]+$/, "", $2);gsub(/^[ \t]+|[ \t]+$/, "", $3);print "\t"$2" = "$3}'
    else
        grep -e '| *Energy \[J\]' -e '^Region' tmp | \
            awk 'BEGIN{dl=FS} NR%2==1{print $2; FS="|"} NR%2==0{FS=dl;gsub(/^[ \t]+|[ \t]+$/, "", $2);gsub(/^[ \t]+|[ \t]+$/, "", $3);print "\t"$2" = "$3}'
    fi
    echo "-------------- (com otimização / Grupo $mtr) --------------"
done

make purge > /dev/null
rm -rf tmp

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor


