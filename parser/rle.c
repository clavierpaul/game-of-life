//
// Created by Paul Clavier on 03/07/2021.
//

#include <SDL_log.h>
#include "rle.h"

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

static int x;
static int y;
static RLEGame* game;
bool error; // Used by set rules since non-standard rules aren't implemented

void rle_add_tag(char* tag) {
    // Only tag we care about is the name tag
    if (tag[1] == 'N' || tag[1] == 'n') {
        // Name starts at char 4
        char* name_ptr = &tag[3];
        game->name = strdup(name_ptr);
    }

    // Free tag when we're done with it
    free(tag);
}

void rle_set_width(int width) {
    game->width = width;
}

void rle_set_height(int height) {
    game->height = height;
}

void rle_set_rules(int born, int survive) {
    if (born != 3 || survive != 23) {
        error = true;
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Non-standard rules are unsupported");
    }
}

void rle_add_cells(int cells) {
    for (int i = 0; i < cells; i++) {
        game_toggle_cell(game->game, x, y);
        x++;
    }
}

void rle_skip_cells(int cells) {
    x += cells;
}

void rle_next_line(int lines) {
    x = 1;
    y += lines;
}

void rle_free_game(RLEGame* rle_game) {
    free(rle_game->name);
    game_free(rle_game->game);
    free(rle_game);
}

RLEGame* rle_parse(char* file) {
    // Pad the left side
    x = 1;
    y = 1;

    game = malloc(sizeof *game);
    game->game = game_create();

    YY_BUFFER_STATE buffer = yy_scan_string(file);

    if (yyparse() != 0 || error) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error parsing RLE file");
        return NULL;
    }

    yy_delete_buffer(buffer);

    return game;
}