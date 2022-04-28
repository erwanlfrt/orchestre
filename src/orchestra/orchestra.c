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
    musicians[current_nb_instru].partition =  assign_partition(type);
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
  assert(recv (sock, buffer, sizeof buffer, 0) != -1);
  create_musician(buffer, sock, nthread);

  load(sock);

  int count = 0;
  int total = 0;
  while ((count = recv(sock, &buffer[total], sizeof buffer - total, 0)) > 0) {
    if (!strcmp(buffer, "play")) {
      play(sock);
    } else if (!strcmp(buffer, "pause")) {
      pause_sound(sock);
    } else if (!strcmp(buffer, "stop")) {
      stop(sock);
    } else {
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
  while(nthr >= N_INSTRU);
  listen_for_connections();  
}

/**
 * @brief listen for new connections
 * 
 */
void listen_for_connections () {
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
  long int prec_nthr = 0;
  while(nthr < N_INSTRU) {
    if (nthr != prec_nthr) {
      prec_nthr = nthr;
    }
    int new_socket;
    new_socket = accept(sockfd, (struct sockaddr *)&addr, (socklen_t*)&addrlen);
    if (new_socket >= 0) {
      args[0] = (void*)(long int)new_socket;
      args[1] = (void*)nthr;
      pthread_create(&thr[nthr], NULL, &thread_musician, args);
      nthr++;
    }
  }
  shutdown(sockfd, 2);
  close(sockfd);
  continue_playing_orchestra (sockfd, addr, addrlen);
}





/**
 * @brief Check if line is a valid command line for RUN section
 * 
 * @param line line to check
 * @return true if line is a valid command line, false if not
 */
bool is_valid_cmd (char* line) {
  char * cmd = strsep(&line, " ");
  if (strcmp(cmd, "play") && strcmp(cmd, "stop") && strcmp(cmd, "wait") && strcmp(cmd, "direction")) {
    return false;
  } else if (!strcmp(cmd, "play") || !strcmp(cmd, "stop") || !strcmp(cmd, "wait")) {
    char* time_or_id_string = strsep(&line, " ");
    if (!is_int(time_or_id_string)) return false;
    if (!strcmp(cmd, "stop") || !strcmp(cmd, "play")) {
      int id = atoi(time_or_id_string);
      if(get_musician(id) == NULL) {
        printf("Unknown musician with id = %i\n", id);
        return false;
      }
    }
    return true;
  } else if (!strcmp(cmd, "direction")) {
    char* string_id = strsep(&line, " ");
    if (!is_int(string_id)) return false;
    int id = atoi(string_id);
    if(get_musician(id) == NULL) return false;
    char * direction = strsep(&line, " ");
    if (get_direction(direction) == -2) return false;
    return true;
  }
  return false;
}


/**
 * @brief Exit orchestra
 * 
 */
void exit_orchestra () {
  for (int i = 0; i < current_nb_instru; i++) {
    Musician musician = musicians[i];
    stop(musician.sockfd);
  }
}

/**
 * @brief Init connection and listen for new connections.
 * 
 */
void thread_orchestra () {
  uint16_t port = 1234;
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  int addrlen = sizeof(addr); 
  printf("Waiting for musicians\n");
  listen_for_connections();
}

/**
 * @brief main function
 * 
 * @param argc number of arguments
 * @param argv arguments
 * @return int EXIT_SUCCESS
 */
int main (int argc, char **argv) {
  init_openAL();
  display_orchestra_header();
  if (argc >= 2) {
    read_file(argv[1]); // if file path is given then read a script.
  } else {
    thread_orchestra(); // else, listen for incoming musicians.
  }
  close_openAL();
  return EXIT_SUCCESS;
}