#include <SDL2/SDL.h>
#include "../include/init.h"
#include "../include/types.h"
#include "../include/draw.h"
#include "../include/input.h"
#include "../include/global.h"

static Entity player;

void cleanup(void);

int main(void)
{
    memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity));

    initSDL(&app);

    player.x = 100;
    player.y = 100;
    player.texture = loadTexture("../resources/spaceship.png");

    atexit(cleanup);

    while (1)
    {
        prepareScene(&app);
        doInput();

        blit(player.texture, player.x, player.y);
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
