#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <AL/al.h>
#include <AL/alc.h>
#include "../freealut/alut.h"

#include "semaphore.h"

#define N_INSTRU 10
#define MAX_LEN 50

ALuint sources[N_INSTRU];

int main()
{
    int nthr = 0;

    char name[MAX_LEN];
    switch(nthr)
    {
        case 0:
            strncpy(name, "soundLiquid.wav", MAX_LEN);
            break;
        case 1:
            strncpy(name, "soundLiquid.wav", MAX_LEN);
            break;
        case 2:
            strncpy(name, "soundLiquid.wav", MAX_LEN);
            break;
        default:
            strncpy(name, "soundLiquid.wav", MAX_LEN);
            break;
    }

    printf("%s \n", name);

    ALuint b = alutCreateBufferFromFile("soundLiquid.wav");
    ALuint s = sources[nthr];

    // lien buffer -> source
    alGenSources(1, &s);
    alSourcei(s, AL_BUFFER, (ALint)b);

    alSourcePlay(s);

    //alSourcePause(s);

    //alSourceStop(s);

    alDeleteSources(1, &s);
    alDeleteBuffers(1, &b);

    return 0;
}
