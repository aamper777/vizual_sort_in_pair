CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SOURCES = main.c sorting_algorithms.c
EXECUTABLE = sorter

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES)

clean:
	rm -f $(EXECUTABLE) *.json

run: $(EXECUTABLE)
	./$(EXECUTABLE)

.PHONY: all clean run