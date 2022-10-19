#include "assets.h"
#include "level_objective.h"

void level_objective_init(LevelObjective *objective, const Vec2 *position)
{
    // Load sprites
    objective->default_sprite = all_sprites + SPRITE_ID_OBJECTIVE_DEFAULT;
    objective->in_range_sprite = all_sprites + SPRITE_ID_OBJECTIVE_IN_RANGE;

    AnimatedSprite *anim_sprite = &(objective->active_anim_sprite);
    animated_sprite_init(anim_sprite, 4, 6);
    anim_sprite->frames[0] = all_sprites + SPRITE_ID_OBJECTIVE_ACTIVE_1;
    anim_sprite->frames[1] = all_sprites + SPRITE_ID_OBJECTIVE_ACTIVE_2;
    anim_sprite->frames[2] = all_sprites + SPRITE_ID_OBJECTIVE_ACTIVE_3;
    anim_sprite->frames[3] = all_sprites + SPRITE_ID_OBJECTIVE_ACTIVE_4;

    // Position requires sprite's data
    objective->active = FALSE;
    objective->position.x = position->x;
    objective->position.y = position->y - objective->default_sprite->meta.height;

    // Load sounds
    objective->hit_sfx = all_sounds + SOUND_ID_CHIME_HIT;
}

void level_objective_update(LevelObjective *objective, GameState *game_state)
{
    if (objective->active)
    {
        animated_sprite_update(&(objective->active_anim_sprite), game_state->delta);
    }
}

void level_objective_draw(const LevelObjective *objective, const GameState *game_state)
{
    const Sprite *sprite = objective->default_sprite;
    if (objective->active)
    {
        sprite = animated_sprite_get(&(objective->active_anim_sprite));
    }

    Vec2 pixel = game_state_camera_transform(game_state, &(objective->position));
    sprite_draw(sprite, pixel.x, pixel.y, 1.0f, FALSE, FALSE);
}

void level_objective_set_active(LevelObjective *objective)
{
    objective->active = TRUE;
    sound_play(objective->hit_sfx);
}

Vec2 level_objective_center(const LevelObjective *objective)
{
    // Width is shared across sprites. It dosn't matter which we pick
    return (Vec2){
        .x = objective->position.x + objective->default_sprite->meta.width / 2,
        .y = objective->position.y + objective->default_sprite->meta.height / 2};
}
