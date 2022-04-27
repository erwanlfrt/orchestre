#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <pthread.h>
#include <string.h>
#include "../musician/musician.h"
#include "./audio.h"
#include "../orchestra/orchestra.h"
#include "../direction/direction.h"


char path[300];
#define SOUND_PATH "./sounds/"
ALuint source;
ALuint buffer;
ALCdevice *device;
ALCcontext *context;


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
  delete_source_buffer(musician->source, musician->buffer);
  
  // Delete the musician from the musicians array and delete is thread
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
  for (int j = 0; j < nthr ; j++) {
    if (j != musician->nthr) {
      tmp_thr[index_j] = j;
      index_j++;
    }
  }
  current_nb_instru--;
  nthr--;
  *musicians = *tmp;
  *thr = *tmp_thr;
}

/**
 * @brief Set the position of a musician
 * 
 * @param sockfd sockfd/id of a musician
 * @param position new position of the musicians
 */
void set_position(int sockfd, int position) {
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
            alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0);
            break;
    }
}

/**
 * @brief Init OpenAL library. No Alut init because it doesn't work on MacOS. Instead init OpenAL library and call alutInitWithoutContext.
 * 
 */
void init_openAL () {
  device = alcOpenDevice (NULL);
  context = alcCreateContext (device, NULL);
  alcMakeContextCurrent (context);
  alutInitWithoutContext(0, NULL);
  alListener3f(AL_POSITION, 0, 0, 0);
  alListener3f(AL_VELOCITY, 0, 0, 0);
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
