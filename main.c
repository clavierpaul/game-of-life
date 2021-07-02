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

typedef enum { add, delete } ClickAction;

int main() {
    if (!draw_initialize(&window, &renderer, 60, 50)) {
        return EXIT_FAILURE;
    }

    Game* game = game_create();

    bool quit = false;
    bool paused = true;
    bool paused_before_click;
    bool mouse_down = false;
    bool should_draw;
    ClickAction click_action = add;

    u_int32_t time = SDL_GetTicks();
    draw_game(renderer, game, 60, 50);

    while (!quit) {
        should_draw = !paused;

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
                        should_draw = true;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    {
                        paused_before_click = paused;
                        paused = true;
                        mouse_down = true;

                        int x = event.motion.x / CELL_SIZE;
                        int y = event.motion.y / CELL_SIZE;

                        // Set click action for further cells that are dragged onto
                        bool cell_exists = game_get_cell(game, x, y);
                        click_action = cell_exists ? delete : add;
                        game_toggle_cell(game, x, y);
                        should_draw = true;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (mouse_down) {
                        int x = event.motion.x / CELL_SIZE;
                        int y = event.motion.y / CELL_SIZE;

                        bool cell_exists = game_get_cell(game, x, y);

                        if ((cell_exists && click_action == delete) || (!cell_exists && click_action == add)) {
                            game_toggle_cell(game, x, y);
                            should_draw = true;
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    paused = paused_before_click;
                    mouse_down = false;
                    break;
                default:
                    break;
            }
        }

        if (SDL_GetTicks() - time > 25 && !paused) {
            game_tick(game);
            time = SDL_GetTicks();
        }

        if (should_draw) {
            draw_game(renderer, game, 60, 50);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    game_free(game);

    return EXIT_SUCCESS;
}