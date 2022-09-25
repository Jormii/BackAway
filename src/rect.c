#include "rect.h"
#include "macros.h"
#include "screen_buffer.h"

bool_t rect_within_rect(const Rect *r1, const Rect *r2)
{
    return r1->origin.x + r1->width >= r2->origin.x &&  // r1 right edge past r2 left
           r1->origin.x <= r2->origin.x + r2->width &&  // r1 left edge past r2 right
           r1->origin.y + r1->height >= r2->origin.y && // r1 top edge past r2 bottom
           r1->origin.y <= r2->origin.y + r2->height;   // r1 bottom edge past r2 top
}
