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

#define N_INSTRU 10
#define MAX_LEN 50
#define SOUND_PATH "./sounds/"


ALuint sources[N_INSTRU];
char path[300];



void init_openAL () {
  ALCdevice *device;
  ALCcontext *context;
  ALuint buffer;
  ALsizei size, freq;
  ALenum format;
  ALvoid *data;
  ALboolean loop = AL_FALSE;
  ALuint source;
  device = alcOpenDevice (NULL);
  context = alcCreateContext (device, NULL);
  alcMakeContextCurrent (context);
  alutInitWithoutContext(0, NULL);
  alListener3f(AL_POSITION, 0, 0, 0);
  alListener3f(AL_VELOCITY, 0, 0, 0);
  strcpy(path, SOUND_PATH);
  strcat(path, "tribal-flute.wav");
  alGenBuffers((ALuint)1, &buffer);
  alutLoadWAVFile(path, &format, &data, &size, &freq, &loop);
  alBufferData(buffer, format, data, size, freq);
  alGenSources(1, &source);
  alSourcei(source, AL_BUFFER, buffer);
  alSourcePlay(source);
  alutSleep (5);
  alDeleteSources(1, &source);
  alDeleteBuffers(1, &buffer);
  alcDestroyContext (context);
  alcCloseDevice (device);
}

void * thread_musician (void * args) {
  int sock = ((int*)args)[0];
  printf("New connection !!! sockfd = %i\n",sock);
}


void thread_orchestra () {
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

  pthread_t thr[N_INSTRU];
	long int nthr = 0;
	void* args[2];

  while(nthr<N_INSTRU) {
    int new_socket;
    new_socket = accept(sockfd, (struct sockaddr *)&addr, (socklen_t*)&addrlen);
    args[0] = (void*)(long int)new_socket;
    args[1] = (void*)nthr;
    pthread_create(&thr[nthr], NULL, &thread_musician, args);
    nthr++;
  }
  printf("thread_orchestra done");
}

int main (int argc, char **argv) {
  // init_openAL();
  thread_orchestra();
  while(1);
  return EXIT_SUCCESS;
}