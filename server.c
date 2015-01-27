#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include "deck.h"


int number_users;
int judge;
int total_clients;
typedef struct n  {
  int descriptor;
  char name[1000];
  //char whitecard[1000];
  int score = 0;

} client_info;
client_info * clients[10];


static void sighandler( int signo ) {
  //Takes the signal number
  //Kills itself after sending a last message to the server
    if ( signo == SIGINT ) {
      terminate();
    }
}

void terminate(){
  char death[1000] = "[exit]A client has disconnected\nFinal scores:\n";
  int x;
  for(x = 0;x<number_users;x++){
    char score[1000];
    sprintf(score,"%d. %s: %d points\n",x+1,clients[x]->name,clients[x]->score);
    strcat(death,score);
  }
  for(x = 0;x<number_users;x++){
    write(clients[x]->descriptor,death,sizeof(death));
  }
  exit(0);
}


void send_to_players(char message[1000]){
  //Sends message to all non-judge clients
  int x;
  for(x = 0;x<number_users;x++){
    if (x != judge)
      write(clients[x]->descriptor,message,1000);
  }
}


void manage_round(){
  //Should pick a judge
  judge = (judge + 1) % total_clients;
  printf("%s is the judge.\n",clients[judge]-> name);
  int x;
  
  char message1[1000] = "You are the judge ";
  write(clients[judge]->descriptor,message1,sizeof(message1));
  
  char message2[1000] = "Judge is ";
  strcat(message2,clients[judge]->name);
  strcat(message2, " \n");
  send_to_players(message2);

  //Draw a black card
  read_in_files("black");
  char to_client[1000] = "Current Black Card: ";
  strcat(to_client, draw("black"));
  printf("%s\n",to_client);

  //Sent black card to each player
  for(x = 0;x<number_users;x++){
    write(clients[x]->descriptor,to_client,sizeof(to_client));
  }

  //get responses and store them in the client_info for each client in array clients
  char to_client2[1000] = "[rn][pc]Pick the number of your chosen card: ";
  send_to_players(to_client2);
  char responses[1000] = "Responses are:";
  int n = 1;
  for(x = 0;x<number_users;x++){
    if (x != judge){
      char card[1000];
      read(clients[x]->descriptor,card,sizeof(card));
      if (strncmp(card, "[exit] ", 6) == 0)
	terminate();
      char response[1000];
      sprintf(response,"\n%d. from %s: ", n, clients[x]->name);
      strcat(response,card);
      printf("%s\n",response);
      strcat(responses,response);
      n++;
    }
  }
  
  send_to_players(responses);
  
  char to_judge[1000] = "[rn][pw]Pick the winner\n";
  strcat(to_judge,responses);
  write(clients[judge]->descriptor,to_judge,sizeof(to_judge));
  int winner;
  read(clients[judge]->descriptor,&winner,sizeof(winner));
  char result[1000];
  if (n <= judge){
    sprintf(result,"%s won\n",clients[winner-1]->name);
    clients[winner-1]->score++;}
  else{
    sprintf(result,"%s won\n",clients[winner]->name);
    clients[winner]->score++;
  }
  
  printf("Peparing to send this result: %s\n",result);
  send_to_players(result);
  for(x = 0;x<number_users;x++){
    if (x != judge){
      char new_card[1000] ="[nc]";
      strcat(new_card,draw("white"));
      printf("Sending this new card: %s\n",new_card);
      write(clients[x]->descriptor,new_card,sizeof(new_card));
    }
  }
  


}


int main() {
  signal( SIGINT, sighandler );
  // Variables
  total_clients = 0;
  judge = 0;
  int id,client,semd;
  char buff[256];
  number_users = 3;//Eventually should ask the server to input
  // Socket Creation
  id = socket(AF_INET,SOCK_STREAM,0);
  // Port/Connect
  struct sockaddr_in reader;
  reader.sin_family = AF_INET;
  reader.sin_port = htons(24601);
  reader.sin_addr.s_addr = INADDR_ANY;
  // Listens, prints online after
  bind(id, (struct sockaddr *)&reader, sizeof(reader));
  listen(id,1);
  printf("Server: Online\n");

  //Having people join in
  while (total_clients < number_users) {

    if( (client = accept(id, NULL, NULL) ) != -1) {

      char to_client[1000] = "[rn]Welcome! Please give your name";
      write(client,to_client,sizeof(to_client));
      client_info *current = (client_info *)malloc(sizeof(client_info));
      current->descriptor = client;
      char from_client[1000] = "";
      read(client,from_client,sizeof(from_client));
      if (strncmp(from_client, "[exit] ", 6) == 0)
	terminate();
      printf("Just Received This: %s from %d\n",from_client,client);
      strcpy(current->name, from_client);
      clients[total_clients] = current;
      total_clients++;}

    //}
  }
  printf("Exited loop\n");
  //Testing to see if all users are accounted for
  int x;
  for(x = 0;x<number_users;x++){
    printf("client %d: name = %s Id = %d\n",x,clients[x]->name, clients[x]->descriptor);
  }

  //Setting up the game
  read_in_files("both");
  for(x = 0;x<number_users;x++){
    int counter;
    char to_client[1000] = "[yd]\n";
    for(counter = 0; counter < 7;counter++){
      strcat(to_client,draw("white"));
    }
    //printf("%s\n",to_client);
    write(clients[x]->descriptor,to_client,sizeof(to_client));

  }

  //Running the game
  while (1){//temporarily permanent, should at some point
    manage_round();
  }



  \
  return 0;





}
