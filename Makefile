CC = gcc
TARGET = cyclocalc
TEST_TARGET = $(TARGET)_test
CFLAGS = -Wall -Werror -O2
TEST_FLAGS = -Wall -Werror -Og -g -fsanitize=address -std=c99

.PHONY: test all clean

default: all

$(TARGET):
	$(CC) $(CFLAGS) lexer.c calc.c main.c -o $@

$(TEST_TARGET):
	$(CC) $(TEST_FLAGS) lexer.c calc.c test_cyclocalc.c -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

all: test $(TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)
