CC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -pthread
SOURCES = myfind.c queue.c search.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = myfind

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.c myfind.h queue.h search.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean