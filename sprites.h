#ifndef sprites_h
#include <wchar.h>

#define SPRITE_HEIGHT 10

typedef enum {
    STAND,
    DUCK
} Stance;

void set_rand_gen();

void get_dino(wchar_t** output, Stance stance);

void get_obstacle(wchar_t** output);

#endif