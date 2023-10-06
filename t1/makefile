CC = gcc
PROGRAM = ajustePol
LIKWID_HOME = /home/soft/likwid
CFLAGS = -Iinclude -Wall -Wextra -g
LFLAGS = -lm

_OBJ = main.o intervalo.o sistema_linear.o ajuste_polinomial.o utils.o
_DEPS = intervalo.h sistema_linear.h ajuste_polinomial.h utils.h

SDIR = src
IDIR = include
OBJ = $(_OBJ:%=$(SDIR)/%)
DEPS = $(_DEPS:%=$(IDIR)/%)

%.o: %c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(LFLAGS)

all: CFLAGS += -I$(LIKWID_HOME)/include -DLIKWID_PERFMON -mavx -march=native -O3
all: LFLAGS += -L$(LIKWID_HOME)/lib -llikwid
all: $(PROGRAM)

sem_otimizacao: CFLAGS += -I$(LIKWID_HOME)/include -DLIKWID_PERFMON -mavx -march=native
sem_otimizacao: LFLAGS += -L$(LIKWID_HOME)/lib -llikwid
sem_otimizacao: $(PROGRAM)

debug: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

zip: purge
	mkdir -p glc22-pfp22
	cp $(SDIR)/* $(IDIR)/* LEIAME makefile glc22-pfp22

clean:
	rm -f $(OBJ)

purge:
	rm -f $(OBJ) $(PROGRAM)
