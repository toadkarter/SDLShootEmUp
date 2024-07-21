#include "../include/draw.h"
#include "../include/types.h"
#include <SDL2/SDL.h>

void prepareScene(App* app)
{
    SDL_SetRenderDrawColor(app->renderer, 96, 128, 255, 255);
    SDL_RenderClear(app->renderer);
}

void drawScene(struct App* app)
{
    SDL_RenderPresent(app->renderer);
}
