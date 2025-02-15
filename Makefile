CC = gcc
CFLAGS = -Wall -Wextra -pthread -std=c99 -Wno-deprecated-declarations
LDFLAGS = -pthread
SOURCES = myfind.c queue.c search.c library.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = myfind

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.c myfind.h queue.h search.h library.h
	$(CC) $(CFLAGS) -c $< -o $@

# Neuer Target: Test-Executable
test: test_executable
	./test_executable

test_executable: test_library.c library.c
	$(CC) $(CFLAGS) test_library.c library.c -o test_executable $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGET) test_executable

.PHONY: all clean test