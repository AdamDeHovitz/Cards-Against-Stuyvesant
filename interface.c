#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

char white_cards[256][256]
char black_cards[256][256]


void read_in_files(){
  printf("\nRead in the file\n");

  int fd = open("white", O_RDONLY);
  int d = 0;
  int num = 0;
  while(num > 0){
    num = read( fd, &white_cards[d], sizeof(doctor));
    /* printf("%d\n",num);
       printf("%s\n",strerror(errno));*/
    printf("%s\n",white_cards[d]);
  }
  close(fd); 
  fd = open("black", O_RDONLY);
  d = 0;
  num = 0;
  while(num > 0){
    num = read( fd, &black_cards[d], sizeof(doctor));
    /* printf("%d\n",num);
       printf("%s\n",strerror(errno));*/
    printf("%s\n",black_cards[d]);
  }
  close(fd); 
}
