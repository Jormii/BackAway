#include "camera.h"

Vec2 camera_transform(const Camera *camera, const Vec2 *position)
{
    float half_w = 0.5f * camera->screen.width;
    float half_h = 0.5f * camera->screen.height;
    Vec2 origin = {
        .x = camera->focus.x - half_w,
        .y = camera->focus.y - half_h};

    return vec2_subtract(position, &origin);
}
