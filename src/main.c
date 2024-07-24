#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <SDL.h>
#elif defined(__linux__)
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
#endif
#include "stage.h"
#include "init.h"
#include "types.h"
#include "draw.h"
#include "input.h"
#include "global.h"
#include "definitions.h"

static void cleanup(void);
static void capFrameRate(long* then, float* remainder);

int main(void)
{
    long then;
    float remainder;

    memset(&app, 0, sizeof(App));

    initSDL();
    atexit(cleanup);

    initStage();

    then = SDL_GetTicks();
    remainder = 0;

    while (1)
    {
        prepareScene();
        doInput();

        app.delegate.logic();
        app.delegate.draw();

        drawScene();

        capFrameRate(&then, &remainder);
    }

    return 0;
}

void cleanup(void)
{
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}

// I don't fully understand this - will need to look into it.
static void capFrameRate(long* then, float* remainder)
{
    long wait;
    long frameTime;

    wait = 16 + *remainder;
    *remainder -= (int)*remainder;

    frameTime = SDL_GetTicks() - *then;

    wait -= frameTime;

    if (wait < 1)
    {
        wait = 1;
    }

    SDL_Delay(wait);

    *remainder += 0.667;
    *then = SDL_GetTicks();
}
