#include <AL/al.h>

typedef struct musician {
  char * type;
  int sockfd;
  char * partition;
  ALuint source;
  ALuint buffer;
} Musician;

char * assign_partition (char * type);