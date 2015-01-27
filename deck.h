#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/shm.h>  //Some of these are relics from when shared memory was being attempted
#include <sys/ipc.h>
#include <sys/sem.h>



//Represents a given "deck"
char * white_cards[1000];
char * black_cards[1000];

//Permanent storage of all cards
char white_cards_buffer[1000][1000];  
char black_cards_buffer[1000][1000];

int numb_white_total;//refer to the buff
int numb_black_total;
int numb_white;//refer to the deck
int numb_black;


void read_in_files(char * type);
void read_white();
void read_black();
char * draw(char * type);
//void randomize ( char arr[], int n );
void shuffle(char * type);
void terminate();
void send_to_players(char message[1000]);
void manage_round();

