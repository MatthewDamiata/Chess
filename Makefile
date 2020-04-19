CFLAGS += -g -Wall -ansi
LDFLAGS += -lm

all: main test

main: main.c io.c moves.c
	gcc $(CFLAGS) main.c io.c moves.c -o chess

test: test.c moves.c chess.c io.c
	gcc -Wall -o test test.c moves.c chess.c io.c -I$(PWD) -L$(PWD) -lrefimpl


.PHONY: clean
clean:
	rm -fv main test *.o
	rm -fv *~ \#*\# *.swp
