#ifdef _WIN32
    #include <SDL.h>
#elif defined(__linux__)
    #include <SDL2/SDL.h>
#endif

#include "stage.h"
#include "types.h"
#include "global.h"
#include "draw.h"
#include "definitions.h"

static void logic(void);
static void draw(void);
static void initPlayer(void);

static void doPlayer(void);
static void doFighters(void);
static void doBullets(void);

static void drawFighters(void);
static void drawBullets(void);

static void spawnEnemies();
static void fireBullet(void);

static Entity* player;

Entity fighters[MAX_ENTITIES_SPAWNED] = {0};
Entity bullets[MAX_ENTITIES_SPAWNED] = {0};

// Precaching these so we don't have to grab this every time a bullet is spawned.
static SDL_Texture* bulletTexture;
static SDL_Texture* enemyTexture;

static int enemySpawnTimer;

void initStage(void)
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    initPlayer();

    bulletTexture = loadTextureFromFileName("bullet.png");
    enemyTexture = loadTextureFromFileName("enemy.png");

    enemySpawnTimer = 0;
}

static void initPlayer(void)
{
    fighters[PLAYER_INDEX].spawned = true;
    player = &fighters[PLAYER_INDEX];

    SDL_Point playerSpawnPosition = {100, 100};
    player->position = playerSpawnPosition;
    player->texture = loadTextureFromFileName("spaceship.png");
    SDL_QueryTexture(player->texture, NULL, NULL, &player->size.x, &player->size.y);
}

static void logic(void)
{
    doPlayer();
    doFighters();

    doBullets();
    spawnEnemies();
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
}

static void doFighters(void)
{
    for (int i = 0; i < MAX_ENTITIES_SPAWNED; i++)
    {
        if (fighters[i].spawned)
        {
            fighters[i].position.x += fighters[i].positionDelta.x;
            fighters[i].position.y += fighters[i].positionDelta.y;

            if (i != PLAYER_INDEX && fighters[i].position.x < -fighters[i].size.x)
            {
                fighters[i].spawned = false;
            }
        }
    }
}

static void fireBullet(void)
{
    for (int i = 0; i < MAX_ENTITIES_SPAWNED; i++)
    {
        if (bullets[i].spawned == false)
        {
            bullets[i].spawned = true;
            bullets[i].position = player->position;
            bullets[i].positionDelta.x = PLAYER_BULLET_SPEED;
            bullets[i].health = 1;
            bullets[i].texture = bulletTexture;
            SDL_QueryTexture(bullets[i].texture, NULL, NULL, &bullets[i].size.x, &bullets[i].size.y);

            // Calculating the spawn position of the bullet with reference to the size of the texture.
            bullets[i].position.y += (player->size.y / 2) - (bullets[i].size.y / 2);
            player->reload = PLAYER_RELOAD_SPEED;
            break;
        }
    }
}

static void doBullets(void)
{
    for (int i = 0; i < MAX_ENTITIES_SPAWNED; i++)
    {
        if (bullets[i].spawned)
        {
            // Update bullet movement
            bullets[i].position.x += bullets[i].positionDelta.x;
            bullets[i].position.y += bullets[i].positionDelta.y;

            // If bullet is off screen, need to despawn it.
            if (bullets[i].position.x > SCREEN_WIDTH)
            {
                bullets[i].spawned = false;
            }
        }
    }
}

static void spawnEnemies()
{
    enemySpawnTimer--;
    if (enemySpawnTimer <= 0)
    {
        for (int i = PLAYER_INDEX + 1; i < MAX_ENTITIES_SPAWNED; i++)
        {
            if (!fighters[i].spawned)
            {
                fighters[i].spawned = true;
                fighters[i].position.x = SCREEN_WIDTH;
                fighters[i].position.y = rand() % SCREEN_HEIGHT;
                fighters[i].texture = enemyTexture;
                SDL_QueryTexture(fighters[i].texture, NULL, NULL, &fighters[i].size.x, &fighters[i].size.y);

                fighters[i].positionDelta.x = -(2 + (rand() % 4));
                enemySpawnTimer = 30 + (rand() % 60);
                break;
            }
        }
    }
}

static void draw(void)
{
    drawBullets();
    drawFighters();
}

static void drawFighters(void)
{
    for (int i = 0; i < MAX_ENTITIES_SPAWNED; i++)
    {
        if (fighters[i].spawned)
        {
            blit(fighters[i].texture, fighters[i].position.x, fighters[i].position.y);
        }
    }
}

static void drawBullets(void)
{
    for (int i = 0; i < MAX_ENTITIES_SPAWNED; i++)
    {
        if (bullets[i].spawned)
        {
            blit(bullets[i].texture, bullets[i].position.x, bullets[i].position.y);
        }
    }
}
