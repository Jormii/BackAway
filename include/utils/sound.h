#ifndef SOUND_H
#define SOUND_H

#include "types.h"

struct OSL_SOUND;
typedef struct Sound_st
{
    int channel;
    bool_t looping;
    bool_t playing;
    struct OSL_SOUND *osl_data;
} Sound;

bool_t sound_load(Sound *sound, int channel, bool_t looping, const char *mp3_path);
void sound_play(Sound *sound);
void sound_stop(Sound *sound);

#endif
