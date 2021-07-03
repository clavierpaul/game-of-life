%{
    #include "rle.tab.h"

    extern void yyerror(const char *s);
%}

%option noyywrap

%%
[ \t]+      ;
#[^\n]+ { yylval.sval = strdup(yytext); return TAG; }
\n          { return NEWLINE; };
x|X         { return WIDTH; }
y|Y         { return HEIGHT; }
rule        { return RULE; }
=           { return EQUALS; }
,           { return COMMA; }
[0-9]+      { yylval.ival = atoi(yytext); return NUMBER; }
b|B         { return RULE_BORN_OR_CELL_DEAD; } // unfortunately b can be both depending on context
\/s|\/S     { return RULE_SURVIVE; }
o|O         { return CELL_ALIVE; }
\$          { return NEXT_LINE; } // not to be confused with newline
%%