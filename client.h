#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
char  white_cards[10][1000];
int numb_cards;
int id;
int chosen;

void print_cards();
void get_cards(char * deck);
void pick_white_card(char * response);
void pick_winner();
void replace_card(char * response);
