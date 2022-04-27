#include <AL/al.h>

#ifndef AUDIO_H_
#define AUDIO_H_

#define N_INSTRU 2


void load (int sockfd);
void play (int sockfd);
void pause_sound (int sockfd);
void stop (int sockfd);
void set_position(int sockfd, int position);
void init_openAL();
void delete_source_buffer (ALuint source, ALuint buffer);
void close_openAL();

#endif
