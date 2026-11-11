
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGET = priority_queue
TEST_TARGET = test_runner

OBJS = main.o priority_queue.o

TEST_OBJS = priority_queue.o test_priority_queue.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

main.o: main.c priority_queue.h
	$(CC) $(CFLAGS) -c main.c

priority_queue.o: priority_queue.c priority_queue.h
	$(CC) $(CFLAGS) -c priority_queue.c

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_OBJS) -o $(TEST_TARGET)

test_priority_queue.o: test_priority_queue.c priority_queue.h
	$(CC) $(CFLAGS) -c test_priority_queue.c

clean:
	rm -f *.o $(TARGET) $(TEST_TARGET)
