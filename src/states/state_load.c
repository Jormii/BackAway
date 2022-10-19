#include <stdio.h>
#include <stdlib.h>

#include "path.h"
#include "level.h"
#include "assets.h"
#include "state_load.h"
#include "state_level.h"

typedef struct SpriteLoadData_st
{
    SpriteID sprite_id;
    const char *path;
} SpriteLoadData;

typedef struct SoundLoadData_st
{
    int channel;
    SoundID sound_id;
    const char *path;
} SoundLoadData;

SpriteLoadData sprite_load_data[_SPRITE_ID_COUNT_] = {
    {SPRITE_ID_PLAYER_IDLE_1, SPRITE("Character_Idle1")},
    {SPRITE_ID_PLAYER_IDLE_2, SPRITE("Character_Idle2")},
    {SPRITE_ID_PLAYER_RUN_1, SPRITE("Character_Run1")},
    {SPRITE_ID_PLAYER_RUN_2, SPRITE("Character_Run2")},
    {SPRITE_ID_PLAYER_JUMP, SPRITE("Character_Jump")},
    {SPRITE_ID_PLAYER_FALL, SPRITE("Character_Fall")},
    {SPRITE_ID_PLAYER_SWING, SPRITE("Character_Swing")},

    {SPRITE_ID_OBJECTIVE_DEFAULT, SPRITE("Objective_Default")},
    {SPRITE_ID_OBJECTIVE_IN_RANGE, SPRITE("Objective_InRange")},
    {SPRITE_ID_OBJECTIVE_ACTIVE_1, SPRITE("Objective_Active1")},
    {SPRITE_ID_OBJECTIVE_ACTIVE_2, SPRITE("Objective_Active2")},
    {SPRITE_ID_OBJECTIVE_ACTIVE_3, SPRITE("Objective_Active3")},
    {SPRITE_ID_OBJECTIVE_ACTIVE_4, SPRITE("Objective_Active4")},
    {SPRITE_ID_GOAL_INACTIVE, SPRITE("Goal_Inactive")},
    {SPRITE_ID_GOAL_ACTIVE, SPRITE("Goal_Active")},
    {SPRITE_ID_MENU_OFF, SPRITE("MenuButton_Off")},
    {SPRITE_ID_MENU_ON, SPRITE("MenuButton_On")}};

SoundLoadData sound_load_data[_SOUND_ID_COUNT_] = {
    {0, SOUND_ID_CHIME_HIT, MP3("Chime_Hit")},
    {1, SOUND_ID_CHIME_ALL, MP3("Chime_All")}};

LoadLevelData all_leveles[_LEVEL_ID_COUNT_] = {
    {LEVEL("Level1")},
    {LEVEL("Level2")},
    {LEVEL("Level3")},
    {LEVEL("Level4")},
    {LEVEL("Level5")}};

void load_state_init(GameState *game_state)
{
    // Load sprites
    for (size_t i = 0; i < _SPRITE_ID_COUNT_; ++i)
    {
        const SpriteLoadData *data = sprite_load_data + i;
        if (!sprite_load(all_sprites + data->sprite_id, data->path))
        {
            printf("Sprite %s couldn't be loaded\n", data->path);
        }
    }

    // Load sounds
    for (size_t i = 0; i < _SOUND_ID_COUNT_; ++i)
    {
        const SoundLoadData *data = sound_load_data + i;
        if (!sound_load(all_sounds + data->sound_id, data->channel, FALSE, data->path))
        {
            printf("Sound %s couldn't be loaded\n", data->path);
        }
    }
}

void load_state_update(GameState *game_state)
{
    game_state->state_id = GAME_STATE_MENU;
}

void load_state_draw(const GameState *game_state)
{
}
