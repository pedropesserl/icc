CC = gcc
PROGRAM = ajustePol
LIKWID_HOME = /home/soft/likwid
CFLAGS = -Wall -Wextra -O3
LFLAGS = -lm
LOGINS = glc22-pfp22

_OBJ = main.o intervalo.o sistema_linear.o ajuste_polinomial.o utils.o
_DEPS = intervalo.h sistema_linear.h ajuste_polinomial.h utils.h

SDIR = src
IDIR = include

OBJ_V1 = $(_OBJ:%=v1/$(SDIR)/%)
DEPS_V1 = $(_DEPS:%=v1/$(IDIR)/%)

OBJ_V2 = $(_OBJ:%=v2/$(SDIR)/%)
DEPS_V2 = $(_DEPS:%=v2/$(IDIR)/%)

all: CFLAGS += -I$(LIKWID_HOME)/include -DLIKWID_PERFMON -mavx -march=native -O3
all: LFLAGS += -L$(LIKWID_HOME)/lib -llikwid
all: v1
all: v2

$(OBJ_V1): %.o: %.c $(DEPS_V1)
	$(CC) $(CFLAGS) -c -o $@ $< $(LFLAGS)

$(OBJ_V2): %.o: %.c $(DEPS_V2)
	$(CC) $(CFLAGS) -c -o $@ $< $(LFLAGS)

v1: CFLAGS += -Iv1/include
v1: $(OBJ_V1)
	$(CC) -o v1/$(PROGRAM) $^ $(LFLAGS)

v2: CFLAGS += -Iv2/include
v2: $(OBJ_V2)
	$(CC) -o v2/$(PROGRAM) $^ $(LFLAGS)

zip: purge
	mkdir -p ../$(LOGINS)
	cp -r -t ../$(LOGINS) $(SDIR) $(IDIR) LEIAME makefile script.sh
	tar -czvf $(LOGINS).tar.gz ../$(LOGINS)
	rm -r ../$(LOGINS)

clean:
	rm -f v2/$(SDIR)/*.o v1/$(SDIR)/*.o

purge: clean
	rm -f v2/$(PROGRAM) v1/$(PROGRAM) gera_entrada
