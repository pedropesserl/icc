#!/bin/bash

METRICA="FLOPS_DP"
CPU=3

xe=$1
entrada=$2
if [[ -z $xe || ! -f $entrada ]]; then
    echo "Usage: ./mflops.sh <xe> <arquivo_entrada>"
    exit 0
fi

echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

make purge > /dev/null
make sem_otimizacao > /dev/null

echo "-------------- (sem otimização) --------------"
echo "Saida da execução:"
likwid-perfctr -C ${CPU} -g $METRICA -m ./interpola $xe < $entrada > tmp
awk 'BEGIN {RS="--------------------------------------------------------------------------------\n"} NR==3{print}' tmp
echo "Análise LIKWID:"
grep -e '| *DP MFLOP' -e '^Region' tmp | \
    awk 'BEGIN{a=""} NR%2==1{sub(",", "", $2); a=a""$2} NR%2==0{print a": "$5" MFLOP/s"; a=""}'
echo "-------------- (sem otimização) --------------"

make purge > /dev/null
make > /dev/null

echo "-------------- (com otimização) --------------"
echo "Saida da execução:"
likwid-perfctr -C ${CPU} -g $METRICA -m ./interpola $xe < $entrada > tmp
awk 'BEGIN {RS="--------------------------------------------------------------------------------\n"} NR==3{print}' tmp
echo "Análise LIKWID:"
grep -e '| *DP MFLOP' -e '^Region' tmp | \
    awk 'BEGIN{a=""} NR%2==1{sub(",", "", $2); a=a""$2} NR%2==0{print a": "$5" MFLOP/s"; a=""}'
echo "-------------- (com otimização) --------------"

rm -rf tmp

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor


