#include <SDL2/SDL.h>

#include "stage.h"
#include "types.h"
#include "global.h"
#include "draw.h"

static void logic(void);
static void draw(void);
static void initPlayer(void);

static void doPlayer(void);
static void doBullets(void);

static void drawPlayer(void);
static void drawBullets(void);

static void fireBullet(void);

static Stage stage;
static Entity* player;

// Precaching this so we don't have to grab this every time a bullet is spawned.
static SDL_Texture* bulletTexture;

void initStage(void)
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    memset(&stage, 0, sizeof(Stage));

    // Both the first and the last pointers are the same.
    // This is because the list is currently empty.
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    initPlayer();

    bulletTexture = loadTexture("../resources/bullet.png");
}

static void initPlayer(void)
{
    // Making a player on the heap.
    // Both the first and last entity are equal to the player.
    // This is because no enemies exist yet.
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    stage.fighterTail->nextEntity = player;
    stage.fighterTail = player;

    SDL_Point playerSpawnPosition = {100, 100};
    player->position = playerSpawnPosition;
    player->texture = loadTexture("../resources/spaceship.png");
    SDL_QueryTexture(player->texture, NULL, NULL, &player->size.x, &player->size.y);
}

static void logic(void)
{
    doPlayer();
    doBullets();
}

static void doPlayer(void)
{
    player->positionDelta.x = 0;
    player->positionDelta.y = 0;

    if (player->reload > 0)
    {
        player->reload--;
    }

    if (app.keyboard[SDL_SCANCODE_UP])
    {
        player->positionDelta.y = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_DOWN])
    {
        player->positionDelta.y = PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_LEFT])
    {
        player->positionDelta.x = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_RIGHT])
    {
        player->positionDelta.x = PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_SPACE] && player->reload == 0)
    {
        fireBullet();
    }

    player->position.x += player->positionDelta.x;
    player->position.y += player->positionDelta.y;
}

static void fireBullet(void)
{
    Entity* bullet = NULL;

    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->nextEntity = bullet;
    stage.bulletTail = bullet;

    bullet->position = player->position;
    bullet->positionDelta.x = PLAYER_BULLET_SPEED;
    bullet->health = 1;
    bullet->texture = bulletTexture;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->size.x, &bullet->size.y);

    // Calculating the spawn position of the bullet with reference to the size of the texture.
    bullet->position.y += (player->size.y / 2) - (bullet->size.y / 2);

    player->reload = PLAYER_RELOAD_SPEED;
}

static void doBullets(void)
{
    // We start iterating through the list with the second node.
    Entity* currentBullet = NULL;
    Entity* previousBullet = &stage.bulletHead;

    // Loop through the linked list.
    for (currentBullet = stage.bulletHead.nextEntity; currentBullet != NULL; currentBullet = currentBullet->nextEntity)
    {
        // Update bullet movement.
        currentBullet->position.x += currentBullet->positionDelta.x;
        currentBullet->position.y += currentBullet->positionDelta.y;

        // If bullet is off screen, need to delete heap allocated memory.
        if (currentBullet->position.x > SCREEN_WIDTH)
        {
            // If this was the last bullet, then we replace
            // the tail with the previously cached last bullet.
            if (currentBullet == stage.bulletTail)
            {
                stage.bulletTail = previousBullet;
            }

            // Delete the current node and stick the nodes on the outside
            // of it together.
            previousBullet->nextEntity = currentBullet->nextEntity;
            free(currentBullet);
            currentBullet = previousBullet;
        }

        previousBullet = currentBullet;
    }
}

static void draw(void)
{
    drawPlayer();
    drawBullets();
}

static void drawPlayer(void)
{
    blit(player->texture, player->position.x, player->position.y);
}

static void drawBullets(void)
{
    Entity* currentBullet = NULL;

    for (currentBullet = stage.bulletHead.nextEntity; currentBullet != NULL; currentBullet = currentBullet->nextEntity)
    {
        blit(currentBullet->texture, currentBullet->position.x, currentBullet->position.y);
    }
}
