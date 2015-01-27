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
int total_clients;
typedef struct n  {
  int descriptor;
  char name[1000];
  char whitecard[1000];
  int score;
  
} client_info;
client_info * clients[10];


void play_round(){
  //Should pick a judge
  //Draw a black card
  //Sent black card to each player
  //get responses and store them in the client_info for each client in array clients
  //Judge picks winner who gets score
  //Could pretty easily implement some kind of system for choosing each judge
 
}


int main() {
  // Variables 
  total_clients = 0;
  int id,client,semd; 
  char buff[256]; 
  number_users = 2;
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
  /*
  int shmkey = ftok("makefile", 'z');
  int sd = shmget(shmkey, sizeof(clients), 0664 | IPC_CREAT );
  client_info *p;
  p = (client_info *) shmat(sd, 0, 0);
  p = clients[0];
  
  int semkey = ftok("makefile", 'a');
  semd = semget( semkey, 1, 0664);
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = 4;
  semop(semd, &sb, 1);
  */


  //fcntl(id, F_SETFL, O_NONBLOCK);

  //Having people join in
  while (total_clients < number_users) {
    
    if( (client = accept(id, NULL, NULL) ) != -1) {
      /*int boolean = 1;
      int x;

      //Attempt to fix strange client looping with first client
      if (total_clients){
	for(x = 0;x<total_clients;x++){
	  printf("id: %d\n",client);
	  if (clients[x]->descriptor == client){
	    boolean = 0;
	  }
	}
      }
      printf("b: %d\n",boolean);
      if (boolean){*/
	
	char to_client[1000] = "Welcome! Please give your name"; 
	write(client,to_client,sizeof(to_client));
	client_info *current = (client_info *)malloc(sizeof(client_info));
	current->descriptor = client;
	char from_client[1000] = "";
	read(client,from_client,sizeof(from_client));
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
    char to_client[1000] = "Your Deck:\n";
    for(counter = 0; counter < 7;counter++){
      strcat(to_client,draw("white"));
    }
    printf("%s\n",to_client);
    write(clients[x]->descriptor,to_client,sizeof(to_client));

  }

  //Now we need to run the game
  /*
  while (1){//temporarily permanent, should at some point
    play_round();
    }
  */


  

  /* 
     printf("Connected.\n");
     total_clients++;
     int count = fork(); 
     if (!count) {
     close(id);
     while (1) {
     char from_client[256] = ""; 
     read(client,from_client,sizeof(from_client));
	// Checks if Exits (If so...)
	if (from_client[0] == 'e' && from_client[1] == 'x' && from_client[2] == 'i' && from_client[3] == 't') {
	  read(client,from_client,sizeof(from_client));
	  printf("Client %d has disconnected.\n",client);
	  return 0; 
	}
      
	// Prints What It Receives if it was NOT exit
	p = (client_info *) shmat(sd, 0, 0);
	client_info *current = (client_info *)malloc(sizeof(client_info));
	strcpy(current->name, from_client);
      
	*(p + total_clients) = *current;
	sb.sem_op = -1;
	semop(semd, &sb, 1);
	printf("Just Received This: %s\n",from_client);

	char to_client[256] = "Recieved message"; 
	write(client,to_client,sizeof(to_client));
      }
      close(client);
      printf("Server: Offline");
      exit(0);
    }
    
    }
  }
  int sval;
  while (sval != 0){
    sval = semctl(semd, 0, GETVAL);
  }

  */
  return 0;





}
