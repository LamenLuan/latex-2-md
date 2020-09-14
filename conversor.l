%option noyywrap nodefault yylineno
%{
    #include "conversor.h"
    #include "conversor.tab.h"
%}

%%
"[" |
"]" |
"{" |
"}" |
"," { return yytext[0]; }

"\\documentclass" { return CLASS; }
"\\usepackage" { return PACKAGE; }

[a-zA-Z0-9]+ { yylval.str = strdup(yytext); return STRING; }

\n {}
"//".*
[ \t] {}
. { yyerror("Caracter not recognized %c \n", *yytext); }

%%