#include "level.h"
#include "draw_geometries.h"

void level_update(Level *level, GameState *game_state)
{
}

void level_draw(const Level *level, const GameState *game_state)
{
    Color black = {0, 0, 0, 0};
    Color green = {128, 255, 128, 0};
    for (size_t i = 0; i < level->n_colliders; ++i)
    {
        draw_polygon(level->colliders + i, &green, &black);
    }
}
