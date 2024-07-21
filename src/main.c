#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../include/init.h"
#include "../include/types.h"
#include "../include/draw.h"
#include "../include/input.h"

static App app;

void cleanup(void);

int main(void)
{
    memset(&app, 0, sizeof(App));

    initSDL(&app);

    atexit(cleanup);

    while (1)
    {
        prepareScene(&app);
        doInput();
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
