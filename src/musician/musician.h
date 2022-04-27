#include <AL/al.h>

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

#endif