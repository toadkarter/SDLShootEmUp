#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "init.h"
#include "types.h"
#include "definitions.h"
#include "global.h"

void initSDL()
{
    int rendererFlags = SDL_RENDERER_ACCELERATED;
    int windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0)
    {
        printf("Couldn't initialize SDL Image: %s\n", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow("Shooter",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SCREEN_WIDTH,
                                   SCREEN_HEIGHT,
                                   windowFlags);

    if (app.window == NULL)
    {
        printf("Failed to open window: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    if (app.renderer == NULL)
    {
        printf("Failed to create renderer: %s\n", SDL_GetError());
    }
}
