PROG = matmult
CFILES = $(wildcard *.c)
_OBJ = $(CFILES:%.c=%.o)
_DEPS = $(wildcard *.h)
SDIR = src
IDIR = include
OBJ = $(_OBJ:%=$(SDIR)/%)
DEPS = $(_DEPS:%=$(IDIR)/%)

CC = gcc
CFLAGS = -Iinclude -Wall -Wextra
LFLAGS = -lm
DISTFILES = *.c *.h README.md Makefile perfctr graficos.sh
DISTDIR = glc22-pfp22

.PHONY: all debug clean purge dist

all: CFLAGS += -I$(LIKWID_HOME)/include -DLIKWID_PERFMON -mavx2 -march=native -O3
all: LFLAGS += -L$(LIKWID_HOME)/lib -llikwid
all: $(PROG)

$(PROG).o: $(PROG).c
	$(CC) $(CFLAGS) -c $<

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $<

print_tempos: CFLAGS += -DPRINT_TEMPOS
print_tempos: $(PROG)

$(PROG): $(OBJ) 
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	rm -f *~ *.bak *.tmp core $(OBJ)

purge: clean
	rm -f  $(PROG) a.out $(DISTDIR) $(DISTDIR).tar
	rm -f *.png marker.out

dist: purge
	ln -s . $(DISTDIR)
	tar -cvf $(DISTDIR).tar $(addprefix ./$(DISTDIR)/, $(DISTFILES))
	rm -f $(DISTDIR)

