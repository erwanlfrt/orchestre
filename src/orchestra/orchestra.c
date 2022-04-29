#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include "../musician/musician.h"
#include "../instruments/instruments.h"
#include "../direction/direction.h"
#include "../audio/audio.h"
#include "./orchestra.h"
#include "../tools/tools.h"
#include "../file_reader/file_reader.h"
#include "../color/colors.h"
#define MAX_LEN 50
#define CREATE_STEP 1
#define RUN_STEP 2
#define EXIT_STEP 3

long int nthr = 0;
int current_nb_instru = 0;
ALuint sources[N_INSTRU];


void display_orchestra_header () {
  printf("=======================================================\n");
  printf("=                      ORCHESTRA                      =\n");
  printf("=======================================================\n");
}


/**
 * @brief Get the musician object
 * 
 * @param sockfd sockfd/id of the musician
 * @return Musician* 
 */
Musician * get_musician(int sockfd) {
  for (int i = 0; i < current_nb_instru; i++) {
    if (sockfd == musicians[i].sockfd) {
      return &(musicians[i]);
    }
  }
  return NULL;
}

/**
 * @brief Get the partition of a musician
 * 
 * @param sockfd sockfd/id of the musician
 * @return char* partition
 */
char * get_partition (int sockfd) {
  int i;
  for (i = 0; i < current_nb_instru ; i++) {
    if (musicians[i].sockfd == sockfd) {
      return musicians[i].partition;
    } 
  }
  return "";
}


/**
 * @brief Create a musician.
 * 
 * @param type type of the musician
 * @param sockfd sockfd/id of the musician
 * @param nthread thread number of the musician
 */
void create_musician (char * type, int sockfd, long int nthread) {
  if (current_nb_instru < N_INSTRU) {
    musicians[current_nb_instru].type = type;
    musicians[current_nb_instru].sockfd = sockfd;
    musicians[current_nb_instru].partition =  assign_partition(type); //assign a partition according to his type
    musicians[current_nb_instru].nthr = nthread;
    printf("New %s playing partition %s\n", musicians[current_nb_instru].type, musicians[current_nb_instru].partition);
    current_nb_instru++;
  }
}

/**
 * @brief Thread when a new musician is connected.
 * 
 * @param args array containing sockfd and thread number of the new musician
 * @return void* 
 */
void * thread_musician (void * args) {
  int sock = ((int*)args)[0];
  long int nthread = ((long int*)args)[1];
  char buffer[8192];
  int count = 0;
  int total = 0;
  assert(recv (sock, buffer, sizeof buffer, 0) != -1); // receive instrument from musician
  create_musician(buffer, sock, nthread); // create a musician with the given instrument
  load(sock); // load the new musician partition
  
  while ((count = recv(sock, &buffer[total], sizeof buffer - total, 0)) > 0) {
    // listen for commands sent by the musician
    if (!strcmp(buffer, "play")) {
      // play command
      play(sock);
    } else if (!strcmp(buffer, "pause")) {
      // pause command
      pause_sound(sock);
    } else if (!strcmp(buffer, "stop")) {
      // stop command
      stop(sock);
    } else {
      // direction position
      int position = buffer[0] - '0';
      set_position(sock, position);
    }
  }
}

/**
 * @brief Continue playing orchestra. 
 * 
 */
void continue_playing_orchestra () {
  while(nthr >= N_INSTRU); // while no new connection available loop
  listen_for_connections();  // listen for new connection when there are new connections available
}

/**
 * @brief listen for new connections
 * 
 */
void listen_for_connections () {
  // init socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  int opt = 1;
  uint16_t port = 1234;

  assert(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR|SO_BROADCAST, &opt, sizeof(opt))==0);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  int addrlen = sizeof(addr); 
  assert(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr))>=0);
  assert(listen(sockfd, 3)==0);
	void* args[2];
  while(nthr < N_INSTRU) {
    // while connections available, listen for new connections
    int new_socket;
    new_socket = accept(sockfd, (struct sockaddr *)&addr, (socklen_t*)&addrlen); // accept new connections 
    if (new_socket >= 0) {
      args[0] = (void*)(long int)new_socket;
      args[1] = (void*)nthr;
      pthread_create(&thr[nthr], NULL, &thread_musician, args);
      nthr++;
    }
  }
  shutdown(sockfd, 2); //shutdown current socket 
  close(sockfd);
  continue_playing_orchestra (sockfd, addr, addrlen); // continue playing orchestra while no new connection available
}

/**
 * @brief Exit orchestra
 * 
 */
void exit_orchestra () {
  for (int i = 0; i < current_nb_instru; i++) {
    Musician musician = musicians[i];
    stop(musician.sockfd); // stop all musicians
  }
}

/**
 * @brief Init connection and listen for new connections.
 * 
 */
void thread_orchestra () {
  // init socket
  uint16_t port = 1234;
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  int addrlen = sizeof(addr); 
  printf("Waiting for musicians\n");
  listen_for_connections(); // Wait incoming musicians
}

/**
 * @brief main function
 * 
 * @param argc number of arguments
 * @param argv arguments
 * @return int EXIT_SUCCESS
 */
int main (int argc, char **argv) {
  init_openAL(); // init audio environment
  display_orchestra_header();
  if (argc >= 2) {
    read_file(argv[1]); // if file path is given then read a script.
  } else {
    thread_orchestra(); // else, listen for incoming musicians.
  }
  close_openAL();
  return EXIT_SUCCESS;
}