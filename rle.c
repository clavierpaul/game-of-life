//
// Created by Paul Clavier on 03/07/2021.
//

#include <string.h>
#include <SDL_log.h>
#include "rle.h"

typedef enum {
    state_0,
    state_rule,
    state_number,
} ReadState;

typedef enum {
    width,
    height,
    rule,
    comma,
    equals,
    number,
    rule_born,
    rule_survive,
    slash,
    cell_alive,
    cell_dead,
    line_end,
    newline,
    eof
} Symbol;

typedef struct token {
    Symbol symbol;
    char value[16];
} Token;

static Token rle_lexer(const char** rle) {
    // Zero out memory in-between tokens
    Token token;
    ReadState state = state_0;
    int ti = 0;
    char* rule_ident = "rule";

    // TODO: Clean up this mess
    while (**rle != '!' && **rle != '\0') {
        switch(state) {
            case state_0:
                if (**rle >= '0' && **rle <= '9') {
                    memset(&token.value, 0, sizeof(char) * 16);
                    state = state_number;
                    token.symbol = number;
                    token.value[0] = **rle;
                    ti++;
                    (*rle)++;
                    break;
                }

                switch(**rle) {
                    // Technically single char tokens are state transitions but who cares
                    case 'x':
                        token.symbol = width;
                        (*rle)++;
                        return token;
                    case 'y':
                        token.symbol = height;
                        (*rle)++;
                        return token;
                    case 'r':
                        state = state_rule;
                        (*rle)++;
                        ti++;
                        break;
                    case ',':
                        token.symbol = comma;
                        (*rle)++;
                        return token;
                    case '=':
                        token.symbol = equals;
                        (*rle)++;
                        return token;
                    case 'B':
                        token.symbol = rule_born;
                        (*rle)++;
                        return token;
                    case 'S':
                        token.symbol = rule_survive;
                        (*rle)++;
                        return token;
                    case '/':
                        token.symbol = slash;
                        (*rle)++;
                        return token;
                    case 'b':
                        token.symbol = cell_dead;
                        (*rle)++;
                        return token;
                    case 'o':
                        token.symbol = cell_alive;
                        (*rle)++;
                        return token;
                    case '$':
                        token.symbol = line_end;
                        (*rle)++;
                        return token;
                    case '\n':
                        token.symbol = newline;
                        (*rle)++;
                        return token;
                    default:
                        (*rle)++;
                        break;
                }

                break;
            case state_rule:
                if (**rle == rule_ident[ti]) {
                    (*rle)++;

                    if (ti == 3) {
                        token.symbol = rule;
                        return token;
                    }

                    ti++;
                } else {
                    // Go back to character after 'r'
                    *rle -= ti - 1;
                    state = state_0;
                }
                break;
            case state_number:
                // TODO: Use malloc to allow arbitrary length
                if (ti == 16) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not lex number longer than 16 digits");
                    exit(0);
                }

                if (**rle >= '0' && **rle <= '9') {
                    token.value[ti] = **rle;
                    ti++;
                    (*rle)++;
                } else {
                    // End of parsing, return char
                    return token;
                }
                break;
        }
    }

    token.symbol = eof;
    return token;
}

static Token token;
static RLEGame* game;
static const char* file;

bool accept(Symbol symbol) {
    if (token.symbol == symbol) {
        token = rle_lexer(&file);
        return true;
    }

    return false;
}

bool expect(Symbol symbol) {
    if (accept(symbol)) {
        return true;
    }

    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Parse error in RLE file");
    return false;
}

static bool header_setting() {
    Symbol setting = token.symbol;
    char* value;

    if (accept(width) || accept(height) || accept(rule)) {
        if (!expect(equals)) return false;
        value = token.value;
        switch(setting) {
            case width:
                if (!expect(number)) return false;
                game->width = atoi(value);
                break;
            case height:
                if (!expect(number)) return false;
                game->height = atoi(value);
                break;
            case rule:
                if (!expect(rule_born)) return false;
                value = token.value;
                if (!expect(number)) return false;
                if (strcmp(value, "3") != 0) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Non-standard rules are not supported");
                    return false;
                }
                if (!expect(slash) || !expect(rule_survive)) return false;
                value = token.value;
                if (!expect(number)) return false;
                if (strcmp(value, "23") != 0) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Non-standard rules are not supported");
                    return false;
                }
        }
        return true;
    }

    return false;
}

static bool header() {
    while (header_setting()) {
        if (accept(comma)) continue;
        if (!expect(newline)) return false;
    }

    return true;
}

static bool cell(int* x, int y) {
    int length = atoi(token.value);

    if (!accept(number)) {
        length = 1;
    }

    if (accept(cell_alive)) {
        for (int i = 0; i < length; i++) {
            game_toggle_cell(game->game, *x, y);
            (*x)++;
        }
    } else if (accept(cell_dead)) {
        *x = *x + length;
    } else {
        return false;
    }

    return true;
}

static bool body() {
    // Padding between file and border
    int x = 1;
    int y = 1;

    while (!accept(eof)) {
        if (accept(line_end)) {
            x = 1;
            y += 1;
            continue;
        }

        if (cell(&x, y)) continue;
        if (!expect(newline)) return false;
    }

    return true;
}

bool rle_parse(RLEGame* rle_game, const char* rle_file) {
    file = rle_file;
    game = rle_game;
    token.symbol = 0;

    game->game = game_create();

    token = rle_lexer(&file);
    header();
    body();

    /*
    while (token.symbol != eof) {
        token = rle_lexer(&rle_file);

        switch (token.symbol) {
            case width:
                symbol_name = "WIDTH";
                break;
            case height:
                symbol_name = "HEIGHT";
                break;
            case rule:
                symbol_name = "RULE";
                break;
            case comma:
                symbol_name = "COMMA";
                break;
            case equals:
                symbol_name = "EQUALS";
                break;
            case number:
                symbol_name = "NUMBER";
                break;
            case rule_born:
                symbol_name = "RULE_BORN";
                break;
            case rule_survive:
                symbol_name = "RULE_SURVIVE";
                break;
            case slash:
                symbol_name = "SLASH";
                break;
            case cell_alive:
                symbol_name = "CELL_ALIVE";
                break;
            case cell_dead:
                symbol_name = "CELL_DEAD";
                break;
            case line_end:
                symbol_name = "END";
                break;
            case eof:
                symbol_name = "EOF";
                break;
            case newline:
                symbol_name = "NEWLINE";
                break;
        }

        //if (strcmp(symbol_name, "NUMBER") != 0) {

            SDL_Log("%s %s", symbol_name, token.value);
        //}
    }*/

    return NULL;
}