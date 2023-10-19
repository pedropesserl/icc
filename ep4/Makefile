PROG = matmult
CFILES = $(wildcard *.c)
OBJS = $(CFILES:%.c=%.o)

CC = gcc
CFLAGS = -Wall
LFLAGS = -lm
DISTFILES = *.c *.h README.md Makefile perfctr
DISTDIR = glc22-pfp22

.PHONY: all debug clean purge dist

all: $(PROG)

$(PROG).o: $(PROG).c
	$(CC) $(CFLAGS) -c $<

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

debug: CFLAGS += -g -D_DEBUG_
debug: $(PROG)

$(PROG): $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	@echo "Limpando ...."
	@rm -f *~ *.bak *.tmp core 

purge: clean
	@echo "Faxina ...."
	@rm -f  $(PROG) *.o a.out $(DISTDIR) $(DISTDIR).tar
	@rm -f *.png marker.out

dist: purge
	@echo "Gerando arquivo de distribuição ($(DISTDIR).tar) ..."
	@ln -s . $(DISTDIR)
	@tar -cvf $(DISTDIR).tar $(addprefix ./$(DISTDIR)/, $(DISTFILES))
	@rm -f $(DISTDIR)
