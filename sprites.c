#include <stdlib.h>
#include <time.h>
#include "sprites.h"

wchar_t *dino_stand[SPRITE_HEIGHT] = {L"                  ",
                                      L"                  ",
                                      L"          ▄▄▄▄▄   ",  
                                      L"         ██▄██████",
                                      L"         ██████▄▄▄",
                                      L"        ▄█████    ",
                                      L"   █▄▄████████▀▀█ ",
                                      L"    ▀▀██████▀     ",
                                      L"      █▀  ▀█      ",
                                      L"      █▄   █▄     "};

wchar_t *dino_duck[SPRITE_HEIGHT] = {L"                          ",
                                     L"                          ",
                                     L"                          ",  
                                     L"                          ",
                                     L"                          ",
                                     L"       ▄▄▄███▄▄  ▄█▀██▄▄▄▄",
                                     L"   █▄▄████████████████▀▀▀▀",
                                     L"    ▀▀███████████▀▀▀▀▀▀▀▀▀",
                                     L"      █▀  ▀█  ▄█          ",
                                     L"      █▄   █▄             "};

wchar_t *cactus[SPRITE_HEIGHT] = {L"               ",
                                  L"               ",
                                  L"               ",
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
                                L"                       ",
                                L"                       ",
                                L"                       ",};

void set_rand_gen() {
    srand(time(NULL));
}

void get_dino(wchar_t** output, Stance stance) {
    if (stance == STAND) {
        for (int i = 0; i < SPRITE_HEIGHT; i++) {
            output[i] = dino_stand[i];
        }
    } else if (stance == DUCK) {
        for (int i = 0; i < SPRITE_HEIGHT; i++) {
            output[i] = dino_duck[i];
        }
    } else {
        abort();
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