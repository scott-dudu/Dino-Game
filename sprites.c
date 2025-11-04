#include <stdlib.h>
#include <time.h>
#include "sprites.h"

wchar_t *dino[SPRITE_HEIGHT] = {L"          ▄▄▄▄▄   ",  
                                L"         ██▄██████",
                                L"         ██████▄▄▄",
                                L"        ▄█████    ",
                                L"   █▄▄████████▀▀█ ",
                                L"    ▀▀██████▀     ",
                                L"      █▀  ▀█      ",
                                L"      █▄   █▄     "};

wchar_t *cactus[SPRITE_HEIGHT] = {L"               ",
                                  L"               ",
                                  L"               ",
                                  L"         ▄▄    ",
                                  L"        ████ ▄▄",
                                  L"     ██ ████▄█▀",
                                  L"      ▀█████▀  ",
                                  L"        ████   "};

wchar_t *cacti[SPRITE_HEIGHT] = {L"                                ",
                                 L"                                ",
                                 L"                                ",
                                 L"         ▄▄               ▄▄    ",
                                 L"        ████ ▄▄       ██ ████ ▄▄",
                                 L"     ██ ████▄█▀ ▄  ▄█▄ ▀█████▄█▀",
                                 L"      ▀█████▀  ███ ███   ████▀  ",
                                 L"        ████   ███ ███   ████   "};

wchar_t *bird[SPRITE_HEIGHT] = {L"         ▄█▄           ",  
                                L"       ▄██▄█▄ ▄▄██▀▀▀  ",
                                L"           ▀████████▀▀▀",
                                L"              ▀▀▀▀     ",
                                L"                       ",
                                L"                       ",
                                L"                       ",
                                L"                       "};

void set_rand_gen() {
    srand(time(NULL));
}

void get_dino(wchar_t** output) {
    for (int i = 0; i < SPRITE_HEIGHT; i++) {
        output[i] = dino[i];
    }
}

void get_obstacle(wchar_t** output){
    int choice = rand() % 3;

    wchar_t** obstacle;
    if (choice == 0) obstacle = cactus;
    else if (choice == 1) obstacle = cacti;
    else obstacle = bird;

    for (int i = 0; i < SPRITE_HEIGHT; i++) {
        output[i] = obstacle[i];
    }
}