#ifndef UTILS_H_
#define UTILS_H_

#include <stdbool.h>

struct Entity;
struct SDL_Point;

int maxi(int x, int y);
int mini(int x, int y);

bool isCollided(struct Entity* entity1, struct Entity* entity2);
void calculateDirection(struct SDL_Point position1, struct SDL_Point position2, struct SDL_Point* direction);

#endif
