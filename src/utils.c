#ifdef _WIN32
    #include <SDL.h>
#elif defined(__linux__)
    #include <SDL2/SDL.h>
#endif

#include "types.h"
#include "utils.h"

int maxi(int x, int y)
{
    if (x >= y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

int mini(int x, int y)
{
    if (x <= y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

bool isCollided(Entity* entity1, Entity* entity2)
{
    if (entity1->side == entity2->side)
    {
        return false;
    }

    int rightmostLeftEdge = maxi(entity1->position.x, entity2->position.x);

    int rightEdge1 = entity1->position.x + entity1->size.x;
    int rightEdge2 = entity2->position.x + entity2->size.x;

    int leftmostRightEdge = mini(rightEdge1, rightEdge2);

    bool isOverlappingHorizontally = rightmostLeftEdge < leftmostRightEdge;

    int highestBottomEdge = maxi(entity1->position.y, entity2->position.y);

    int topEdge1 = (entity1->position.y + entity1->size.y);
    int topEdge2 = (entity2->position.y + entity2->size.y);

    int lowestTopEdge = mini(topEdge1, topEdge2);

    bool isOverlappingVertically = highestBottomEdge < lowestTopEdge;

    return isOverlappingHorizontally && isOverlappingVertically;
}

void calculateDirection(struct SDL_Point position1, struct SDL_Point position2, struct SDL_Point* direction)
{
    // Getting distance so that we can normalize our result.
    int steps = maxi(abs(position1.x - position2.x), abs(position1.y - position2.y));

    if (steps == 0)
    {
        direction->x = 0;
        direction->y = 0;
        return;
    }

    direction->x = (position1.x - position2.x);
    direction->x /= steps;

    direction->y = (position1.y - position2.y);
    direction->y /= steps;
}
