//
// Created by Paul Clavier on 03/07/2021.
//

#include "game.h"

#ifndef GAMEOFLIFE_RLE_H
#define GAMEOFLIFE_RLE_H

typedef struct {
    char* name; // Reserved for future implementation
    int width;
    int height;
    Game* game;
} RLEGame;

bool rle_parse(RLEGame* rle_game, const char* rle_file);

#endif //GAMEOFLIFE_RLE_H
