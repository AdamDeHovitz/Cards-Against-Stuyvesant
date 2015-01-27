#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <time.h>

//Represent a given "deck"
char * white_cards[1000];
char * black_cards[1000];

//Permanent storage of all cards
char white_cards_buffer[1000][1000];  
char black_cards_buffer[1000][1000];

int numb_white_total;
int numb_black_total;
int numb_white;
int numb_black;


void read_in_files(char * type);
void read_white();
void read_black();
char * draw(char * type);
//void randomize ( char arr[], int n );
void shuffle(char * type);
void terminate();

