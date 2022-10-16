#include <math.h>
#include <stdlib.h>

#include "color.h"
#include "macros.h"
#include "screen_buffer.h"
#include "level_collider.h"
#include "draw_geometries.h"

void level_draw_polygon(const Polygon *collider, const Color *color, const GameState *game_state);

void level_collider_draw(const Polygon *collider, const GameState *game_state)
{
    Color color = {11, 1, 25, 255};
    if (collider->ephemeral)
    {
        color.red = 120;
        color.green = 190;
        color.blue = 252;
        color.alpha = 64;
    }

    level_draw_polygon(collider, &color, game_state);
}

void level_draw_polygon(const Polygon *collider, const Color *color, const GameState *game_state)
{
    size_t n = collider->n_vertices;
    for (size_t i = 0; i < n; ++i)
    {
        Vec2 p = game_state_camera_transform(game_state, collider->vertices + i);
        Vec2 q = game_state_camera_transform(game_state, collider->vertices + ((i + 1) % n));
        draw_line(&p, &q, color);
    }
}
