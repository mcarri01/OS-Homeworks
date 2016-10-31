# Makefile for HW2 COMP 111
# Matthew Carrington-fair

############## Variables ###############

CC = gcc # The compiler being used

# Compile flags

CFLAGS = -g -std=gnu99 -Wall -Wextra -Werror -Wfatal-errors -pedantic

############### Rules ###############

all: sysproc

## Compile step (.c files -> .o files)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

## Linking step (.o -> executable program)

sysproc: sysproc.o
	$(CC)  -o sysproc sysproc.c $(CFLAGS) 

clean:
	rm -f *.o core* *~sysproc
