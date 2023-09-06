CC = gcc
LIKWID_HOME = /home/soft/likwid
CFLAGS = -I$(LIKWID_HOME)/include -DLIKWID_PERFMON -O3 -mavx -march=native -Wall -Wextra -g
LFLAGS = -L$(LIKWID_HOME)/lib -llikwid -lm
DEVFALGS = -g -Wall -Wextra -lm
DEVFILES = sistemas_lineares


all: sistemas_lineares.c
	$(CC) $(CFLAGS) sistemas_lineares.c -o perfEG $(LFLAGS)

run:
	echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor
	likwid-perfctr -C 3 -g FLOPS_DP -m ./perfEG > sistema.log
	echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

dev: $(DEVFILES)

$(DEVFILES) : % : %.c
	$(CC) $(DEVFALGS) $< -o $@

purge:
	rm -f perfEG $(DEVFILES)
