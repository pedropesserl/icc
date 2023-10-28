PROG = matmult
CFILES = $(wildcard *.c)
OBJS = $(CFILES:%.c=%.o)

CC = gcc
CFLAGS = -Wall
LFLAGS = -lm
DISTFILES = *.c *.h README.md Makefile perfctr
DISTDIR = glc22-pfp22

.PHONY: all debug clean purge dist

all: CFLAGS += -I$(LIKWID_HOME)/include -DLIKWID_PERFMON -mavx2 -march=native -O3
all: LFLAGS += -L$(LIKWID_HOME)/lib -llikwid
all: $(PROG)

$(PROG).o: $(PROG).c
	$(CC) $(CFLAGS) -c $<

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

debug1: CFLAGS += -g -D_DEBUG_1
debug1: $(PROG)

debug2: CFLAGS += -g -D_DEBUG_2
debug2: $(PROG)

$(PROG): $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	rm -f *~ *.bak *.tmp core 

purge: clean
	rm -f  $(PROG) *.o a.out $(DISTDIR) $(DISTDIR).tar
	rm -f *.png marker.out

dist: purge
	ln -s . $(DISTDIR)
	tar -cvf $(DISTDIR).tar $(addprefix ./$(DISTDIR)/, $(DISTFILES))
	rm -f $(DISTDIR)

