CC = gcc
WARNS = -Wall -Wextra -pedantic -Werror
DEFS = -D_POSIX_C_SOURCE
CFLAGS := $(WARNS) $(DEFS) -ansi $(CFLAGS)
TARGS = unseven enseven

all: $(TARGS)

unseven enseven: src/type7.c
	$(CC) -o $@ $(CFLAGS) $<
	
.PHONY: clean
clean:
	rm -f $(TARGS)
