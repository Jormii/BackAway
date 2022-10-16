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
    objective->state = LEVEL_OBJECTIVE_STATE_DEFAULT;
    objective->position.x = position->x;
    objective->position.y = position->y - objective->default_sprite->meta.height;

    // Load sounds
    objective->in_range_sfx = all_sounds + SOUND_ID_CHIME_IN_RANGE;
    objective->hit_sfx = all_sounds + SOUND_ID_CHIME_HIT;
}

void level_objective_update(LevelObjective *objective, GameState *game_state)
{
    if (objective->state == LEVEL_OBJECTIVE_STATE_ACTIVE)
    {
        animated_sprite_update(&(objective->active_anim_sprite), game_state->delta);
    }
}

void level_objective_draw(const LevelObjective *objective, const GameState *game_state)
{
    Vec2 pixel = game_state_camera_transform(game_state, &(objective->position));
    const Sprite *sprite = objective->default_sprite;
    switch (objective->state)
    {
    case LEVEL_OBJECTIVE_STATE_IN_RANGE:
        sprite = objective->in_range_sprite;
        break;
    case LEVEL_OBJECTIVE_STATE_ACTIVE:
        sprite = animated_sprite_get(&(objective->active_anim_sprite));
        break;
    case LEVEL_OBJECTIVE_STATE_DEFAULT:
    default:
        break;
    }

    sprite_draw(sprite, pixel.x, pixel.y, FALSE, FALSE);
}

void level_objective_set_active(LevelObjective *objective)
{
    objective->state = LEVEL_OBJECTIVE_STATE_ACTIVE;
    sound_play(objective->hit_sfx);
}

void level_objective_trigger_in_range(LevelObjective *objective, bool_t in_range)
{
    if (in_range)
    {
        if (objective->state != LEVEL_OBJECTIVE_STATE_IN_RANGE)
        {
            // Play sound to indicate player got in range
            sound_play(objective->in_range_sfx);
        }
        objective->state = LEVEL_OBJECTIVE_STATE_IN_RANGE;
    }
    else
    {
        objective->state = LEVEL_OBJECTIVE_STATE_DEFAULT;
    }
}

Vec2 level_objective_center(const LevelObjective *objective)
{
    // Width is shared across sprites. It dosn't matter which we pick
    return (Vec2){
        .x = objective->position.x + objective->default_sprite->meta.width / 2,
        .y = objective->position.y + objective->default_sprite->meta.height / 2};
}
