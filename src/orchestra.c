#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

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

int main (int argc, char **argv) {
  init_openAL();
  while(1);
  return EXIT_SUCCESS;
}