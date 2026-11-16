CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: calc libs/add.so libs/sub.so libs/mul.so libs/divide.so

calc: main.o operation_loader.o
	$(CC) $(CFLAGS) main.o operation_loader.o -o calc -ldl

main.o: main.c operation.h operation_loader.h
	$(CC) $(CFLAGS) -c main.c

operation_loader.o: operation_loader.c operation_loader.h operation.h
	$(CC) $(CFLAGS) -c operation_loader.c

libs:
	mkdir -p libs

libs/add.so: add.c | libs
	$(CC) $(CFLAGS) -fPIC -shared add.c -o $@

libs/sub.so: sub.c | libs
	$(CC) $(CFLAGS) -fPIC -shared sub.c -o $@

libs/mul.so: mul.c | libs
	$(CC) $(CFLAGS) -fPIC -shared mul.c -o $@

libs/divide.so: divide.c | libs
	$(CC) $(CFLAGS) -fPIC -shared divide.c -o $@

clean:
	rm -f calc *.o libs/*.so

.PHONY: all clean
