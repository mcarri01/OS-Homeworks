# Makefile for HW3 COMP 111
# Matthew Carrington-fair

############## Variables ###############

CC = gcc # The compiler being used

# Compile flags

CFLAGS = -g -std=gnu99 -Wall -Wextra -Werror -Wfatal-errors -pedantic -lpthread

############### Rules ###############

all: wc

## Compile step (.c files -> .o files)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

## Linking step (.o -> executable program)

wc: wc.o
	$(CC)  -o wc wc.c $(CFLAGS) 

clean:
	rm -f *.o core* *~wc