#include "deck.h"

//reads in "white", "black", or both decks if not specified
void read_in_files(char * type){
  if (!strcmp("white",type))
    read_white();
  if (!strcmp("black",type))
    read_black();
  else{
    read_white();
    read_black();
  }

}
void read_white(){
  char line[1000];
  FILE *ptr_file;
  ptr_file =fopen("whitecards.txt","r");
  numb_white = 0;
  
  if (ptr_file)
    while (fgets(line,1000, ptr_file)!=NULL){
      strcpy(white_cards[numb_white], line);
      //printf("line: %s\n",line);
      numb_white++;
    }

  fclose(ptr_file);
  shuffle("white");
}
  
void read_black(){
  char line[1000];
  FILE *ptr_file;
  ptr_file =fopen("blackcards.txt","r");
  numb_black = 0;
  
  if (ptr_file)
    while (fgets(line,1000, ptr_file)!=NULL){
      strcpy(black_cards[numb_black], line);
      //printf("line: %s\n",line);
      numb_black++;
    }

  fclose(ptr_file);
  shuffle("black");
}

char * draw(char * type){
  if (!strcmp("black",type)){
    if (!numb_black)
      read_black();
    numb_black--;
    return black_cards[numb_black];
  }
  else{
    if (!numb_white)
      read_white();
    numb_white--;
    return white_cards[numb_white];
  }
}

void shuffle(char * type){
  if (!strcmp("black",type)){
    randomize(black_cards, numb_black);
  }
  else{
    randomize(white_cards, numb_white);
  }
}

void swap (char a[], char b[]){
    char temp[] = a;
    a = b;
    b = temp;
}
 
void randomize ( char  arr[], int n ){

    srand ( time(NULL) );
    for (int x = n-1; x > 0; x--)
    {
        int r = rand() % (x+1);
        swap(arr[x], arr[r]);
    }
}


int main(int argc, char *argv[]){

  read_in_files("both");
  int x = 0;
  
  while (x< 10){
    printf("%d.\n",x);
    printf("White: %s",draw("white"));
    printf("Black: %s",draw("black"));
    x++;
  }


  return 0;
}





