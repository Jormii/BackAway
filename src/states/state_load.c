#include "path.h"
#include "assets.h"
#include "state_load.h"

AssetLoadData sprite_load_data[_SPRITE_ID_COUNT_] = {
    {SPRITE_ID_OBJECTIVE_DEFAULT, SPRITE("Objective_Default")},
    {SPRITE_ID_OBJECTIVE_IN_RANGE, SPRITE("Objective_InRange")},
    {SPRITE_ID_OBJECTIVE_ACTIVE_1, SPRITE("Objective_Active1")},
    {SPRITE_ID_OBJECTIVE_ACTIVE_2, SPRITE("Objective_Active2")},
    {SPRITE_ID_OBJECTIVE_ACTIVE_3, SPRITE("Objective_Active3")},
    {SPRITE_ID_OBJECTIVE_ACTIVE_4, SPRITE("Objective_Active4")}};

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
    // TODO
    //game_state->state_id = GAME_STATE_MAIN_MENU;
    game_state->state_id = GAME_STATE_LEVEL;
}

void load_state_draw(const GameState *game_state)
{
}
