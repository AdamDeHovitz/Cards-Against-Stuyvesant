#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <signal.h> 

typedef struct client {
	char name[256];
	char whitecard[256];
	int score;
} client;

int main() {
  // Variables 
  int id,client; 
  char buff[256]; 
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
  // The main thing
  while (1) {
    client = accept(id, NULL, NULL); 
    printf("Connected.\n");
    int count = fork(); 
    if (!count) {
      close(id);
      while (1) {
	char parameter[100] = ""; 
	read(client,parameter,sizeof(parameter));
	// Checks if Exits (If so...)
	if (parameter[0] == 'e' && parameter[1] == 'x' && parameter[2] == 'i' && parameter[3] == 't') {
	  read(client,parameter,sizeof(parameter));
	  printf("Client %d has disconnected.\n",client);
	  return 0; 
	}
	// Prints What It Receives if it was NOT exit
	printf("Just Received This: %s\n",parameter);
	write(client,parameter,sizeof(parameter));
      }
      close(client);
      printf("Server: Offline");
      exit(0);
    }
  }
  return 0;





}
