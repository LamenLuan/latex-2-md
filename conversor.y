%{
    #include "conversor.h"
%}
%union {
    char *c;
    struct wordList *l;
    struct ast *a;
}

%token <c> TEXT WORD
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

documentClass: CLASS '[' wordList ']' '{' WORD '}' {
        $$ = newElement($3, $6, Tclass);
    }
    | CLASS '{' WORD '}' { $$ = newElement(NULL, $3, Tclass); }
;

usePackage: PACKAGE '[' wordList ']' '{' WORD '}' {
    $$ = newElement($3, $6, Tpackage);
}
    | PACKAGE '{' WORD '}' { $$ = newElement(NULL, $3, Tpackage); }
;

wordList: WORD ',' wordList { $$ = newWordList($1, $3); }
    | WORD { $$ = newWordList($1, NULL); }
;

main: START MAKETITLE bodyList END { $$ = $3; }
;

bodyList: chapter
;

chapter: CHAPTER '{' TEXT '}' { $$ = newElement(NULL, $3, Tchapter); }