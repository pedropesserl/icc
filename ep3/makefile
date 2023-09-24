CC = gcc
PROGRAM=interpola
LIKWID_HOME = /home/soft/likwid
CFLAGS = -I$(LIKWID_HOME)/include -DLIKWID_PERFMON -mavx -march=native -Wall -Wextra -g
LFLAGS = -L$(LIKWID_HOME)/lib -llikwid -lm

all: CFLAGS += -O3
all: interpola

interpola: interpola.c utils.c
	$(CC) $(CFLAGS) -o $(PROGRAM) $^ $(LFLAGS)

sem_otimizacao: interpola.c utils.c
	$(CC) $(CFLAGS) -o $(PROGRAM) $^ $(LFLAGS)

purge:
	rm -f $(PROGRAM)
