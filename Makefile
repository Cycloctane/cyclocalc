CC = gcc
TARGET = cyclocalc
TEST_TARGET = $(TARGET)_test
SRC = lexer.c calc.c main.c
TEST_SRC = lexer.c calc.c test_cyclocalc.c
CFLAGS = -Wall
TEST_FLAGS = $(CFLAGS) -g -fsanitize=address -std=c99

$(TARGET):
	$(CC) $(CFLAGS) $(SRC) -o $@

$(TEST_TARGET):
	$(CC) $(TEST_FLAGS) $(TEST_SRC) -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

all: test cyclocalc

clean:
	rm -f $(TARGET) $(TEST_TARGET)
