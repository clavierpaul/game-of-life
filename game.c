//
// Created by Paul Clavier on 01/07/2021.
//

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

typedef struct game {
    Hashmap* cells;
} Game;

static void game_get_neighbours(Game* game, int x, int y, Cell* neighbours[8], Cell positions[8]) {
    int i = 0;

    // Iterate over adjacent cells
    for (int j = x - 1; j <= x + 1; j++) {
        for (int k = y - 1; k <= y + 1; k++) {
            // Don't include ourself!
            if (j == x && k == y) continue;

            neighbours[i] = hashmap_get(game->cells, &(Cell){ .x = j, .y = k });

            // If a positions array is provided, write to it
            if (positions != NULL) {
                positions[i].x = j;
                positions[i].y = k;
            }

            i++;
        }
    }
}

// Needed for hashmap library, hashes the Cell struct
uint64_t game_cell_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const Cell* cell = item;
    return hashmap_sip(cell, sizeof *cell, seed0, seed1);
}

// Needed for hashmap library, compares Cells and returns 0 if equal, -1 otherwise
// Blame strncmp
int game_cell_compare(const void *a, const void *b, void *udata) {
    const Cell* ca = a;
    const Cell* cb = b;

    // Hashmap expects 0 on equal
    return ca->x == cb->x && ca->y == cb->y ? 0 : -1;
}

Game *game_create() {
    // Seed the hashmap
    // Clang-tidy is upset that this isn't random enough
    // Too bad!
    srand(time(NULL));

    Game* game = malloc(sizeof *game);
    game->cells = hashmap_new(sizeof(Cell), 0, rand(), rand(), game_cell_hash, game_cell_compare, 0);
    return game;
}

void game_free(Game* game) {
    hashmap_free(game->cells);
    free(game);
}

bool game_get_cell(Game* game, int x, int y) {
    Cell* cell = hashmap_get(game->cells, &(Cell){ .x = x, .y = y });
    return cell != NULL;
}

void game_clear(Game* game) {
    hashmap_clear(game->cells, true);
}

void game_toggle_cell(Game* game, int x, int y) {
    if (game_get_cell(game, x, y)) {
        hashmap_delete(game->cells, &(Cell){ .x = x, .y = y });
    } else {
        hashmap_set(game->cells, &(Cell){ .x = x, .y = y });
    }
}

typedef struct {
    Game* game;
    Hashmap* new_generation;
} IterData;

// Used by the hashmap library to iterate over each cell,
// and works out whether the cell lives or dies in the next generation
bool game_cell_tick_iter(const void *item, void *data) {
    // Load data
    const Cell* cell = item;
    Game* game = ((IterData*) data)->game;
    Hashmap* new_generation = ((IterData*) data)->new_generation;

    // Get all neighbours of the alive cell
    // Save the positions so we can check NULL (dead) cells
    Cell* neighbours[8];
    Cell positions[8];

    game_get_neighbours(game, cell->x, cell->y, neighbours, positions);
    int neighbour_count = 0;

    for (int i = 0; i < 8; i++) {
        // Get adjacent cell
        Cell* adjacent = neighbours[i];

        // If cell is dead, check its neighbours to see if it should be alive
        // This means dead cells are potentially checked multiple times but it's probably fine
        if (adjacent == NULL) {
            Cell* adjacent_neighbours[8];
            Cell position = positions[i];

            // If the cell was already added don't add it again
            if (hashmap_get(new_generation, &position) != NULL) continue;

            game_get_neighbours(game, position.x, position.y, adjacent_neighbours, NULL);
            int adjacent_count = 0;

            for (int a = 0; a < 8; a++) {
                if (adjacent_neighbours[a] != NULL) {
                    adjacent_count++;
                }
            }

            // If there are 3 alive cells adjacent to dead cell, add it to the list
            if (adjacent_count == 3) {
                hashmap_set(new_generation, &(Cell) { .x = position.x, .y = position.y });
            }
        } else {
            neighbour_count++;
        }
    }

    // If there are 2 or 3 neighbours, the cell survives to the next generation
    if (neighbour_count == 2 || neighbour_count == 3) {
        // Copy values into new generation
        hashmap_set(new_generation, &(Cell){ .x = cell->x, .y = cell->y });
    }

    return true;
}

void game_tick(Game* game) {
    // Create a new map for the new generation
    // This is needed for the adjacent cells to be calculated correctly
    Hashmap *new_generation = hashmap_new(sizeof(Cell), 0, rand(), rand(), game_cell_hash, game_cell_compare, 0);
    hashmap_scan(game->cells, game_cell_tick_iter, &(IterData){ .game = game, .new_generation = new_generation });
    hashmap_free(game->cells);
    game->cells = new_generation;
}