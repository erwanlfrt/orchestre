#include <AL/al.h>

typedef struct musician {
  char * type;
  int sockfd;
  char * partition;
  ALuint source;
  ALuint buffer;
  long int nthr;
} Musician;

char * assign_partition (char * type);