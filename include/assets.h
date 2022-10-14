#ifndef ASSETS_H
#define ASSETS_H

#include "types.h"
#include "sprite.h"

typedef struct AssetLoadData_st
{
    size_t index;
    const char *path;
} AssetLoadData;

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

Sprite all_sprites[_SPRITE_ID_COUNT_];

#endif
