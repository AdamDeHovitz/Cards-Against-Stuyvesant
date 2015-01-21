#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

char white_cards[1000][1000];
char black_cards[1000][1000];


void read_in_files(){
  printf("\nRead in the file\n");
  char line[1000];

  FILE *ptr_file;
  ptr_file =fopen("whitecards.txt","r");
  int count = 0;
  if (ptr_file)
    while (fgets(line,1000, ptr_file)!=NULL){
      strcpy(white_cards[count], line);
      printf("line: %s\n",line);
      count++;
    }

  fclose(ptr_file);


  /*

  int fd = open("whitecards.txt", O_RDONLY);
  int d = 0;


  char * file_content;
  char * line;
  printf("preparing to read");
  int num = read( fd, &file_content, 2048);
  printf("read in file");
  int count = 0;
  while (file_content){
    line = strsep(&file_content, "\n");
    printf("line: %s\n",line);
    white_cards[count] = line;
    count++;

  }
  close(fd); 

  */

}

int main(int argc, char *argv[]){

  read_in_files();


  return 0;
}





