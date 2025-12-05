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

wchar_t *dino_stand_left[SPRITE_HEIGHT] = {L"                  ",
                                           L"                  ",
                                           L"          ▄▄▄▄▄   ",  
                                           L"         ██▄██████",
                                           L"         ██████▄▄▄",
                                           L"        ▄█████    ",
                                           L"   █▄▄████████▀▀█ ",
                                           L"    ▀▀██████▀     ",
                                           L"      █▀   █▄     ",
                                           L"      █▄          "};

wchar_t *dino_stand_right[SPRITE_HEIGHT] = {L"                  ",
                                            L"                  ",
                                            L"          ▄▄▄▄▄   ",  
                                            L"         ██▄██████",
                                            L"         ██████▄▄▄",
                                            L"        ▄█████    ",
                                            L"   █▄▄████████▀▀█ ",
                                            L"    ▀▀██████▀     ",
                                            L"      █▄  ▀█      ",
                                            L"           █▄     "};

wchar_t *dino_duck_left[SPRITE_HEIGHT] = {L"                          ",
                                          L"                          ",
                                          L"                          ",  
                                          L"                          ",
                                          L"                          ",
                                          L"       ▄▄▄███▄▄  ▄█▀██▄▄▄▄",
                                          L"   █▄▄████████████████▀▀▀▀",
                                          L"    ▀▀███████████▀▀▀▀▀▀▀▀▀",
                                          L"      █▀   █▄  ▄█         ",
                                          L"      █▄                  "};

wchar_t *dino_duck_right[SPRITE_HEIGHT] = {L"                          ",
                                          L"                          ",
                                          L"                          ",  
                                          L"                          ",
                                          L"                          ",
                                          L"       ▄▄▄███▄▄  ▄█▀██▄▄▄▄",
                                          L"   █▄▄████████████████▀▀▀▀",
                                          L"    ▀▀███████████▀▀▀▀▀▀▀▀▀",
                                          L"      █▄  ▀█   ▄█         ",
                                          L"           █▄             "};

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
    wchar_t **dino;
    switch (stance) {
        case STAND:
            dino = dino_stand;
            break;
        case STAND_LEFT:
            dino = dino_stand_left;
            break;
        case STAND_RIGHT:
            dino = dino_stand_right;
            break;
        case DUCK_LEFT:
            dino = dino_duck_left;
            break;
        case DUCK_RIGHT:
            dino = dino_duck_right;
            break;
        default:
            abort();
    }

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