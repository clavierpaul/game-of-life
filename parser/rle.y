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
%token EQUALS COMMA
%token RULE_SURVIVE RULE_BORN_OR_CELL_DEAD
%token CELL_ALIVE NEXT_LINE END
%token <ival> NUMBER
%token <sval> TAG

%%
file:
    tags header body END
    ;

tag: TAG { rle_add_tag($1); }

tags:
      tags tag
    | tag
    ;

header_setting:
      WIDTH EQUALS NUMBER { rle_set_width($3); }
    | HEIGHT EQUALS NUMBER { rle_set_height($3); }
    | RULE EQUALS RULE_BORN_OR_CELL_DEAD NUMBER RULE_SURVIVE NUMBER { rle_set_rules($4, $6); }
    ;

header:
      header COMMA header_setting
    | header_setting
    ;

cells:
      NUMBER CELL_ALIVE { rle_add_cells($1); }
    | NUMBER RULE_BORN_OR_CELL_DEAD { rle_skip_cells($1); }
    | CELL_ALIVE { rle_add_cells(1); }
    | RULE_BORN_OR_CELL_DEAD { rle_skip_cells(1); }
    ;

row:
      cells NEXT_LINE { rle_next_line(); }
    | cells
    ;

body:
      body row
    | row
    ;
%%

// Don't care about yyerror since error handling in done in rle.c
void yyerror(const char *s) {}