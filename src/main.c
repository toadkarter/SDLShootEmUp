#include <SDL2/SDL.h>
#include "init.h"
#include "types.h"
#include "draw.h"
#include "input.h"
#include "global.h"
#include "definitions.h"

static Entity player;
static Entity bullet;

void cleanup(void);

int main(void)
{
    memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity));
    memset(&bullet, 0, sizeof(Entity));

    initSDL(&app);

    SDL_Point position = {100, 100};
    player.position = position;
    player.texture = loadTexture("../resources/spaceship.png");

    SDL_Point playerSize;
    SDL_QueryTexture(player.texture, NULL, NULL, &playerSize.x, &playerSize.y);
    player.size = playerSize;

    bullet.texture = loadTexture("../resources/bullet.png");
    SDL_Point bulletTexture;
    SDL_QueryTexture(bullet.texture, NULL, NULL, &bulletTexture.x, &bulletTexture.y);

    atexit(cleanup);

    while (1)
    {
        prepareScene(&app);
        doInput();

        player.position.x += player.lastPosition.x;
        player.position.y += player.lastPosition.y;

        if (app.up)
        {
            player.position.y -= PLAYER_SPEED;

            if (player.position.y < 0)
            {
                player.position.y = 0;
            }
        }

        if (app.down)
        {
            player.position.y += PLAYER_SPEED;

            if (player.position.y > SCREEN_HEIGHT - player.size.y)
            {
                player.position.y = SCREEN_HEIGHT - player.size.y;
            }
        }

        if (app.left)
        {
            player.position.x -= PLAYER_SPEED;

            if (player.position.x < 0)
            {
                player.position.x = 0;
            }
        }

        if (app.right)
        {
            player.position.x += PLAYER_SPEED;

            if (player.position.x > SCREEN_WIDTH - player.size.x)
            {
                player.position.x = SCREEN_WIDTH - player.size.x;
            }
        }

        if (app.fire && bullet.health == 0)
        {
            bullet.position = player.position;
            bullet.lastPosition.x = BULLET_SPEED;
            bullet.lastPosition.y = 0;
            bullet.health = 1;
        }

        bullet.position.x += bullet.lastPosition.x;
        bullet.position.y += bullet.lastPosition.y;

        if (bullet.position.x > SCREEN_WIDTH)
        {
            bullet.health = 0;
        }

        blit(player.texture, player.position.x, player.position.y);

        if (bullet.health > 0)
        {
            blit(bullet.texture, bullet.position.x, bullet.position.y);
        }

        drawScene(&app);

        SDL_Delay(16);
    }

    return 0;
}

void cleanup(void)
{
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}
