//
// Created by Paul Clavier on 01/07/2021.
//

#ifndef GAMEOFLIFE_GAME_H
#define GAMEOFLIFE_GAME_H

#include "stdbool.h"
#include "hashmap/hashmap.h"

typedef struct hashmap Hashmap;

typedef struct {
    int x;
    int y;
} Cell;

typedef struct game Game;

/// Creates a new game
/// \return A new Game struct
Game* game_create();

/// Deletes all buffers associated with a game and then deletes the game
/// \param game Game to be deleted
void game_free(Game* game);

/// Performs one generation
/// \param game Game to tick on
void game_tick(Game* game);

/// Clears all cells in a game
/// \param game Game to be cleared
void game_clear(Game* game);

/// Toggles a cell in a game
/// \param game Game to modify
/// \param x x-coordinate of the cell
/// \param y y-coordinate of the cell
void game_toggle_cell(Game* game, int x, int y);

/// Get the value of a cell
/// \param game Game to view
/// \param x x-coordinate of the cell
/// \param y y-coordinate of the cell
/// \return The value of the cell at (x, y)
bool game_get_cell(Game* game, int x, int y);

#endif //GAMEOFLIFE_GAME_H
