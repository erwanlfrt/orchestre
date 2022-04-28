#include <AL/al.h>
#include "../direction/direction.h"
#include "../display/display.h"

#ifndef MUSICIAN_H_
#define MUSICIAN_H_

typedef struct musician {
  char * type;
  int sockfd;
  char * partition;
  ALuint source;
  ALuint buffer;
  long int nthr;
} Musician;

int current_direction;
int sockfd;

void listen_for_key_events();


#endif