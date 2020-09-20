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
"}" { return yytext[0]; }

"\\documentclass" { return CLASS; }
"\\usepackage" { return PACKAGE; }
"\\title" { return TITLE; }
"\\author" { return AUTHOR; }
"\\begin{document}" { return START; }
"\\maketitle" { return MAKETITLE; }
"\\end{document}" { return END; }
"\\chapter" { return CHAPTER; }
"\\section" { return SECTION; }
"\\subsection" { return SUBSECTION; }
"\\paragraph" { return PARAGRAPH; }
"\\bf" { return BOLD; }
"\\underline" { return UNDERLINE; }
"\\it" { return ITALIC; }
"\\begin{itemize}" { return BEGINITEMLIST; }
"\\end{itemize}" { return ENDITEMLIST; }
"\\begin{enumerate}" { return BEGINENUMLIST; }
"\\end{enumerate}" { return ENDENUMLIST; }
"\\item" { return ITEM; }

","|"."|" " { yylval.c = strdup(yytext); return ACCENTUATION;  }

{ALPHANUM} { yylval.c = strdup(yytext); return WORD; }

\n {}
"//".*
[ \t] {}
. { yyerror("Caracter not recognized %c \n", *yytext); }

%%