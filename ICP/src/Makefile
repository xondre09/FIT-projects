CC=g++
CFLAGS=-std=c++11 -Wall -Wextra -pedantic 

QMAKE=qmake

all: hra2017 hra2017-cli

.PHONY: hra2017
hra2017: Build
	make -f Build

hra2017-cli: card.o game.o undo.o proj2017-cli.o hint.o
	$(CC) $(CFLAGS) $^ -o $@

proj2017-cli.o: proj2017-cli.cc
card.o: card.cc card.h
game.o: game.cc game.h
undo.o: undo.cc undo.h
hint.o: hint.cc hint.h

%.o: %.cc
	$(CC) $(CFLAGS) -c $<

Build: ./proj2017.pro
	$(QMAKE) ./proj2017.pro -o Build -r -spec linux-g++

clean:
	make -f Build clean
	rm -rf *.o

clean-all: clean
	rm hra2017 hra2017-cli Build
