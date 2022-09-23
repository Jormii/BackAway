#include <math.h>

#include "vec2.h"

void vec2_normalize(Vec2 *vector)
{
    float sqr_magnitude = (vector->x * vector->x) + (vector->y * vector->y);
    float magnitude = sqrtf(sqr_magnitude);

    vector->x /= magnitude;
    vector->y /= magnitude;
}