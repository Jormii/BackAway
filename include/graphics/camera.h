#ifndef CAMERA_H
#define CAMERA_H

#include "rect.h"

typedef struct Camera_st
{
    Rect screen;
    Vec2 focus;
} Camera;

Vec2 camera_transform(const Camera *camera, const Vec2 *position);

#endif
