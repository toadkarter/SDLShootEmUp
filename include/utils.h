#ifndef UTILS_H_
#define UTILS_H_

#include <stdbool.h>

struct Entity;

int maxi(int x, int y);
int mini(int x, int y);

bool isCollided(struct Entity* entity1, struct Entity* entity2);

#endif
