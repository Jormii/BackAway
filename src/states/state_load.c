#include "path.h"
#include "assets.h"
#include "state_load.h"

#include <stdio.h>

AssetLoadData sprite_load_data[_SPRITE_ID_COUNT_] = {
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
};

void load_state_init(GameState *game_state)
{
    // Load sprites
    for (size_t i = 0; i < _SPRITE_ID_COUNT_; ++i)
    {
        const AssetLoadData *data = sprite_load_data + i;
        if (!sprite_load(all_sprites + data->index, data->path))
        {
            printf("Sprite %s couldn't be loaded\n", data->path);
        }
    }
}

void load_state_update(GameState *game_state)
{
    game_state->state_id = GAME_STATE_LEVEL;
}

void load_state_draw(const GameState *game_state)
{
}
