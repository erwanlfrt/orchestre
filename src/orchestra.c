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
  alutInit (&argc, argv);
  alListener3f(AL_POSITION, 0, 0, 0);
  alListener3f(AL_VELOCITY, 0, 0, 0);
  strcpy(path, SOUND_PATH);
  strcat(path, "tribal-flute.wav");
  ALuint b = alutCreateBufferFromFile(path);
  ALuint s = sources[0];

  // lien buffer -> source
  alGenSources(1, &s);
  alSourcei(s, AL_BUFFER, (ALint)b);
  assert(alGetError()==AL_NO_ERROR && "Failed to setup sound source");

  alSourcePlay(s);

  alutSleep (5);

  alDeleteSources(1, &s);
  alDeleteBuffers(1, &b);
  alutExit ();

  return EXIT_SUCCESS;
}