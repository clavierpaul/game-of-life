#include <SDL.h>
#include "game.h"
#include "draw.h"

typedef enum { add, delete } ClickAction;

// Global vars for the program
// Cry about it
Game* game;

SDL_Window* window;
SDL_Renderer* renderer;
int width = 70;
int height = 50;

bool quit = false;
bool paused = true;
bool paused_before_click;
bool mouse_down = false;
bool should_draw;
ClickAction click_action;

void do_resize(SDL_Keycode key) {
    switch (key) {
        case SDLK_UP:
            if (height != 20) {
                height -= 1;
            }
            break;
        case SDLK_RIGHT:
            width += 1;
            break;
        case SDLK_DOWN:
            height += 1;
            break;
        case SDLK_LEFT:
            if (width != 20) {
                width -= 1;
            }
            break;
        default:
            break;
    }

    SDL_SetWindowSize(window, width * CELL_SIZE, height * CELL_SIZE);
}

void do_event(SDL_Event event) {
    switch (event.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_SPACE:
                    paused = !paused;
                    break;
                case SDLK_c:
                    game_clear(game);
                    should_draw = true;
                    break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN: {
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
            break;
        }
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

// Allows for drawing while resizing
int filter_event(void *data, SDL_Event* event) {
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
        int set_width = event->window.data1;
        int set_height = event->window.data2;

        // Set the new grid width
        // Add one cell for cells that clip off-screen
        width = set_width / CELL_SIZE + 1;
        height = set_height / CELL_SIZE + 1;
        draw_game(renderer, game, width, height);

        return 0;
    }

    return 1;
}

int main() {
    if (!draw_initialize(&window, &renderer, width, height)) {
        return EXIT_FAILURE;
    }

    game = game_create();

    u_int32_t time = SDL_GetTicks();
    draw_game(renderer, game, width, height);

    SDL_SetEventFilter(filter_event, NULL);

    while (!quit) {
        should_draw = !paused;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            do_event(event);
        }

        if (SDL_GetTicks() - time > 25 && !paused) {
            game_tick(game);
            time = SDL_GetTicks();
        }

        if (should_draw) {
            draw_game(renderer, game, width, height);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    game_free(game);

    return EXIT_SUCCESS;
}