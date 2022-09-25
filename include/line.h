#ifndef LINE_H
#define LINE_H

#include "vec2.h"
#include "types.h"

bool_t lines_intersect(const Vec2 *p0, const Vec2 *pf, const Vec2 *q0, const Vec2 *qf);

#endif