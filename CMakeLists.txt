CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

all: main

main: main.o graph.o
	$(CC) $(CFLAGS) -o main main.o graph.o

main.o: main.c graph.h
	$(CC) $(CFLAGS) -c main.c

graph.o: graph.c graph.h
	$(CC) $(CFLAGS) -c graph.c

clean:
	rm -f *.o main graph_mermaid.txt