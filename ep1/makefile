CC = gcc
CFLAGS = -Wall -Wextra -g
OBJ = analise_intervalar.o
DEPS = 

%.o: %c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: analise_intervalar

analise_intervalar: $(OBJ)
	$(CC) -o $@ $^ -lm

clean:
	rm -f $(OBJ)

purge:
	rm -f $(OBJ) analise_intervalar
