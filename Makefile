CC = gcc
CFLAGS = -Wall -Wextra -g
OBJ = main.o ls_core.o
EXEC = myls

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(EXEC)