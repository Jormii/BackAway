#include <oslib/oslib.h>

#include "sound.h"

int sound_loop(OSL_SOUND *osl_sound, int channel);

bool_t sound_load(Sound *sound, int channel, bool_t looping, const char *mp3_path)
{
    sound->channel = channel;
    sound->looping = looping;
    sound->playing = FALSE;
    sound->osl_data = oslLoadSoundFileMP3(mp3_path, OSL_FMT_STREAM);
    if (sound->osl_data == NULL)
    {
        return FALSE;
    }

    return TRUE;
}

void sound_play(Sound *sound)
{
    if (sound->playing)
    {
        return;
    }

    if (sound->looping)
    {
        oslSetSoundEndCallback(sound->osl_data, sound_loop);
    }

    sound->playing = TRUE;
    oslPlaySound(sound->osl_data, sound->channel);
}

void sound_stop(Sound *sound)
{
    if (!sound->playing)
    {
        return;
    }

    sound->playing = FALSE;
    oslStopSound(sound->osl_data);
}

int sound_loop(OSL_SOUND *osl_sound, int channel)
{
    oslPlaySound(osl_sound, channel);
    return 1;
}
