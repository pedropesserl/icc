CC = gcc
PROGRAM = ajustePol
LIKWID_HOME = /home/soft/likwid
CFLAGS = -Iinclude -Wall -Wextra -g
LFLAGS = -lm
LOGINS = glc22-pfp22

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
	mkdir -p ../$(LOGINS)
	cp -r -t ../$(LOGINS) $(SDIR) $(IDIR) LEIAME makefile script.sh
	tar -czvf $(LOGINS).tar.gz ../$(LOGINS)
	rm -r ../$(LOGINS)

clean:
	rm -f $(OBJ)

purge:
	rm -f $(OBJ) $(PROGRAM)
