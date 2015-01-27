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
  numb_white_total = 0;

  if (ptr_file)
    while (fgets(line,1000, ptr_file)!=NULL){
      strcpy(white_cards_buffer[numb_white_total], line);
      //printf("line: %s\n",line);
      numb_white_total++;
    }

  fclose(ptr_file);

  int x;
  numb_white = numb_white_total;
  for( x = 0; x< numb_white_total;x++){
    white_cards[x]=white_cards_buffer[x];
  }

  //shuffle("white");
}

void read_black(){
  char line[1000];
  FILE *ptr_file;
  ptr_file =fopen("blackcards.txt","r");
  numb_black_total = 0;

  if (ptr_file)
    while (fgets(line,1000, ptr_file)!=NULL){
      strcpy(black_cards_buffer[numb_black_total], line);
      //printf("line: %s\n",line);
      numb_black_total++;
    }

  fclose(ptr_file);
  numb_black = numb_black_total;
  int x;
  for( x = 0; x< numb_black;x++){
    black_cards[x]=black_cards_buffer[x];
  }
  shuffle("black");
}

char * draw(char * type){
  if (!strcmp("black",type)){
    if (!numb_black){

      ///Reset Deck
      int x;
      for( x = 0; x< numb_black_total;x++){
	black_cards[x]=black_cards_buffer[x];
      }
      numb_black = numb_black_total;
      ///


    }
    numb_black--;
    return black_cards[numb_black];
  }
  else{
    if (!numb_white){

      ///Reset Deck
      int x;
      for( x = 0; x< numb_white_total;x++){
	white_cards[x]=white_cards_buffer[x];
      }
      numb_white = numb_white_total;
      //end
    }
    numb_white--;
    return white_cards[numb_white];
  }
}



void shuffle ( char* type ){
  if (!strcmp("black",type)){
    srand ( time(NULL) );
    int n = numb_black;
    for (int x = n-1; x > 0; x--)
      {
	int r = rand() % (x+1);
	char * storage;
	storage = black_cards[x];
	black_cards[x] = black_cards[r];
	black_cards[r] = storage;
      }
  }

  //Doing it for white cards
  else{
    srand ( time(NULL) );
    int n = numb_white;
    for (int x = n-1; x > 0; x--)
      {
	int r = rand() % (x+1);
	char * storage;
	storage = white_cards[x];
	white_cards[x] = white_cards[r];
	white_cards[r] = storage;
      }

  }
}


/*
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
  }*/
