# Makefile for HW1 COMP 111
# Matthew Carrington-fair

############## Variables ###############

CC = gcc # The compiler being used

# Compile flags

CFLAGS = -g -std=gnu99 -Wall -Wextra -Werror -Wfatal-errors -pedantic

############### Rules ###############

all: timing

## Compile step (.c files -> .o files)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

## Linking step (.o -> executable program)

timing: timing.o
	$(CC)  -o timing timing.c $(CFLAGS) 

clean:
	rm -f *.o core* *~timing
