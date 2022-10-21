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

    // Goal
    SPRITE_ID_GOAL_INACTIVE,
    SPRITE_ID_GOAL_ACTIVE,

    // Numbers
    SPRITE_ID_NUMBER_0,
    SPRITE_ID_NUMBER_1,
    SPRITE_ID_NUMBER_2,
    SPRITE_ID_NUMBER_3,
    SPRITE_ID_NUMBER_4,
    SPRITE_ID_NUMBER_5,
    SPRITE_ID_NUMBER_6,
    SPRITE_ID_NUMBER_7,
    SPRITE_ID_NUMBER_8,
    SPRITE_ID_NUMBER_9,

    // Menu
    SPRITE_ID_MENU_OFF,
    SPRITE_ID_MENU_ON,

    // END
    _SPRITE_ID_COUNT_
} SpriteID;

typedef enum SoundID_en
{
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
    LEVEL_ID_SIX,
    LEVEL_ID_SEVEN,
    LEVEL_ID_EIGHT,
    LEVEL_ID_NINE,
    LEVEL_ID_TEN,
    LEVEL_ID_ELEVEN,
    LEVEL_ID_TWELVE,
    LEVEL_ID_THIRTEEN,
    LEVEL_ID_FOURTEEN,
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
