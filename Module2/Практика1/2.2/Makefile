CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: simpleCalc

simpleCalc: calc.o main.o
	$(CC) calc.o main.o -o simpleCalc

main.o: main.c calc.h
	$(CC) $(CFLAGs) -c main.c

calc.o: calc.c calc.h
	$(CC) $(CFLAGs) -c calc.c

clean:
	rm -f *.o simpleCalc