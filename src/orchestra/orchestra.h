#include "../musician/musician.h"
#include <stdbool.h>

#ifndef ORCHESTRA_H_
#define ORCHESTRA_H_

Musician musicians[N_INSTRU];
long int nthr;
pthread_t thr[N_INSTRU];
int current_nb_instru;



Musician * get_musician(int sockfd);
char * get_partition (int sockfd);
void create_musician (char * type, int sockfd, long int nthread);
void exit_orchestra ();
void listen_for_connections();
#endif