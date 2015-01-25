#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> 
#include <string.h> 

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
    printf("Client: ");
    char parameter[100];
    fgets(parameter,sizeof(parameter),stdin);
    parameter[strlen(parameter)-1] = '\0';
    write(id,parameter,strlen(parameter)); 
    // Determines if you put in 'exit', in which case you can disconnect
    if (parameter[0] == 'e' && parameter[1] == 'x' && parameter[2] == 'i' && parameter[3] == 't') {
      char stringy[10];
      sprintf(stringy,"%d",id);
      write(id,stringy,strlen(stringy));
      return 0; 
    }
    // Otherwise, if not exit, this prints out what you just sent
    printf("Message Sent: %s\n",parameter);
    char response[100] = ""; 
    read(id,response,sizeof(response));
    printf("Message Recieved: %s\n",response);
  }
  return 0; 
}
