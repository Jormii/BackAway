#ifndef BUTTON_H
#define BUTTON_H

#include "vec2.h"
#include "sprite.h"

struct UIButton_st;

typedef void (*UIButtonOnPress_fp)(struct UIButton_st *button);
typedef void (*UIButtonOnHighlighted_fp)(struct UIButton_st *button);

typedef struct UIButton_st
{
    Vec2 position;
    const Sprite *sprite;
    UIButtonOnPress_fp on_press_cb;
    UIButtonOnHighlighted_fp on_highlighted_cb;
    void *cb_ptr;
} UIButton;

typedef struct UIButtonCollection_st
{
    size_t n_buttons;
    UIButton *buttons;
    size_t highlighted_idx;
} UIButtonCollection;

void ui_button_collection_init(UIButtonCollection *button_coll, size_t n_buttons);
void ui_button_collection_update(UIButtonCollection *button_coll);

#endif
