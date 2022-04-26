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
#include "./musician/musician.h"
#include "./instruments/instruments.h"
#include "./direction/direction.h"
#define N_INSTRU 2
#define MAX_LEN 50
#define SOUND_PATH "./sounds/"


ALuint sources[N_INSTRU];
char path[300];
Musician musicians[N_INSTRU];

ALuint source;
ALuint buffer;
ALCdevice *device;
ALCcontext *context;

int current_nb_instru = 0;
pthread_t thr[N_INSTRU];
long int nthr = 0;

Musician * get_musician(int sockfd) {
  for (int i = 0; i < current_nb_instru; i++) {
    if (sockfd == musicians[i].sockfd) {
      return &(musicians[i]);
    }
  }
  return NULL;
}

char * get_partition (int sockfd) {
  int i;
  for (i = 0; i < current_nb_instru ; i++) {
    if (musicians[i].sockfd == sockfd) {
      return musicians[i].partition;
    } 
  }
  return "";
}

void load (int sockfd) {
  ALsizei size, freq;
  ALenum format;
  ALvoid *data;
  ALboolean loop = AL_FALSE;

  Musician * musician = get_musician(sockfd);

  strcpy(path, SOUND_PATH);
  strcat(path, get_partition(sockfd));
  
  alGenBuffers((ALuint)1, &(buffer));
  alutLoadWAVFile(path, &format, &data, &size, &freq, &loop);
  alBufferData(buffer, format, data, size, freq);

  alGenSources(1, &(source));
  alSourcei(source, AL_BUFFER, buffer);

  musician->source = source;
  musician->buffer = buffer;

}

void play (int sockfd) {
  Musician * musician = get_musician(sockfd);
  // printf("play partition : %s\n", musician->partition);
  // printf("play source : %i\n", musician->source);
  // printf("source = %i\n", source);
  alSourcePlay(musician->source);
}

void pause (int sockfd) {
  Musician * musician = get_musician(sockfd);
  alSourcePause(musician->source);
}

void stop (int sockfd) {
  Musician * musician = get_musician(sockfd);
  alSourceStop(musician->source);
  deleteSourceBuffer(musician->source, musician->buffer);
  Musician tmp[N_INSTRU];
  int index = 0;
  for (int i = 0; i < current_nb_instru; i++) {
    if (musicians[i].sockfd != sockfd) {
      tmp[index] = musicians[i];
      index++;
    }
  }

  pthread_t tmp_thr[N_INSTRU];
  int index_j = 0;
  // printf("nthr = %i\n", nthr);
  // printf("nthr to look at = %i\n", musician->nthr);
  for (int j = 0; j < nthr ; j++) {
    // printf("nthr = %i\n", j);
    if (j != musician->nthr) {
      tmp_thr[index_j] = j;
      index_j++;
    } else {
      printf("trouvé on delete %i\n", musician->nthr);
    }
  }
  current_nb_instru--;
  nthr--;
  *musicians = *tmp;
  *thr = *tmp_thr;
}

void setPosition(int sockfd, int position) {
    Musician * musician = get_musician(sockfd);
    switch (position) {
        case N:
            alSource3f(musician->source, AL_POSITION, 0.0, 2.0, 0.0);
            break;
        case S:
            alSource3f(musician->source, AL_POSITION, 0.0, -2.0, 0.0);
            break;
        case E:
            alSource3f(musician->source, AL_POSITION, 2.0, 0.0, 0.0);
            break;
        case W:
            alSource3f(musician->source, AL_POSITION, -2.0, 0.0, 0.0);
            break;
        case NE:
            alSource3f(musician->source, AL_POSITION, 2.0, 2.0, 0.0);
            break;
        case NW:
            alSource3f(musician->source, AL_POSITION, -2.0, 2.0, 0.0);
            break;
        case SE:
            alSource3f(musician->source, AL_POSITION, 2.0, -2.0, 0.0);
            break;
        case SW:
            alSource3f(musician->source, AL_POSITION, -2.0, -2.0, 0.0);
            break;
        case CENTER:
            alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0);
            break;
        default:
            // front of the scene
            alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0);
            break;
    }
}

void init_openAL () {
  device = alcOpenDevice (NULL);
  context = alcCreateContext (device, NULL);
  alcMakeContextCurrent (context);
  alutInitWithoutContext(0, NULL);
  alListener3f(AL_POSITION, 0, 0, 0);
  alListener3f(AL_VELOCITY, 0, 0, 0);
  alutSleep (5);
}

void deleteSourceBuffer (ALuint source, ALuint buffer) {
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
}

void close_openAL () {
  alcDestroyContext (context);
  alcCloseDevice (device);
}

void create_musician (char * type, int sockfd, long int nthread) {
  if (current_nb_instru < N_INSTRU) {
    musicians[current_nb_instru].type = type;
    musicians[current_nb_instru].sockfd = sockfd;
    musicians[current_nb_instru].partition = assign_partition(type);
    musicians[current_nb_instru].nthr = nthread;
    current_nb_instru++;
  }
}

void * thread_musician (void * args) {
  int sock = ((int*)args)[0];
  long int nthread = ((long int*)args)[1];
  printf("New connection !!! sockfd = %i\n",sock);

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
      pause(sock);
    } else if (!strcmp(buffer, "stop")) {
      stop(sock);
    } else {
      int position = buffer[0] - '0';
      setPosition(sock, position);
    }
  }
}

void continue_playing_orchestra (int sockfd, struct sockaddr_in addr, int addrlen) {
  while(nthr >= N_INSTRU);
  listen_for_connections(sockfd, addr, addrlen);  
}

void listen_for_connections (int soczedzdfd, struct sockaddr_in adezddedr, int addrledzdzen) {
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

  bool error_displayed = false;
  while(nthr < N_INSTRU) {
    if (nthr != prec_nthr) {
      printf("nthr = %li\n", nthr);
      prec_nthr = nthr;
    }
    int new_socket;
    new_socket = accept(sockfd, (struct sockaddr *)&addr, (socklen_t*)&addrlen);
    // printf("On create un new thread !\n");
    if (new_socket >= 0) {
      args[0] = (void*)(long int)new_socket;
      args[1] = (void*)nthr;
      pthread_create(&thr[nthr], NULL, &thread_musician, args);
      nthr++;
    }
    // while (nthr >= N_INSTRU) {
    //   shutdown(sockfd, 2);
    //   close(sockfd);
    //   break;
    // }
  }
  shutdown(sockfd, 2);
  close(sockfd);
  continue_playing_orchestra (sockfd, addr, addrlen);

}

void thread_orchestra () {
  printf("thread_orchestra\n");
  // int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  // int opt = 1;
  uint16_t port = 1234;

  // assert(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR|SO_BROADCAST, &opt, sizeof(opt))==0);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  int addrlen = sizeof(addr); 

  // assert(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr))>=0);
  listen_for_connections(0, addr, addrlen);
  
}

int main (int argc, char **argv) {
  init_openAL();
  thread_orchestra();
  close_openAL();
  return EXIT_SUCCESS;
}