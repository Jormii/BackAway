#include <pspkernel.h>

#include "player.h"
#include "callbacks.h"
#include "screen_buffer.h"

PSP_MODULE_INFO("BackAway", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

void create_player(Player *player)
{
    player->pbody.mass = 50;
    player->pbody.position.x = 0;
    player->pbody.position.y = 0;
    player->pbody.velocity.x = 0;
    player->pbody.velocity.y = 0;

    player->sprite = sprite_allocate(30, 30);
    Sprite *sprite = player->sprite;
    {
        size_t idx = 0;
        for (int y = 0; y < sprite->height; ++y)
        {
            for (int x = 0; x < sprite->width; ++x)
            {
                rgb_t color = 0;
                switch (y % 3)
                {
                case 0:
                    color = 0x000000FF;
                    break;
                case 1:
                    color = 0x0000FF00;
                    break;
                case 2:
                    color = 0x00FF0000;
                default:
                    break;
                }

                sprite->bitmap[idx] = color;
                ++idx;
            }
        }
    }
}

int main()
{
    setup_callbacks();
    screen_buffer_init();

    Player player;
    create_player(&player);
    while (running())
    {
        screen_buffer_clear(0x00444444);

        player_update(&player, 0.1);

        screen_buffer_swap();
    }

    sceKernelExitGame();
    return 0;
}