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
//extern int yy_flex_debug;

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

void rle_parse(char* file) {
    //yydebug = 1;
    YY_BUFFER_STATE buffer = yy_scan_string(file);
    yyparse();
    yy_delete_buffer(buffer);
}