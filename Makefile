CC = gcc
CFLAGS = -g -Wall -std=c11
DEPS = types.h
OBJ = main.o other_commands.o mkdir_splitpath.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) main
