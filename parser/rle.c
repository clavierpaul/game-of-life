//
// Created by Paul Clavier on 03/07/2021.
//

#include <SDL_log.h>
#include "rle.h"

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern int yydebug;

static int x;
static int y;
static RLEGame* game;

void rle_add_tag(char* tag) {
    SDL_Log("%s", tag);
    free(tag);
}

void rle_set_width(int width) {
    SDL_Log("Width = %d", width);
}

void rle_set_height(int height) {
    SDL_Log("Height = %d", height);
}

void rle_set_rules(int born, int survive) {
    SDL_Log("Rules = B%d/S%d", born, survive);
}

void rle_add_cells(int cells) {
    SDL_Log("Adding %d cells", cells);
}

void rle_skip_cells(int cells) {
    SDL_Log("Skipping %d cells", cells);
}

void rle_next_line(int cells) {
    SDL_Log("Next line");
}

bool rle_parse(char* file) {
    x = 0;
    y = 0;

    YY_BUFFER_STATE buffer = yy_scan_string(file);
    if (yyparse() != 0) {
        SDL_Log("Error parsing RLE file");
    }
    yy_delete_buffer(buffer);
}