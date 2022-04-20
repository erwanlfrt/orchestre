#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "./tools/colors.h"
#define NUMBER_OF_INSTRUMENTS 18
#define NUMBER_OF_CMD 2

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

char * available_cmd[2] = {
  "play",
  "pause"
};

void display_help () {
  int i;
  printf("available instruments : \n");
  for (i = 0; i < NUMBER_OF_INSTRUMENTS; i++) {
    printf("%s\n", music_instruments[i]);
  }
}

bool is_valid_instrument (char * instrument) {
  int i;
  if (strcmp("", instrument) && strcmp("help", instrument) && strcmp("h", instrument) ) {
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

void is_valid_cmd (char* cmd) {
  int i;
  if (strcmp("", cmd)) {
    for (i = 0; i < NUMBER_OF_CMD; i++) {
      if (strcmp(available_cmd[i], cmd) == 0) {
        return;
      }
    }
    red();
    printf("\n%s is not a valid command.\n", cmd);
    white();
  }
  return;
}

void get_instrument () {
  char instrument[40];
  strncpy(instrument, "", sizeof(instrument));
  while (!is_valid_instrument(instrument)) {
    printf("\nWhat is your instrument ? ");
    scanf("%s", instrument);
    if(!strcmp("help", instrument) || !strcmp("h", instrument)) {
      display_help();
    }
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

void listen_for_key_events() {
  char cmd[40];
  printf("Type \"play\" or \"pause\"\n");
  while(true) {
    scanf(" %s", cmd);
    is_valid_cmd(cmd);
  }
}




int main(int argc, char* argv[]) {
  display_welcome_screen();
  get_instrument();
  listen_for_key_events();
  return 0;
}
