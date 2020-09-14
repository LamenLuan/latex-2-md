%option noyywrap nodefault yylineno
%{
    #include "conversor.h"
    #include "conversor.tab.h"
%}
STR [a-zA-Z0-9]+
%%
"[" |
"]" |
"{" |
"}" |
"," { return yytext[0]; }

"\\documentclass" { return CLASS; }
"\\usepackage" { return PACKAGE; }
"\\title" { return TITLE; }
"\\author" { return AUTHOR; }

{STR} { yylval.str = strdup(yytext); return STRING; }
({STR}" ")+{STR} { yylval.str = strdup(yytext); return NAME; }

\n {}
"//".*
[ \t] {}
. { yyerror("Caracter not recognized %c \n", *yytext); }

%%