#ifndef TYPES_H_
#define TYPES_H_

#include <SDL2/SDL.h>

typedef struct App
{
    SDL_Renderer* renderer;
    SDL_Window* window;
} App;

typedef struct Entity
{
    int x;
    int y;
    SDL_Texture* texture;
} Entity;

#endif
