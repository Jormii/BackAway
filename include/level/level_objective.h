#ifndef LEVEL_OBJECTIVE_H
#define LEVEL_OBJECTIVE_H

#include "vec2.h"
#include "types.h"
#include "sound.h"
#include "game_state.h"
#include "animated_sprite.h"

typedef struct LevelObjective_st
{
    Vec2 position;
    bool_t active;

    const Sprite *default_sprite;
    const Sprite *in_range_sprite;
    AnimatedSprite active_anim_sprite;
    Sound *hit_sfx;
} LevelObjective;

void level_objective_init(LevelObjective *objective, const Vec2 *position);
void level_objective_free(LevelObjective *objective);
void level_objective_update(LevelObjective *objective, GameState *game_state);
void level_objective_draw(const LevelObjective *objective, const GameState *game_state);

void level_objective_set_active(LevelObjective *objective);
Vec2 level_objective_center(const LevelObjective *objective);

#endif
