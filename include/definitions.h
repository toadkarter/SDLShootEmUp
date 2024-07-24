#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#ifdef _WIN32
    #define RESOURCES_PATH "../../resources/"
#elif defined(__linux__)
    #define RESOURCES_PATH "../resources/"
#endif

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define TARGET_FPS 60

#define PLAYER_INDEX 0
#define MAX_ENTITIES_SPAWNED 128

#define PLAYER_SPEED 4
#define PLAYER_BULLET_SPEED 16
#define PLAYER_RELOAD_SPEED 8

#define ALIEN_BULLET_SPEED 8

#define MAX_KEYBOARD_KEYS 350

#define SIDE_PLAYER 0
#define SIDE_ALIEN 1

#endif
