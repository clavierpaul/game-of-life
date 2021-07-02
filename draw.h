//
// Created by Paul Clavier on 02/07/2021.
//

#ifndef GAMEOFLIFE_DRAW_H
#define GAMEOFLIFE_DRAW_H

#include <SDL.h>
#include <stdbool.h>
#include "game.h"

// Grid constants
static const int CELL_SIZE = 12;

// Color constants
static const SDL_Color BACKGROUND_COLOR = {255, 255, 255};
static const SDL_Color GRID_COLOR = {220, 220, 220};
static const SDL_Color CELL_COLOR = {28, 179, 255};

/// Initializes SDL2 and creates a window and renderer for the application
/// \param window Application window
/// \param renderer Application renderer
/// \param width Grid width
/// \param height Grid height
/// \return true on success, false on error
bool draw_initialize(SDL_Window** window, SDL_Renderer** renderer, int width, int height);

/// Draws the current game state
/// \param renderer Application renderer
/// \param game Current game
/// \param width Grid width
/// \param height Grid height
void draw_game(SDL_Renderer* renderer, Game* game, int width, int height);

#endif //GAMEOFLIFE_DRAW_H
