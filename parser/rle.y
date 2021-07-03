%{
    #include <SDL_log.h>
    #include "rle.h"

    extern int yylex();
    void yyerror(const char *s);
%}

// Tokens can either have an associated number or a string tag
%union {
    int ival;
    char *sval;
}

%token WIDTH HEIGHT RULE
%token EQUALS COMMA SLASH NEWLINE
%token RULE_SURVIVE RULE_BORN_OR_CELL_DEAD
%token CELL_ALIVE NEXT_LINE
%token <ival> NUMBER
%token <sval> TAG

%%
file:
    tags header
    ;

tag: TAG NEWLINE { rle_add_tag($1); }

tags:
      tags tag
    | tag
    ;

header_setting:
      WIDTH EQUALS NUMBER { rle_set_width($3); }
    | HEIGHT EQUALS NUMBER { rle_set_height($3); }
    | RULE EQUALS RULE_BORN_OR_CELL_DEAD NUMBER RULE_SURVIVE NUMBER { rle_set_rules($4, $6); }
    ;

header_settings:
      header_settings COMMA header_setting
    | header_setting
    ;

header: header_settings NEWLINE;
%%

void yyerror(const char *s) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error parsing RLE file");
}