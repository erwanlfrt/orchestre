#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "./instruments.h"

extern int sockfd;
char * music_instruments[NUMBER_OF_INSTRUMENTS] = { 
  "violin", 
  // "viola", 
  // "cello",
  // "double bass",
  // "flute",
  // "oboe",
  // "clarinet",
  // "bassoon",
  // "horn",
  // "trumpet",
  // "trombone",
  // "tuba",
  // "timpani",
  // "snare drum",
  // "bass drum",
  // "cymbals",
  // "triangle",
  // "tambourine",
  "piano"
};

char * violins[4] = {
  "violins/violin_1.wav",
  "violins/violin_2.wav",
  "violins/violin_3.wav",
  "violins/violin_4.wav"
};

char * pianos[1] = {
  "piano/piano_1.wav"
};

char * assign_partition (char * type) {
  // static attribution of partition.
  // idea : get a random partition or ask for a specific partition
  if (!strcmp(type, music_instruments[0])) {
    return violins[3];
  }
  if (!strcmp(type, music_instruments[1])) {
    return pianos[0];
  }
  return "";
}

/**
 * @brief check if instrument is a valid instrument
 * 
 * @param instrument instrument to check 
 * @return true if instrument is a valid one, false if not
 */
bool is_valid_instrument (char * instrument) {
  int i;
  if (strcmp("", instrument) && strcmp("help", instrument) && strcmp("h", instrument) ) {
    for (i = 0; i < NUMBER_OF_INSTRUMENTS; i++) {
      if (strcmp(music_instruments[i], instrument) == 0) {
        return true; // return true if the instrument is found in the list.
      }
    }
    printf("%s is not a valid instrument.\n", instrument);
  }
  return false; // return false if the instrument is not found
}