CC = gcc
LIKWID_HOME = /home/soft/likwid
CFLAGS = -I$(LIKWID_HOME)/include -DLIKWID_PERFMON -O3 -mavx -march=native -Wall -Wextra -g
LFLAGS = -L$(LIKWID_HOME)/lib -llikwid -lm

all: sistemas_lineares.c
	$(CC) $(CFLAGS) sistemas_lineares.c -o perfEG $(LFLAGS)

purge:
	rm -f perfEG
