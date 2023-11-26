#!/bin/bash

set -e

LOGS="./resultados/likwid-logs"
if [ ! -d $LOGS ]; then
    mkdir $LOGS
fi

GRUPOS="FLOPS_DP L3 L2CACHE"
N="64 128 200 256 512 600 800 1024 2000 3000 4096 6000 7000 10000 50000 100000 1000000 10000000 100000000"
VERSOES="v1 v2"

make purge > /dev/null
gcc -o gera_entrada gera_entrada.c
make > /dev/null

echo "Gerando logs do Likwid..."
for grupo in $GRUPOS; do
    for n in $N; do
        for versao in $VERSOES; do
            echo "Gerando log $grupo"_"$n"_$versao""
            echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor
            ./gera_entrada $n | likwid-perfctr -O -C 3 -g $grupo -m v1/ajustePol \
                > $LOGS/$grupo"_"$n"_$versao.log"
            echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor
        done
    done
    echo "Logs de $grupo gerados no diretório $LOGS."
done

make purge > /dev/null
