CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGET = calc
TEST_TARGET = test_runner

OBJS = main.o calc.o

TEST_OBJS = unity.o calc.o test_calc.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

main.o: main.c calc.h
	$(CC) $(CFLAGS) -c main.c

calc.o: calc.c calc.h
	$(CC) $(CFLAGS) -c calc.c

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(TEST_OBJS) -o $(TEST_TARGET)

unity.o: unity.c unity.h unity_internals.h
	$(CC) $(CFLAGS) -c unity.c

test_calc.o: test_calc.c calc.h unity.h
	$(CC) $(CFLAGS) -c test_calc.c
	
clean:
	del /Q *.o calc.exe test_runner.exe 2>NUL