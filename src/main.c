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

int main(void)
{
    memset(&app, 0, sizeof(App));

    initSDL();
    atexit(cleanup);

    initStage();

    while (1)
    {
        prepareScene();
        doInput();

        app.delegate.logic();
        app.delegate.draw();

        drawScene();

        SDL_Delay(16);
    }

    return 0;
}

void cleanup(void)
{
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}
