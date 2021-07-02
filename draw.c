//
// Created by Paul Clavier on 02/07/2021.
//

#include "draw.h"

bool draw_initialize(SDL_Window** window, SDL_Renderer** renderer, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing SDL: %s", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               width * CELL_SIZE,height * CELL_SIZE, SDL_WINDOW_RESIZABLE);
    if (*window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating window: %s", SDL_GetError());
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating renderer: %s", SDL_GetError());
        return false;
    }

    return true;
}

void draw_game(SDL_Renderer* renderer, Game* game, int width, int height) {
    // Draw background
    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, 255);
    SDL_RenderClear(renderer);

    // Draw cells
    SDL_SetRenderDrawColor(renderer, CELL_COLOR.r, CELL_COLOR.g, CELL_COLOR.b, 255);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (game_get_cell(game, x, y)) {
                SDL_Rect cell_rect = {
                        .x = x * CELL_SIZE,
                        .y = y * CELL_SIZE,
                        .w = CELL_SIZE + 1,
                        .h = CELL_SIZE + 1
                };

                SDL_RenderFillRect(renderer, &cell_rect);
            }
        }
    }

    // Draw vertical grid lines
    SDL_SetRenderDrawColor(renderer, GRID_COLOR.r, GRID_COLOR.g, GRID_COLOR.b, 255);

    for (int x = CELL_SIZE; x < width * CELL_SIZE; x += CELL_SIZE) {
        SDL_RenderDrawLine(renderer, x, 0, x, height * CELL_SIZE);
    }

    // Draw horizontal grid lines
    for (int y = CELL_SIZE; y < height * CELL_SIZE; y += CELL_SIZE) {
        SDL_RenderDrawLine(renderer, 0, y, width * CELL_SIZE, y);
    }

    SDL_RenderPresent(renderer);
}