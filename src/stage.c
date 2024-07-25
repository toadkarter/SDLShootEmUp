#ifdef _WIN32
    #include <SDL.h>
#elif defined(__linux__)
    #include <SDL2/SDL.h>
#endif

#include "stage.h"
#include "utils.h"
#include "types.h"
#include "global.h"
#include "draw.h"
#include "definitions.h"

static void logic(void);
static void draw(void);
static void initPlayer(void);

static void doPlayer(void);
static void doAllFighters(void);
static void attemptEnemiesAttack(void);

static void doBullets(void);

static void keepPlayerInBounds(void);

static void drawAllFighters(void);
static void drawBullets(void);

static void spawnEnemies(void);
static void spawnPlayerBullet(void);
static void fireAlienBullet(Entity* enemy);

static void checkCollisions(void);
static void clearAllEntities(Entity entities[MAX_ENTITIES_SPAWNED]);
static bool entityIsOffScreen(Entity* entity);

static Entity* player;

Entity fighters[MAX_ENTITIES_SPAWNED] = {0};
Entity bullets[MAX_ENTITIES_SPAWNED] = {0};

// Precaching these so we don't have to grab this every time a bullet is spawned.
static SDL_Texture* bulletTexture;
static SDL_Texture* enemyTexture;
static SDL_Texture* alienbulletTexture;

static int enemySpawnTimer;
static int stageResetTimer;

void initStage(void)
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    initPlayer();

    bulletTexture = loadTextureFromFileName("bullet.png");
    enemyTexture = loadTextureFromFileName("enemy.png");
    alienbulletTexture = loadTextureFromFileName("alienbullet.png");

    resetStage();
}

inline void resetStage(void)
{
    clearAllEntities(fighters);
    clearAllEntities(bullets);

    initPlayer();
    stageResetTimer = TARGET_FPS * 2;
}

static void initPlayer(void)
{
    fighters[PLAYER_INDEX].spawned = true;
    fighters[PLAYER_INDEX].side = SIDE_PLAYER;
    fighters[PLAYER_INDEX].health = 1;
    player = &fighters[PLAYER_INDEX];

    SDL_Point playerSpawnPosition = {100, 100};
    player->position = playerSpawnPosition;
    player->texture = loadTextureFromFileName("spaceship.png");
    SDL_QueryTexture(player->texture, NULL, NULL, &player->size.x, &player->size.y);
}

static void logic(void)
{
    doPlayer();
    doAllFighters();
    attemptEnemiesAttack();

    doBullets();
    spawnEnemies();

    checkCollisions();
    keepPlayerInBounds();

    if (player->spawned == false)
    {
        stageResetTimer--;
        if (stageResetTimer <= 0)
        {
            resetStage();
        }
    }
}

static void doPlayer(void)
{
    if (player->spawned == false)
    {
        return;
    }

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
        spawnPlayerBullet();
    }
}

static void doAllFighters(void)
{
    for (int i = 0; i < MAX_ENTITIES_SPAWNED; i++)
    {
        Entity* currentFighter = &fighters[i];
        if (currentFighter->spawned)
        {
            currentFighter->position.x += currentFighter->positionDelta.x;
            currentFighter->position.y += currentFighter->positionDelta.y;

            if (i != PLAYER_INDEX && (currentFighter->position.x < -currentFighter->size.x || currentFighter->health == 0))
            {
                currentFighter->health = 0;
            }

            if (currentFighter->health == 0)
            {
                currentFighter->spawned = false;
            }
        }
    }
}

void attemptEnemiesAttack(void)
{
    for (int i = PLAYER_INDEX + 1; i < MAX_ENTITIES_SPAWNED; i++)
    {
        Entity* currentEnemy = &fighters[i];
        if (currentEnemy->spawned)
        {
            currentEnemy->reload--;
            if (currentEnemy->reload <= 0)
            {
                fireAlienBullet(currentEnemy);
            }
        }
    }
}

static void spawnPlayerBullet(void)
{
    for (int i = 0; i < MAX_ENTITIES_SPAWNED; i++)
    {
        Entity* currentPlayerBullet = &bullets[i];
        if (currentPlayerBullet->spawned == false)
        {
            currentPlayerBullet->spawned = true;
            currentPlayerBullet->side = SIDE_PLAYER;
            currentPlayerBullet->position = player->position;
            currentPlayerBullet->positionDelta.x = PLAYER_BULLET_SPEED;

            // We could be reusing an alien bullet - make sure to reset Y to 0.
            currentPlayerBullet->positionDelta.y = 0;

            currentPlayerBullet->health = 1;
            currentPlayerBullet->texture = bulletTexture;
            SDL_QueryTexture(currentPlayerBullet->texture, NULL, NULL, &currentPlayerBullet->size.x, &currentPlayerBullet->size.y);

            // Calculating the spawn position of the bullet with reference to the size of the texture.
            currentPlayerBullet->position.y += (currentPlayerBullet->size.y / 2) - (currentPlayerBullet->size.y / 2);
            player->reload = PLAYER_RELOAD_SPEED;
            break;
        }
    }
}

static void fireAlienBullet(Entity* enemy)
{
    for (int i = 0; i < MAX_ENTITIES_SPAWNED; i++)
    {
        Entity* currentAlienBullet = &bullets[i];
        if (currentAlienBullet->spawned == false)
        {
            currentAlienBullet->spawned = true;
            currentAlienBullet->position.x = enemy->position.x;
            currentAlienBullet->position.y = enemy->position.y;

            currentAlienBullet->health = 1;
            currentAlienBullet->texture = alienbulletTexture;
            currentAlienBullet->side = SIDE_ALIEN;
            SDL_QueryTexture(currentAlienBullet->texture, NULL, NULL, &currentAlienBullet->size.x, &currentAlienBullet->size.y);

            currentAlienBullet->position.x += (enemy->size.x / 2) - (enemy->size.x / 2);
            currentAlienBullet->position.y += (enemy->size.y / 2) - (enemy->size.y / 2);

            SDL_Point playerCenter;
            playerCenter.x = player->position.x + (player->size.x / 2);
            playerCenter.y = player->position.y + (player->size.y / 2);

            SDL_Point enemyCenter;
            enemyCenter.x = enemy->position.x + (enemy->size.x / 2);
            enemyCenter.y = enemy->position.y + (enemy->size.y / 2);

            calculateDirection(playerCenter, enemyCenter, &currentAlienBullet->positionDelta);

            currentAlienBullet->positionDelta.x *= ALIEN_BULLET_SPEED;
            currentAlienBullet->positionDelta.y *= ALIEN_BULLET_SPEED;

            enemy->reload = (rand() % TARGET_FPS * 10);
            break;
        }
    }
}

static void doBullets(void)
{
    for (int i = 0; i < MAX_ENTITIES_SPAWNED; i++)
    {
        Entity* currentBullet = &bullets[i];
        if (currentBullet->spawned)
        {
            // Update bullet movement
            currentBullet->position.x += currentBullet->positionDelta.x;
            currentBullet->position.y += currentBullet->positionDelta.y;

            // If bullet is off screen, need to despawn it.
            if (entityIsOffScreen(currentBullet) || currentBullet->health == 0)
            {
                currentBullet->spawned = false;
            }
        }
    }
}

static void keepPlayerInBounds(void)
{
    if (player->spawned == false)
    {
        return;
    }

    if (player->position.x < 0)
    {
        player->position.x = 0;
    }

    if (player->position.y < 0)
    {
        player->position.y = 0;
    }

    if (player->position.x > SCREEN_WIDTH / 2)
    {
        player->position.x = SCREEN_WIDTH / 2;
    }

    if (player->position.y > SCREEN_HEIGHT - player->size.y)
    {
        player->position.y = SCREEN_HEIGHT - player->size.y;
    }
}

static void spawnEnemies(void)
{
    enemySpawnTimer--;
    if (enemySpawnTimer <= 0)
    {
        for (int i = PLAYER_INDEX + 1; i < MAX_ENTITIES_SPAWNED; i++)
        {
            if (fighters[i].spawned == false)
            {
                fighters[i].spawned = true;
                fighters[i].side = SIDE_ALIEN;
                fighters[i].health = 1;
                fighters[i].reload = TARGET_FPS * (1 + (rand() % 3));

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
    drawAllFighters();
}

static void drawAllFighters(void)
{
    for (int i = 0; i < MAX_ENTITIES_SPAWNED; i++)
    {
        Entity* currentFighter = &fighters[i];
        if (currentFighter->spawned)
        {
            blit(currentFighter->texture, currentFighter->position.x, currentFighter->position.y);
        }
    }
}

static void drawBullets(void)
{
    for (int i = 0; i < MAX_ENTITIES_SPAWNED; i++)
    {
        Entity* currentBullet = &bullets[i];
        if (currentBullet->spawned)
        {
            blit(currentBullet->texture, currentBullet->position.x, currentBullet->position.y);
        }
    }
}

static void checkCollisions(void)
{
    for (int i = 0; i < MAX_ENTITIES_SPAWNED; i++)
    {
        Entity* currentBullet = &bullets[i];
        for (int j = 0; j < MAX_ENTITIES_SPAWNED; j++)
        {
            Entity* currentFighter = &fighters[j];
            if (currentBullet->spawned &&
                currentFighter->spawned &&
                isCollided(currentBullet, currentFighter))
            {
                currentBullet->health = 0;
                currentFighter->health = 0;
            }
        }
    }
}

static void clearAllEntities(Entity entities[MAX_ENTITIES_SPAWNED])
{
    for (int i = 0; i < MAX_ENTITIES_SPAWNED; i++)
    {
        entities[i].spawned = false;
    }
}

static bool entityIsOffScreen(Entity* entity)
{
    return entity->position.x < -entity->size.x ||
           entity->position.y < -entity->size.y ||
           entity->position.x > SCREEN_WIDTH ||
           entity->position.y > SCREEN_HEIGHT;
}
