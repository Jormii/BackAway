#include "player_aoe.h"
#include "draw_geometries.h"

void player_aoe_draw_pixel(int x, int y);
void player_aoe_draw_circunference(int xc, int yc, int x, int y);

void player_aoe_draw(const Vec2 *center, int radius)
{
    if (radius > 0) {
        player_aoe_draw(center, radius - 1);
    }

    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    player_aoe_draw_circunference(center->x, center->y, x, y);
    while (y >= x)
    {
        ++x;
        if (d > 0)
        {
            --y;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }

        player_aoe_draw_circunference(center->x, center->y, x, y);
    }
}

void player_aoe_draw_pixel(int x, int y)
{
    Vec2 point = {.x = x, .y = y};
    uint32_t black = 0;
    draw_point(&point, (Color *)&black);
}

void player_aoe_draw_circunference(int xc, int yc, int x, int y)
{
    player_aoe_draw_pixel(xc + x, yc + y);
    player_aoe_draw_pixel(xc - x, yc + y);
    player_aoe_draw_pixel(xc + x, yc - y);
    player_aoe_draw_pixel(xc - x, yc - y);
    player_aoe_draw_pixel(xc + y, yc + x);
    player_aoe_draw_pixel(xc - y, yc + x);
    player_aoe_draw_pixel(xc + y, yc - x);
    player_aoe_draw_pixel(xc - y, yc - x);
}
