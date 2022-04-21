#include "./instruments.h"
#include <string.h>

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

char * violins[2] = {
  "violins/violin_1.wav",
  "violins/violin_2.wav"
};


char * assign_partition (char * type) {
  if (!strcmp(type, music_instruments[0])) {
    return violins[1];
  }
  return "";
}