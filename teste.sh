#!/bin/bash

METRICA="FLOPS_DP"
CPU=3

LIKWID_HOME=/home/soft/likwid
CFLAGS="-I${LIKWID_HOME}/include -DLIKWID_PERFMON"
LFLAGS="-L${LIKWID_HOME}/lib -llikwid"

echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

gcc ${CFLAGS} teste.c -o teste ${LFLAGS}
for k in $METRICA
do
    likwid-perfctr -C ${CPU} -g ${k} -m ./teste >${k}_SemOtimiz.log
done

rm -f teste
gcc ${CFLAGS} -O3 teste.c -o teste ${LFLAGS}
for k in $METRICA
do
    likwid-perfctr -C ${CPU} -g ${k} -m ./teste >${k}_Otimiz.log
done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

