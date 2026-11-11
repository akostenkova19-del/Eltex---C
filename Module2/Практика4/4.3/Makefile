CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGET = phoneBook
TEST_TARGET = test_runner

OBJS = main.o phoneBook.o

TEST_OBJS = unity.o phoneBook.o test_phoneBook.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

main.o: main.c phoneBook.h
	$(CC) $(CFLAGS) -c main.c

phoneBook.o: phoneBook.c phoneBook.h
	$(CC) $(CFLAGS) -c phoneBook.c

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(TEST_OBJS) -o $(TEST_TARGET)

unity.o: unity.c unity.h unity_internals.h
	$(CC) $(CFLAGS) -c unity.c

test_phoneBook.o: test_phoneBook.c phoneBook.h unity.h
	$(CC) $(CFLAGS) -c test_phoneBook.c
	
clean:
	rm -f *.o $(TARGET)