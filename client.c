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

void print_cards(){
  int x;
  printf("Your hand:\n");
  for(x = 0; x< numb_cards; x++)
    printf("Card %d: %s\n", x+1,white_cards[x]);
}
void get_cards(char * deck){
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
  
  int myInt;
  int result = scanf("%d", &myInt);
  write(id,&myInt,sizeof(myInt));
  printf("Message Sent: %d\n",myInt);
}
void replace_card(char * response){
  //printf("%s\n",response);
  strcpy(white_cards[chosen -1],strsep(&response, "\n"));
}

int main(int arg, char ** arr) {
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
	  // Determines if you put in 'exit', in which case you can disconnect
	  if (parameter[0] == 'e' && parameter[1] == 'x' && parameter[2] == 'i' && parameter[3] == 't') {
	    char stringy[10];
	    sprintf(stringy,"%d",id);
	    write(id,stringy,strlen(stringy));
	    printf("Exiting\n");
	    return 0;
	  }
	  // Otherwise, if not exit, this prints out what you just sent
	  printf("Message Sent: %s\n",parameter);
	}
      }
      else //if it doesn't need a response, just print out text
	printf("** %s \n",response);
    }

  }
  return 0;
}
