CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_DEFAULT_SOURCE -Iinclude -g
OBJ = src/main.o src/options.o src/file_entry.o src/sort.o src/display.o src/traverse.o
EXEC = ls

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)