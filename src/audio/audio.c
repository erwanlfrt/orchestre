#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "../musician/musician.h"
#include "./audio.h"
#include "../orchestra/orchestra.h"
#include "../direction/direction.h"

#define SOUND_PATH "./sounds/"

char path[300];
ALuint source;
ALuint buffer;
ALCdevice *device;
ALCcontext *context;
extern int current_nb_instru;
extern long int nthr;

/**
 * @brief Custom load wav file function to get rid of the deprecated function alutLoadWavFile().
 * 
 */
void load_wav_file(ALbyte *fileName, ALenum *format, void **data, ALsizei *size, ALsizei *frequency
    #if !defined(__APPLE__)
      , ALboolean *loop
    #endif
  ) {
  ALfloat freq;
  *data = alutLoadMemoryFromFile (fileName, format, size, &freq);
  if (*data == NULL) {
    return;
  }

  if (frequency) {
    *frequency = (ALsizei) freq;
  }

  #if !defined(__APPLE__)
    if (loop) {
      *loop = AL_FALSE;
    }
  #endif
}

/**
 * @brief Load buffer and source for a musician
 * 
 * @param sockfd sockfd/id of the musician
 */
void load (int sockfd) {
  ALsizei size, freq;
  ALenum format;
  ALvoid *data;
  ALboolean loop = AL_FALSE;
  Musician * musician = get_musician(sockfd); // get the musician from his sockd to get his partition.
  strcpy(path, SOUND_PATH); 
  strcat(path, get_partition(sockfd)); // path = SOUND_PATH + partition
  alGenBuffers((ALuint)1, &(buffer)); // generate buffer
  load_wav_file(path, &format, &data, &size, &freq, &loop); // load wav file in memory
  alBufferData(buffer, format, data, size, freq); // load data in buffer
  alGenSources(1, &(source)); // generate source
  alSourcei(source, AL_BUFFER, buffer); // link source and buffer
  musician->source = source; // update musician source
  musician->buffer = buffer; // update musician buffer
}

/**
 * @brief Play the partition of a musician
 * 
 * @param sockfd sockfd/id of the musician
 */
void play (int sockfd) {
  Musician * musician = get_musician(sockfd);
  alSourcePlay(musician->source);
}

/**
 * @brief Put a musician in pause
 * 
 * @param sockfd sockfd/id of the musician
 */
void pause_sound (int sockfd) {
  Musician * musician = get_musician(sockfd);
  alSourcePause(musician->source);
}

/**
 * @brief Stop a musician, he won't be able to play anymore.
 * 
 * @param sockfd sockfd/id of the musician
 */
void stop (int sockfd) {
  Musician * musician = get_musician(sockfd);
  alSourceStop(musician->source);
  delete_source_buffer(musician->source, musician->buffer); // delete source and buffer of the musician
  
  // Delete the musician from the musicians array and delete is thread
  Musician tmp[N_INSTRU];
  int index = 0;
  for (int i = 0; i < current_nb_instru; i++) {
    if (musicians[i].sockfd != sockfd) {
      tmp[index] = musicians[i]; // copy exclusively the other musicians
      index++;
    }
  }

  pthread_t tmp_thr[N_INSTRU];
  int index_j = 0;
  for (int j = 0; j < nthr ; j++) {
    if (j != musician->nthr) {
      tmp_thr[index_j] = j; // copy exclusively the other threads
      index_j++;
    }
  }
  current_nb_instru--;
  nthr--;
  *musicians = *tmp; // update musicians array
  *thr = *tmp_thr; // update threads array
}

/**
 * @brief Set the position of a musician
 * 
 * @param sockfd sockfd/id of a musician
 * @param position new position of the musicians
 */
void set_position(int sockfd, int position) {
    Musician * musician = get_musician(sockfd);
    float step = 2.0;
    switch (position) {
        case N:
            alSource3f(musician->source, AL_POSITION, 0.0, step, 0.0);
            break;
        case S:
            alSource3f(musician->source, AL_POSITION, 0.0, -step, 0.0);
            break;
        case E:
            alSource3f(musician->source, AL_POSITION, step, 0.0, 0.0);
            break;
        case W:
            alSource3f(musician->source, AL_POSITION, -step, 0.0, 0.0);
            break;
        case NE:
            alSource3f(musician->source, AL_POSITION, step, step, 0.0);
            break;
        case NW:
            alSource3f(musician->source, AL_POSITION, -step, step, 0.0);
            break;
        case SE:
            alSource3f(musician->source, AL_POSITION, step, -step, 0.0);
            break;
        case SW:
            alSource3f(musician->source, AL_POSITION, -step, -step, 0.0);
            break;
        case CENTER:
            alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0);
            break;
        default:
            alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0); // default center
            break;
    }
}

/**
 * @brief Init OpenAL library. No Alut init because it doesn't work on MacOS. Instead init OpenAL library and call alutInitWithoutContext.
 * 
 */
void init_openAL () {
  device = alcOpenDevice (NULL); // open audio device
  context = alcCreateContext (device, NULL); // create context
  alcMakeContextCurrent (context); // created context becomes the current context
  alutInitWithoutContext(0, NULL); // init alut without context because context has been created by openAL
  alListener3f(AL_POSITION, 0, 0, 0); // set listener position to center
  alListener3f(AL_VELOCITY, 0, 0, 0); //set listener velocity to zero.
  alutSleep (1);
}

/**
 * @brief Delete a source and a buffer
 * 
 * @param source source to delete
 * @param buffer buffer to delete
 */
void delete_source_buffer (ALuint source, ALuint buffer) {
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
}

/**
 * @brief Close OpenAL
 * 
 */
void close_openAL () {
  alcDestroyContext (context);
  alcCloseDevice (device);
}
