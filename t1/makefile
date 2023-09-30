CC = gcc
PROGRAM = ajustePol
LIKWID_HOME = /home/soft/likwid
CFLAGS = -I$(LIKWID_HOME)/include -DLIKWID_PERFMON -mavx -march=native -Wall -Wextra
LFLAGS = -L$(LIKWID_HOME)/lib -llikwid -lm

_OBJ = main.o intervalo.o sistema_linear.o utils.o
_DEPS = intervalo.h sistema_linear.h utils.h

SDIR = src
IDIR = include
OBJ = $(patsubst %,$(SDIR)/%,$(_OBJ))
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

%.o: %c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(LFLAGS)

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

clean:
	rm -f $(OBJ)

purge:
	rm -f $(OBJ) $(PROGRAM)
