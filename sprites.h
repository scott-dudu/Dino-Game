#ifndef sprites_h
#include <wchar.h>

#define SPRITE_HEIGHT 10

typedef enum {
    STAND,
    STAND_LEFT,
    STAND_RIGHT,
    DUCK_LEFT,
    DUCK_RIGHT
} Stance;

void set_rand_gen();

void get_dino(wchar_t** output, Stance stance);

void get_obstacle(wchar_t** output);

#endif