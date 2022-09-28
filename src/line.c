#include "line.h"

Vec2 line_segment_interpolate(const Vec2 *p, const Vec2 *q, float t)
{
    Vec2 left = vec2_mult_scalar(1.0f - t, p);
    Vec2 right = vec2_mult_scalar(t, q);
    return vec2_add(&left, &right);
}

bool_t line_segments_intersect(const Vec2 *p0, const Vec2 *pf, const Vec2 *q0, const Vec2 *qf, float *out_t)
{
    float num = (p0->x - q0->x) * (q0->y - qf->y) - (p0->y - q0->y) * (q0->x - qf->x);
    float den = (p0->x - pf->x) * (q0->y - qf->y) - (p0->y - pf->y) * (q0->x - qf->x);
    float t = num / den;

    bool_t intersection = t >= 0 && t <= 1;
    *out_t = t;

    return intersection;
}
