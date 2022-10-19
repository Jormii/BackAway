#ifndef ASSETS_H
#define ASSETS_H

#include "types.h"
#include "sound.h"
#include "sprite.h"

typedef enum SpriteID_en
{
    // Player
    SPRITE_ID_PLAYER_IDLE_1,
    SPRITE_ID_PLAYER_IDLE_2,
    SPRITE_ID_PLAYER_RUN_1,
    SPRITE_ID_PLAYER_RUN_2,
    SPRITE_ID_PLAYER_JUMP,
    SPRITE_ID_PLAYER_FALL,
    SPRITE_ID_PLAYER_SWING,

    // Objective
    SPRITE_ID_OBJECTIVE_DEFAULT,
    SPRITE_ID_OBJECTIVE_IN_RANGE,
    SPRITE_ID_OBJECTIVE_ACTIVE_1,
    SPRITE_ID_OBJECTIVE_ACTIVE_2,
    SPRITE_ID_OBJECTIVE_ACTIVE_3,
    SPRITE_ID_OBJECTIVE_ACTIVE_4,

    // Goal
    SPRITE_ID_GOAL_INACTIVE,
    SPRITE_ID_GOAL_ACTIVE,

    // END
    _SPRITE_ID_COUNT_
} SpriteID;

typedef enum SoundID_en
{
    SOUND_ID_CHIME_IN_RANGE,
    SOUND_ID_CHIME_HIT,
    SOUND_ID_CHIME_ALL,
    _SOUND_ID_COUNT_
} SoundID;

typedef enum LevelID_en
{
    LEVEL_ID_ONE,
    LEVEL_ID_TWO,
    LEVEL_ID_THREE,
    LEVEL_ID_FOUR,
    LEVEL_ID_FIVE,
    _LEVEL_ID_COUNT_
} LevelID;

typedef struct LoadLevelData_en
{
    const char *path;
} LoadLevelData;

Sprite all_sprites[_SPRITE_ID_COUNT_];
Sound all_sounds[_SOUND_ID_COUNT_];
LoadLevelData all_leveles[_LEVEL_ID_COUNT_];

#endif
