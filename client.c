#include "client.h"

static void sighandler( int signo ) {
  //Takes the signal number
  //Kills itself after sending a last message to the server
    if ( signo == SIGINT ) {
      if (id!= -1){
	char parameter[1000] = "[exit]";
	write(id,parameter,sizeof(parameter));}
      exit(0);
        
    }
}


void print_cards(){
  //Basic print function to show the user their hand
  int x;
  printf("Your hand:\n");
  for(x = 0; x< numb_cards; x++)
    printf("Card %d: %s\n", x+1,white_cards[x]);
}
void get_cards(char * deck){
  //Importing the full 7 card deck
  char * card;
  strsep(&deck, "\n");
  numb_cards = 0;
  while (deck){
    card = strsep(&deck, "\n");
    strcpy(white_cards[numb_cards], card);
    numb_cards ++;
  }
  //white_cards[numb_cards] = 0;
  numb_cards--;
  //print_cards();
  
}

void pick_white_card(char * response){
  //The user picks their white card of choice
  print_cards();
  response = response + 4;
  printf("** %s \n",response);
  int result = scanf("%d", &chosen);
  char parameter[1000];
  strcpy(parameter,white_cards[chosen - 1]);
  write(id,parameter,sizeof(parameter));
  printf("Message Sent: %s\n",parameter);
}
void pick_winner(){
  //The judge here picks their favorite white card
  int myInt;
  int result = scanf("%d", &myInt);
  write(id,&myInt,sizeof(myInt));
  printf("Message Sent: %d\n",myInt);
}
void replace_card(char * response){
  //Replacing the "used" card with a new, random one
  strcpy(white_cards[chosen -1],strsep(&response, "\n"));
}

int main(int arg, char ** arr) {
  id = -1;
  signal( SIGINT, sighandler );
  char buff[256];
  // Creates The Socket
  id = socket(AF_INET,SOCK_STREAM,0);
  //Port/Connecting
  struct sockaddr_in s;
  s.sin_family = AF_INET;
  s.sin_port = htons(24601);
  if (arg == 1) {
    inet_aton("127.0.0.1",&(s.sin_addr));
  } else {
    inet_aton(arr[1],&(s.sin_addr));
  }
  // Bind
  bind(id,(struct sockaddr *)&s,sizeof(s));
  int c;
  c = connect(id,(struct sockaddr *)&s,sizeof(s));



  while (1) {
    // Connected, sets parameter and gets it
    // (Used to test) printf("Client Connect %d: ",c);
    char response_buffer[1000];
    if (0 != read(id,response_buffer,sizeof(response_buffer))){
      //read(id,response,sizeof(response));
      char * response = response_buffer;
      if (strncmp(response, "[exit] ", 6) == 0){
	response = response + 6;
	printf("** %s \n",response);
	exit(0);}
      if (strncmp(response, "[yd] ", 4) == 0){//Checking if recieving Your Deck (complete deck)
	//printf("Your Deck: \n");
	get_cards(response);
      } 
      else if (strncmp(response, "[nc] ", 4) == 0){//Checking if recieving New Card
	//printf("Your Deck: \n");
	printf("Recieved New card:\n");
	response = response + 4;
	replace_card(response);
      } 
      else if(strncmp("[rn]",response,4) == 0){  //Checking if Reply Needed
	response = response + 4;
	if(strncmp("[pc]",response,4) == 0){//Checking if it's Pick Card time
	  pick_white_card(response);
	}
	else if(strncmp("[pw]",response,4) == 0){//Checking if it's time to Pick Winner
	  printf("** %s \n",response+4);
	  pick_winner();
	}
	else{
	  printf("** %s \n",response);
	  printf("[Client]: ");
	  char parameter[1000];
	  fgets(parameter,sizeof(parameter),stdin);
	  parameter[strlen(parameter)-1] = '\0';
	  write(id,parameter,sizeof(parameter));

	  printf("Message Sent: %s\n",parameter);
	}
      }
      else //if it doesn't need a response, just print out text
	printf("** %s \n",response);
    }

  }
  return 0;
}
