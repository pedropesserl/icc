CC = gcc
CFLAGS = -Wall -Wextra -O3 -march=native
LDFLAGS = -lm

FILES = utils.c integral.c
PROGRAM = integral

all: $(FILES)
	$(CC) $(CFLAGS) $^ -o $(PROGRAM) $(LDFLAGS)

clean:
	rm -rf $(PROGRAM)
