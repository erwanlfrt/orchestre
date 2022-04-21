typedef struct musician {
  char * type;
  int sockfd;
  char * partition;
} Musician;

char * assign_partition (char * type);