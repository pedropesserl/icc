CC = gcc
PROGRAM = ajustePol
LIKWID_HOME = /home/soft/likwid
CFLAGS = -Wall -Wextra -g
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
all: v1
all: v2

v1: CFLAGS += -Iv1/include
v1: v1/src/main.o v1/src/intervalo.o v1/src/sistema_linear.o v1/src/ajuste_polinomial.o v1/src/utils.o
	$(CC) -o v1/$(PROGRAM) $^ $(LFLAGS)

v2: CFLAGS += -Iinclude
v2: $(OBJ)
	$(CC) -o $(PROGRAM) $^ $(LFLAGS)

zip: purge
	mkdir -p ../$(LOGINS)
	cp -r -t ../$(LOGINS) $(SDIR) $(IDIR) LEIAME makefile script.sh
	tar -czvf $(LOGINS).tar.gz ../$(LOGINS)
	rm -r ../$(LOGINS)

clean:
	rm -f $(OBJ) v1/src/*.o

purge:
	rm -f $(OBJ) v1/src/*.o $(PROGRAM) v1/$(PROGRAM)
