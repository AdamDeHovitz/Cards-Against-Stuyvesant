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

int total_clients;
typedef struct n  {
	char name[256];
	char whitecard[256];
	int score;
} client_info;
client_info * clients[10];

int main() {
  // Variables 
  total_clients = 0;
  int id,client,semd; 
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

  // The main thing
  while (total_clients < 4) {
    client = accept(id, NULL, NULL); 
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
    else{
      if (total_clients == 4){
	wait(&count);
      }
    }
  }
  int sval;
  while (sval != 0){
    sval = semctl(semd, 0, GETVAL);
  }
  int x;
  for(x = 0;x<5;x++){
    printf("client %d: name = %s",x,(p + x)->name);
  }
  return 0;





}
