//
// Created by Paul Clavier on 01/07/2021.
//

#include <string.h>
#include <stdlib.h>
#include "game.h"

static inline int get_total_cells(Game* game) {
    return game->height * game->width;
}

static inline int get_cell_index(Game* game, int x, int y) {
    return y * game->width + x;
}

static bool is_out_of_bounds(Game* game, int x, int y) {
    return x < 0 || x >= game->width || y < 0 || y >= game->height;
}

static int get_neighbour_count(Game* game, int x, int y) {
    int count = 0;

    for (int j = x - 1; j <= x + 1; j++) {
        for (int k = y - 1; k <= y + 1; k++) {
            if ((j == x && k == y) || is_out_of_bounds(game, j, k)) continue;

            if (game_get_cell(game, j, k)) {
                count++;
            }
        }
    }

    return count;
}

Game *game_create(int width, int height) {
    Game* game = malloc(sizeof *game);
    game->height = height;
    game->width = width;
    game->bufferA = calloc(height * width, sizeof(bool));
    game->bufferB = calloc(height * width, sizeof(bool));
    game->cells = game->bufferA;
    return game;
}

void game_free(Game* game) {
    free(game->bufferA);
    free(game->bufferB);
    free(game);
}

bool game_get_cell(Game* game, int x, int y) {
    return game->cells[get_cell_index(game, x, y)];
}

void game_clear(Game* game) {
    int total_cells = get_total_cells(game);
    memset(game->bufferA, false, total_cells);
    memset(game->bufferB, false, total_cells);
    game->cells = game->bufferA;
}

void game_toggle_cell(Game* game, int x, int y) {
    int c = get_cell_index(game, x, y);
    game->cells[c] = !game->cells[c];
}

void game_tick(Game* game) {
    bool* new_generation = game->cells == game->bufferA ? game->bufferB : game->bufferA;

    for (int x = 0; x < game->width; x++) {
        for (int y = 0; y < game->height; y++) {
            int neighbours = get_neighbour_count(game, x, y);
            bool cell = game_get_cell(game, x, y);

            if (neighbours == 3 && !cell) {
                new_generation[get_cell_index(game, x, y)] = true; // A new cell is born
            } else if (cell && (neighbours < 2 || neighbours > 3)) {
                new_generation[get_cell_index(game, x, y)] = false; // The cell dies
            } else {
                new_generation[get_cell_index(game, x, y)] = game->cells[get_cell_index(game, x, y)];
            }
        }
    }

    game->cells = new_generation;
}