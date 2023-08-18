CC= gcc
LIKWID_HOME= /home/soft/likwid
CFLAGS= -I$(LIKWID_HOME)/include -DLIKWID_PERFMON -O3 -mavx -march=native
LFLAGS= -L$(LIKWID_HOME)/lib -llikwid

all: sistemas_lineares.c
	$(CC) $(CFLAGS) sistemas_lineares.c -o sistemas_lineares $(LFALGS)

run:
	echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor
	likwid-perfctr -C 3 -g FLOPS_DP -m ./sistemas_lineares > sistema.log
	echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

purge:
	rm -f sistemas_lineares
