#include "line.h"

bool_t lines_intersect(const Vec2 *p0, const Vec2 *pf, const Vec2 *q0, const Vec2 *qf)
{
    float num = (p0->x - q0->x) * (q0->y - qf->y) - (p0->y - q0->y) * (q0->x - qf->x);
    float den = (p0->x - pf->x) * (q0->y - qf->y) - (p0->y - pf->y) * (q0->x - qf->x);
    float t = num / den;

    bool_t intersection = t >= 0 && t <= 1;
    return intersection;
}
