#ifndef TYPES_H_
#define TYPES_H_

#include <SDL2/SDL.h>

typedef struct App
{
    SDL_Renderer* renderer;
    SDL_Window* window;

    int up;
    int down;
    int left;
    int right;
    int fire;
} App;

typedef struct Entity
{
    SDL_Point position;
    SDL_Point lastPosition;
    SDL_Point size;
    int health;
    SDL_Texture* texture;
} Entity;

#endif
