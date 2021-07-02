#include <SDL.h>
#include "game.h"
#include "draw.h"

/*
void print_game(Game* game) {
    for (int y = 0; y < game->height; y ++) {
        for (int x = 0; x < game->width; x++) {
            bool cell = game_get_cell(game, x, y);
            char to_print = cell ? 'X' : '.';
            printf("%c", to_print);
        }

        printf("\n");
    }

    printf("\n");
}*/


SDL_Window* window;
SDL_Renderer* renderer;

int main() {
    if (!draw_initialize(&window, &renderer)) {
        return EXIT_FAILURE;
    }

    // Make game larger than window size in order to correctly handle off-screen affecting on screen
    Game* game = game_create(700, 350);
    bool quit = false;
    bool paused = true;

    u_int32_t time = SDL_GetTicks();
    draw_game(renderer, game);

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        paused = !paused;
                    } else if (event.key.keysym.sym == SDLK_c) {
                        game_clear(game);
                        draw_game(renderer, game);
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (paused) {
                        int x = event.motion.x / CELL_SIZE;
                        int y = event.motion.y / CELL_SIZE;

                        SDL_Log("(%d, %d)", x + 300, y + 150);
                        game_toggle_cell(game, x + 300, y + 150);
                        draw_game(renderer, game);
                    }
                    break;
                default:
                    break;
            }
        }

        if (SDL_GetTicks() - time > 25 && !paused) {
            game_tick(game);
            draw_game(renderer, game);
            time = SDL_GetTicks();
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}