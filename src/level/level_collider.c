#include <math.h>
#include <stdlib.h>

#include "color.h"
#include "macros.h"
#include "screen_buffer.h"
#include "level_collider.h"
#include "draw_geometries.h"

float random_zero_to_one();
void level_collider_draw_ephemeral(const Polygon *collider, const GameState *game_state);
void level_collider_draw_non_ephemeral(const Polygon *collider, const GameState *game_state);

void level_collider_draw(const Polygon *collider, const GameState *game_state)
{
    if (collider->ephemeral)
    {
        level_collider_draw_ephemeral(collider, game_state);
    }
    else
    {
        level_collider_draw_non_ephemeral(collider, game_state);
    }
}

float random_zero_to_one()
{
    return rand() / (float)RAND_MAX;
}

void level_collider_draw_ephemeral(const Polygon *collider, const GameState *game_state)
{
    Color color = {120, 190, 252, 64};
    Rect rect_copy = collider->bbox;
    rect_copy.origin = game_state_camera_transform(game_state, &(rect_copy.origin));
    draw_rect(&rect_copy, &color);
}

void level_collider_draw_non_ephemeral(const Polygon *collider, const GameState *game_state)
{
    Color color = {11, 1, 25, 255};
    Rect rect_copy = collider->bbox;
    rect_copy.origin = game_state_camera_transform(game_state, &(rect_copy.origin));
    draw_rect(&rect_copy, &color);
}
