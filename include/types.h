#ifndef TYPES_H_
#define TYPES_H_

#include <stdbool.h>

#ifdef _WIN32
    #include <SDL.h>
#elif defined(__linux__)
    #include <SDL2/SDL.h>
#endif
#include "definitions.h"

typedef struct Delegate
{
    void (*logic)(void);
    void (*draw)(void);
} Delegate;

typedef struct App
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    Delegate delegate;
    int keyboard[MAX_KEYBOARD_KEYS];
} App;

typedef struct Entity
{
    SDL_Point position;
    SDL_Point positionDelta;
    SDL_Point size;
    int health;
    int reload;
    SDL_Texture* texture;
    bool spawned;
} Entity;

#endif
