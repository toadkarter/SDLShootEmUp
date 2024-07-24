#ifdef _WIN32
    #include <SDL.h>
    #include <SDL_image.h>
#elif defined(__linux__)
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
#endif

#include "draw.h"
#include "types.h"
#include "global.h"

void prepareScene()
{
    SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
    SDL_RenderClear(app.renderer);
}

void drawScene()
{
    SDL_RenderPresent(app.renderer);
}

SDL_Texture* loadTexture(char* filename)
{
    SDL_Texture* texture = NULL;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

    texture = IMG_LoadTexture(app.renderer, filename);

    return texture;
}

void blit(struct SDL_Texture* texture, int x, int y)
{
    SDL_Rect destination;
    destination.x = x;
    destination.y = y;

    SDL_QueryTexture(texture, NULL, NULL, &destination.w, &destination.h);

    SDL_RenderCopy(app.renderer, texture, NULL, &destination);
}
