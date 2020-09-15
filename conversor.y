%{
    #include "conversor.h"
%}
%union {
    char *c;
    struct wordList *l;
    struct ast *a;
}

%token <c> TEXT
%token CLASS PACKAGE TITLE AUTHOR START MAKETITLE END CHAPTER

%type <a> configuration identification documentClass usePackage title 
author main bodyList chapter
%type <l> wordList

%%
latexDocument: configuration identification main {
    callMakeOutput( newAST( $1, newAST($2, $3) ) );
}
;

configuration: documentClass usePackage { $$ = newAST($1, $2); }
    | documentClass
;

identification: title author { $$ = newAST($1, $2); }
    | title
;

title: TITLE '{' TEXT '}' { $$ = newElement(NULL, $3, Ttitle); }
;

author: AUTHOR '{' TEXT '}' { $$ = newElement(NULL, $3, Tauthor); }
;

documentClass: CLASS '[' wordList ']' '{' TEXT '}' {
        $$ = newElement($3, $6, Tclass);
    }
    | CLASS '{' TEXT '}' { $$ = newElement(NULL, $3, Tclass); }
;

usePackage: PACKAGE '[' wordList ']' '{' TEXT '}' {
    $$ = newElement($3, $6, Tpackage);
}
    | PACKAGE '{' TEXT '}' { $$ = newElement(NULL, $3, Tpackage); }
;

wordList: TEXT ',' wordList { $$ = newWordList($1, $3); }
    | TEXT { $$ = newWordList($1, NULL); }
;

main: START MAKETITLE bodyList END { $$ = $3; }
;

bodyList: chapter
;

chapter: CHAPTER '{' TEXT '}' { $$ = newElement(NULL, $3, Tchapter); }