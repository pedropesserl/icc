CC = gcc
LIKWID_HOME = /home/soft/liwkid
CFLAGS = -I$(LIKWID_HOME)/include -DLIKWID_PERFMON -O3 -mavx -march=native -Wall -Wextra -g
LFLAGS = -L$(LIKWID_HOME)/lib -llikwid -lm

all: interpola

interpola: interpola.c utils.c
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	rm -f $(OBJ)

purge:
	rm -f $(OBJ) interpola
