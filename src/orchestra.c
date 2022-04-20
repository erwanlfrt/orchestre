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

int main (int argc, char **argv) {
  // alutInit (0, NULL);
  ALCdevice *device;
  ALCcontext *context;
  device = alcOpenDevice (NULL);
  context = alcCreateContext (device, NULL);
  alcMakeContextCurrent (context);
  alutInitWithoutContext(0, NULL);
  printf("%i", alcGetCurrentContext() == NULL);
  
  alListener3f(AL_POSITION, 0, 0, 0);
  alListener3f(AL_VELOCITY, 0, 0, 0);
  strcpy(path, SOUND_PATH);
  strcat(path, "tribal-flute.wav");
  printf("device = %s", alcGetString(NULL, ALC_DEVICE_SPECIFIER));


  ALuint buffer;
  alGenBuffers((ALuint)1, &buffer);
  printf("%i", buffer);


  ALsizei size, freq;
  ALenum format;
  ALvoid *data;
  ALboolean loop = AL_FALSE;

  alutLoadWAVFile(path, &format, &data, &size, &freq, &loop);

  alBufferData(buffer, format, data, size, freq);

  ALuint source;
  alGenSources(1, &source);
  alSourcei(source, AL_BUFFER, buffer);


  // int context = GetContextRef();
  // printf("%i", alcGetCurrentContext() == NULL);
  // ALuint b = alutCreateBufferFromFile(path);
  // ALuint s = sources[0];
  
  // printf("%i", b);

  // lien buffer -> source
  // alGenSources(1, &s);
  // alSourcei(s, AL_BUFFER, (ALint)b);
  // assert(alGetError()==AL_NO_ERROR && "Failed to setup sound source");


  alSourcePlay(source);

  alutSleep (5);

  alDeleteSources(1, &source);
  alDeleteBuffers(1, &buffer);

  alcDestroyContext (context);
  alcCloseDevice (device);
  // alutExit ();

  return EXIT_SUCCESS;
}