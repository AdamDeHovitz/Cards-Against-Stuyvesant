all: server.c client.c deck.o
	gcc -o server server.c deck.o
	gcc -o client client.c 
deck.o: deck.c deck.h
	gcc -c deck.c
