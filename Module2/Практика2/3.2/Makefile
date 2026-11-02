CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = network
SOURCES = main.c ipv4.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

main.o: main.c ipv4.h
	$(CC) $(CFLAGS) -c main.c

ipv4.o: ipv4.c ipv4.h
	$(CC) $(CFLAGS) -c ipv4.c

clean:
	rm -f $(OBJECTS) $(TARGET)
