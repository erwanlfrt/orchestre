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
  "tambourine",
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
  if (!strcmp(type, music_instruments[0])) {
    return violins[3];
  }
  if (!strcmp(type, music_instruments[18])) {
    return pianos[0];
  }
  return "";
}