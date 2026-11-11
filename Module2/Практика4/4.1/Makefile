CC = gcc
AR = ar
CFLAGS = -Wall -Wextra -std=c99

TARGET = phoneBook
LIBRARY = libphoneBook.a

OBJS = main.o
LIB_OBJS = phoneBook.o

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBRARY)
	$(CC) $(OBJS) -L. -lphoneBook -o $(TARGET)

main.o: main.c phoneBook.h
	$(CC) $(CFLAGS) -c main.c

phoneBook.o: phoneBook.c phoneBook.h
	$(CC) $(CFLAGS) -c phoneBook.c

$(LIBRARY): $(LIB_OBJS)
	$(AR) rcs $(LIBRARY) $(LIB_OBJS)

clean:
	rm -f *.o $(TARGET) $(LIBRARY)

