%option noyywrap nodefault yylineno
%{
    #include "conversor.h"
    #include "conversor.tab.h"
%}
ALPHANUM [a-zA-Z0-9]+
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
"\\begin{document}" { return START; }
"\\maketitle" { return MAKETITLE; }
"\\end{document}" { return END; }
"\\chapter" { return CHAPTER; }
"\\paragraph" { return PARAGRAPH; }
"\\bf" { return BOLD; }
"\\underline" { return UNDERLINE; }
"\\it" { return ITALIC; }

{ALPHANUM} { yylval.c = strdup(yytext); return WORD; }

\n {}
"//".*
[ \t] {}
. { yyerror("Caracter not recognized %c \n", *yytext); }

%%