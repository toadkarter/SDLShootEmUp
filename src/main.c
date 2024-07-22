#include <SDL2/SDL.h>
#include "init.h"
#include "types.h"
#include "draw.h"
#include "input.h"
#include "global.h"
#include "definitions.h"

static Entity player;

void cleanup(void);

int main(void)
{
    memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity));

    initSDL(&app);

    SDL_Point position = {100, 100};
    player.position = position;
    player.texture = loadTexture("../resources/spaceship.png");

    SDL_Point size;
    SDL_QueryTexture(player.texture, NULL, NULL, &size.x, &size.y);
    player.size = size;

    atexit(cleanup);

    while (1)
    {
        prepareScene(&app);
        doInput();

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

        blit(player.texture, player.position.x, player.position.y);
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
