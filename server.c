#include "deck.h"
int number_users; //Max number of users
int judge; //judge value
typedef struct n  {
  int descriptor; //Used for socket connection
  char name[1000];//Their name
  int score;      //The all important score

} client_info;
client_info * clients[10]; //client array (currently limited to 10)


static void sighandler( int signo ) {
  //Take keyboard interrupt and ruins terminate
  //Kills itself after sending a last message to the server
    if ( signo == SIGINT ) {
      terminate();
    }
}

void terminate(){
  //Caused by keyboard interrupt to clients or server
  //Sends last message and KILLS ALL CLIENTS
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
  //Runs the main round

  //Let's pick a judge
  judge = (judge + 1) % number_users;
  int x;
  
  //Everyone should know who the judge now is
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

  //Sent black card to each player
  for(x = 0;x<number_users;x++){
    write(clients[x]->descriptor,to_client,sizeof(to_client));
  }

  //Get the card choices from each player
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
      strcat(responses,response);
      n++;
    }
  }
  
  //Each player should know out of what the judge is picking
  send_to_players(responses);
  
  //The judge now picks
  char to_judge[1000] = "[rn][pw]Pick the winner\n";
  strcat(to_judge,responses);
  write(clients[judge]->descriptor,to_judge,sizeof(to_judge));
  int winner;
  read(clients[judge]->descriptor,&winner,sizeof(winner));
  char result[1000];
  
  //Handling the weird number business caused by the judge
  if (n <= judge){
    sprintf(result,"%s won\n",clients[winner-1]->name);
    clients[winner-1]->score++;}
  else{
    sprintf(result,"%s won\n",clients[winner]->name);
    clients[winner]->score++;
  }
  
  //Send the players the result, followed by a new card to replace what they spent
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
  int total_clients = 0;
  judge = 0;
  int id,client,semd;
  char buff[256];
  printf("How many users? ");
  int result = scanf("%d", &number_users);
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
      strcpy(current->name, from_client);
      clients[total_clients] = current;
      total_clients++;}

    //}
  }
  printf("Exited loop\n");
  //Testing to see if all users are accounted for
  int x;
  printf("Clients in game:\n");
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
    write(clients[x]->descriptor,to_client,sizeof(to_client));

  }

  //Running the game
  while (1){//Goes until someone keyboard interrupts
    manage_round();
  }



  
  return 0;





}
