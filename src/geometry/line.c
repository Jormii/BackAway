#include "line.h"
#include "macros.h"

Vec2 line_segment_interpolate(const Vec2 *p, const Vec2 *q, float t)
{
    Vec2 left = vec2_mult_scalar(1.0f - t, p);
    Vec2 right = vec2_mult_scalar(t, q);
    return vec2_add(&left, &right);
}

bool_t line_segments_intersect(const Vec2 *p0, const Vec2 *pf, const Vec2 *q0, const Vec2 *qf, float *out_t)
{
    // Cramer's rule
    Vec2 v = vec2_subtract(pf, p0);
    Vec2 u = vec2_subtract(qf, q0);
    Vec2 w = vec2_subtract(q0, p0);

    // We name 'pt' and 'qt' the scalar parameters in 'p = p0 + pt*(pf - p0)'
    float det_PT = w.y * u.x - w.x * u.y;
    float det_QT = v.x * w.y - v.y * w.x;
    float det_A = v.y * u.x - v.x * u.y;

    if (EQUAL_EPSILON(det_A, 0.0f, __FLT_EPSILON__))
    {
        return FALSE;
    }

    float pt = det_PT / det_A;
    float qt = det_QT / det_A;

    *out_t = pt;
    bool_t intersection = (pt >= 0 && pt <= 1) && (qt >= 0 && qt <= 1);

    return intersection;
}
