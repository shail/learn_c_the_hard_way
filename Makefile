CFLAGS=-Wall -g

test:
	bash game.sh

all: ex1 ex3

ex19: object.o

clean:
	rm -rf ex1 ex1.dSYM ex3 ex3.dSYM

.PHONY: all clean
