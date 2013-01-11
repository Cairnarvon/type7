CC = gcc
WARNS = -Wall -Wextra -pedantic -Werror
DEFS = -D_POSIX_C_SOURCE
CFLAGS := $(WARNS) $(DEFS) -ansi $(CFLAGS)
TARGS = unseven enseven

all: $(TARGS)

unseven enseven: src/main.c type7.o
	$(CC) -o $@ $(CFLAGS) $^

type7.o: src/type7.c
	$(CC) -c $(CFLAGS) $<

.PHONY: clean
clean:
	rm -f $(TARGS)
