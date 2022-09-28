#ifndef LINE_H
#define LINE_H

#include "vec2.h"
#include "types.h"

Vec2 line_segment_interpolate(const Vec2 *p, const Vec2 *q, float t);
bool_t line_segments_intersect(const Vec2 *p0, const Vec2 *pf, const Vec2 *q0, const Vec2 *qf, float *out_t);

#endif
