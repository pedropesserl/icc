#!/bin/bash

METRICA="FLOPS_DP"
CPU=3

LIKWID_HOME=/home/soft/likwid
CFLAGS="-I${LIKWID_HOME}/include -DLIKWID_PERFMON -mavx -march=native -Wall -Wextra -g"
LFLAGS="-L${LIKWID_HOME}/lib -llikwid -lm"

echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

gcc ${CFLAGS} sistemas_lineares.c -o perfEG ${LFLAGS}
for k in $METRICA
do
    likwid-perfctr -C ${CPU} -g ${k} -m ./perfEG > ${k}_SemOtimiz.log
done

rm -f perfEG
gcc ${CFLAGS} -O3 sistemas_lineares.c -o perfEG ${LFLAGS}
for k in $METRICA
do
    likwid-perfctr -C ${CPU} -g ${k} -m ./perfEG > ${k}_Otimiz.log
done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor
