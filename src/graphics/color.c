#include "color.h"
#include "macros.h"

Color color_interpolate(const Color *c1, const Color *c2, float t)
{
    t = CLAMP(t, 0.0f, 1.0f);
    float u = 1.0f - t;
    return (Color){
        .red = u * c1->red + t * c2->red,
        .green = u * c1->green + t * c2->green,
        .blue = u * c1->blue + t * c2->blue,
        .alpha = u * c1->alpha + t * c2->alpha};
}
