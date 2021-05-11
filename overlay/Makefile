# Makefile for the overlay 
#
# CS 60, Spring 2021

PROGS = overlay
LIBS = -pthread

FLAGS = #-DTEST
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: $(PROGS)

overlay: overlay.o neighbortable.o topology.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

##### source dependencies
overlay.o: overlay.h neighbortable.h topology.h 

neighbortable.o: neighbortable.h topology.h 

topology.o: topology.h 


.PHONY: valgrind clean

########### valgrind ##################
valgrind: overlay
	$(VALGRIND) ./overlay

############## clean  ##########
clean:
	rm -rf *~ *.o *.dSYM .DS_Store
	rm -rf $(PROGS)

