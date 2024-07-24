#ifdef _WIN32
    #include <SDL.h>
#elif defined(__linux__)
    #include <SDL2/SDL.h>
#endif

#include "stage.h"
#include "types.h"
#include "global.h"
#include "draw.h"

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

static Stage stage;
static Entity* player;

// Precaching these so we don't have to grab this every time a bullet is spawned.
static SDL_Texture* bulletTexture;
static SDL_Texture* enemyTexture;

static int enemySpawnTimer;

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
    enemyTexture = loadTexture("../resources/enemy.png");

    enemySpawnTimer = 0;
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
    Entity* currentEnemy = NULL;
    Entity* previousEnemy = &stage.fighterHead;

    for (currentEnemy = stage.fighterHead.nextEntity; currentEnemy != NULL; currentEnemy = currentEnemy->nextEntity)
    {
        currentEnemy->position.x += currentEnemy->positionDelta.x;
        currentEnemy->position.y += currentEnemy->positionDelta.y;

        if (currentEnemy != player && currentEnemy->position.x < -currentEnemy->size.x)
        {
            if (currentEnemy == stage.fighterTail)
            {
                stage.fighterTail = previousEnemy;
            }

            previousEnemy->nextEntity = currentEnemy->nextEntity;
            free(currentEnemy);
            currentEnemy = previousEnemy;
        }

        previousEnemy = currentEnemy;
    }
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

static void spawnEnemies()
{
    Entity* enemy = NULL;

    enemySpawnTimer--;
    if (enemySpawnTimer <= 0)
    {
        enemy = malloc(sizeof(Entity));
        memset(enemy, 0, sizeof(Entity));
        stage.fighterTail->nextEntity = enemy;
        stage.fighterTail = enemy;

        enemy->position.x = SCREEN_WIDTH;
        enemy->position.y = rand() % SCREEN_HEIGHT;
        enemy->texture = enemyTexture;
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->size.x, &enemy->size.y);

        // Random number between -2 and -5
        enemy->positionDelta.x = -(2 + (rand() % 4));

        enemySpawnTimer = 30 + (rand() % 60);
    }
}

static void draw(void)
{
    drawBullets();
    drawFighters();
}

static void drawFighters(void)
{
    Entity* currentEnemy;
    for (currentEnemy = stage.fighterHead.nextEntity; currentEnemy != NULL; currentEnemy = currentEnemy->nextEntity)
    {
        blit(currentEnemy->texture, currentEnemy->position.x, currentEnemy->position.y);
    }
}

static void drawBullets(void)
{
    Entity* currentBullet = NULL;

    for (currentBullet = stage.bulletHead.nextEntity; currentBullet != NULL; currentBullet = currentBullet->nextEntity)
    {
        blit(currentBullet->texture, currentBullet->position.x, currentBullet->position.y);
    }
}
