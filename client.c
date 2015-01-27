#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> 
#include <string.h> 
char * white_cards[10];
int numb_cards;

void get_cards(char * deck){
  char * card;
  strsep(&deck, "\n");
  numb_cards = 0;
  while (deck){
    card = strsep(&deck, "\n");
    white_cards[numb_cards] = card;
    numb_cards ++;
  }
  white_cards[numb_cards] = 0;
  numb_cards--;
  int x;
  for(x = 0; x< numb_cards; x++)
    printf("Card %d: %s", x+1,white_cards[x]);
}




int main(int arg, char ** arr) {
  int id; 
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
    char response[1000]; 
    printf("id: %zd\n",read(id,response,sizeof(response)));
    //read(id,response,sizeof(response));
    if (strncmp(response, "Your Deck:", 10) == 0){
      printf("Recieved cards\n");
      get_cards(response);
      read(id,response,sizeof(response));
    }
    printf("Message Recieved: %s\n",response);
    printf("Client: ");
    char parameter[1000];
    fgets(parameter,sizeof(parameter),stdin);
    parameter[strlen(parameter)-1] = '\0';
    write(id,parameter,strlen(parameter)); 
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
  return 0; 
}
