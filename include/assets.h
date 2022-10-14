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

    // Objective
    SPRITE_ID_OBJECTIVE_DEFAULT,
    SPRITE_ID_OBJECTIVE_IN_RANGE,
    SPRITE_ID_OBJECTIVE_ACTIVE_1,
    SPRITE_ID_OBJECTIVE_ACTIVE_2,
    SPRITE_ID_OBJECTIVE_ACTIVE_3,
    SPRITE_ID_OBJECTIVE_ACTIVE_4,

    // Goal
    SPRITE_ID_GOAL_INACTIVE_BOTTOM,
    SPRITE_ID_GOAL_INACTIVE_TOP,
    SPRITE_ID_GOAL_ACTIVE_BOTTOM,
    SPRITE_ID_GOAL_ACTIVE_TOP,

    // END
    _SPRITE_ID_COUNT_
} SpriteID;

Sprite all_sprites[_SPRITE_ID_COUNT_];

#endif
