//
// Created by Paul Clavier on 01/07/2021.
//

#ifndef GAMEOFLIFE_GAME_H
#define GAMEOFLIFE_GAME_H

#include "stdbool.h"

typedef struct {
    int width;
    int height;
    bool* cells;
    bool* bufferA;
    bool* bufferB;
} Game;

Game *game_create(int width, int height);
void game_free(Game* game);

void game_tick(Game* game);
void game_clear(Game* game);
void game_toggle_cell(Game* game, int x, int y);
bool game_get_cell(Game* game, int x, int y);

#endif //GAMEOFLIFE_GAME_H
