#include "../musician/musician.h"
#include <stdbool.h>

#ifndef ORCHESTRA_H_
#define ORCHESTRA_H_

Musician musicians[N_INSTRU];
static long int nthr = 0;
pthread_t thr[N_INSTRU];
static int current_nb_instru = 0;



Musician * get_musician(int sockfd);
char * get_partition (int sockfd);
void create_musician (char * type, int sockfd, long int nthread);
bool is_valid_cmd (char* line);
void exit_orchestra ();
#endif