#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "./tools/colors.h"
#define NUMBER_OF_INSTRUMENTS 18

char * music_instruments[NUMBER_OF_INSTRUMENTS] = { 
  "violin", 
  "viola", 
  "cello",
  "double bass",
  "flute",
  "oboe",
  "clarinet",
  "bassoon",
  "horn",
  "trumpet",
  "trombone",
  "tuba",
  "timpani",
  "snare drum",
  "bass drum",
  "cymbals",
  "triangle",
  "tambourine"
};

bool is_valid_instrument (char * instrument) {
  int i;
  if (strcmp("", instrument) != 0) {
    for (i = 0; i < NUMBER_OF_INSTRUMENTS; i++) {
      if (strcmp(music_instruments[i], instrument) == 0) {
        return true;
      }
    }
    red();
    printf("%s is not a valid instrument.\n", instrument);
    white();
  }
  
  return false;
}

void get_instrument () {
  char instrument[40];
  strncpy(instrument, "", sizeof(instrument));
  while (!is_valid_instrument(instrument)) {
    printf("\nWhat is your instrument ? ");
    scanf("%s", instrument);
  }
}


void display_welcome_screen () {
  yellow();
  printf("======================================================\n");
  printf("=                    MUSICIAN                        =\n");
  printf("======================================================\n");
  green();
  printf("To see the available instrument, type \"help\" or \"h\"\n");
  white();
}


int main(int argc, char* argv[]) {
  display_welcome_screen();
  get_instrument();
  return 0;
}
