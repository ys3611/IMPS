CC      = gcc
CFLAGS  = -Wall -g -pedantic -std=c99

.PHONY: clean

emulate: emulate.o utils.o

#Rules for .o files	
emulate.o: emulate.c utils.h
utils.o: utils.c utils.h

clean:
	rm -f *.o
	rm -f emulate
