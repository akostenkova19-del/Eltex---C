CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L -I.

TARGET = filecopy
TEST_TARGET = test_runner

OBJS = main.o copy.o
TEST_OBJS = unity.o test_copy.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

main.o: main.c copy.h
	$(CC) $(CFLAGS) -c main.c

copy.o: copy.c copy.h
	$(CC) $(CFLAGS) -c copy.c

test: $(TARGET) $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(TEST_OBJS) -o $(TEST_TARGET)

unity.o: unity.c unity.h unity_internals.h
	$(CC) $(CFLAGS) -c unity.c

test_copy.o: test_copy.c unity.h
	$(CC) $(CFLAGS) -c test_copy.c

clean:
	rm -f *.o $(TARGET) $(TEST_TARGET)
