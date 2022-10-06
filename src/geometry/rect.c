#include "rect.h"
#include "macros.h"
#include "screen_buffer.h"

Vec2 rect_center(const Rect *rect)
{
    return (Vec2){
        .x = rect->origin.x + 0.5 * rect->width,
        .y = rect->origin.y + 0.5 * rect->height};
}

bool_t rect_contains_point(const Rect *rect, const Vec2 *p)
{
    float dx = p->x - rect->origin.x;
    float dy = p->y - rect->origin.y;
    return (dx >= 0 && dx <= rect->width) && (dy >= 0 && dy <= rect->height);
}

bool_t rect_within_rect(const Rect *r1, const Rect *r2)
{
    return r1->origin.x + r1->width >= r2->origin.x &&  // r1 right edge past r2 left
           r1->origin.x <= r2->origin.x + r2->width &&  // r1 left edge past r2 right
           r1->origin.y + r1->height >= r2->origin.y && // r1 top edge past r2 bottom
           r1->origin.y <= r2->origin.y + r2->height;   // r1 bottom edge past r2 top
}
