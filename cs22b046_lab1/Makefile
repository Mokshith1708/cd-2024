# Compiler and flags
CC = gcc
LEX = lex
CFLAGS = -ll

# Default value for fname (can be overridden by command line)
fname ?= default

# Rule to build the executable named lexer from the corresponding Lex file
$(fname): $(fname).l
	$(LEX) $(fname).l
	$(CC) lex.yy.c -o lexer $(CFLAGS)

# Clean up generated files
clean:
	rm -f lexer lex.yy.c

