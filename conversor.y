%{
    #include "conversor.h"
%}
%union {
    char *c;
    struct ast *a;
}

%token <c> WORD ACCENTUATION
%token CLASS PACKAGE TITLE AUTHOR START MAKETITLE END 
CHAPTER PARAGRAPH BOLD UNDERLINE ITALIC SECTION SUBSECTION 
BEGINITEMLIST ENDITEMLIST ITEM BEGINENUMLIST ENDENUMLIST

%type <a> configuration identification documentClass usePackage title 
author main bodyList chapter section subSection body text stylizedText
lists itemList item enumList enum
%type <c> wordList

%start latexDocument
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

title: TITLE '{' wordList '}' { $$ = newElement($3, NULL, Ttitle); }
;

author: AUTHOR '{' wordList '}' { $$ = newElement($3, NULL, Tauthor); }
;

documentClass: CLASS '[' wordList ']' '{' WORD '}' {
        $$ = newElement($6, $3, Tclass);
    }
    | CLASS '{' WORD '}' { $$ = newElement($3, NULL, Tclass); }
;

usePackage: PACKAGE '[' wordList ']' '{' WORD '}' {
    $$ = newElement($6, $3, Tpackage);
}
    | PACKAGE '{' WORD '}' { $$ = newElement($3, NULL, Tpackage); }
;

main: START MAKETITLE bodyList END { $$ = $3; }
;

bodyList: chapter
    | body
;

chapter: CHAPTER '{' wordList '}' section chapter {
        $$ = newAST(
            newElement($3, NULL, Tchapter), newAST($5, $6) 
        );
    }
    | CHAPTER '{' wordList '}' section {
        $$ = newAST( newElement($3, NULL, Tchapter), $5 );
    }
;

section: SECTION '{' wordList '}' subSection section {
        $$ = newAST(
           newElement($3, NULL, Tsection), newAST($5, $6) 
        );
    }
    | SECTION '{' wordList '}' subSection {
        $$ = newAST( newElement($3, NULL, Tsection), $5 );
    }
    | body
;

subSection: SUBSECTION '{' wordList '}' body subSection {
        $$ = newAST(
            newElement($3, NULL, TsubSection), newAST($5, $6) 
        );
    }
    | SUBSECTION '{' wordList '}' body {
        $$ = newAST( newElement($3, NULL, TsubSection), $5 );
    }
    | body
;

body: text
    | text body { $$ = newAST($1, $2); }
    | stylizedText body { $$ = newAST($1, $2); }
    | lists body { $$ = newAST($1, $2); }
;

text: PARAGRAPH '{' wordList '}' { $$ = newElement($3, NULL, Tparagraph); }
;

stylizedText: BOLD '{' wordList '}' { $$ = newElement($3, NULL, Tbold); }
    | UNDERLINE '{' wordList '}' { $$ = newElement($3, NULL, Tunderline); }
    | ITALIC '{' wordList '}' { $$ = newElement($3, NULL, Titalic); }
;

lists: itemList {
        $$ = newAST( $1, newElement("\n", NULL, Tbreak) );
    }
    | enumList {
        $$ = newAST( $1, newElement("\n", NULL, Tbreak) );
    }
;

itemList: BEGINITEMLIST item ENDITEMLIST { $$ = $2; }
;

enumList: BEGINENUMLIST enum ENDENUMLIST { $$ = $2; }
;

item: ITEM '{' wordList '}' { $$ = newElement($3, NULL, Titem); }
    | ITEM '{' wordList '}' item {
        $$ = newAST(newElement($3, NULL, Titem), $5);
    }
    | lists
;

enum: ITEM '{' wordList '}' { $$ = newElement($3, NULL, Tenum); }
    | ITEM '{' wordList '}' enum {
        $$ = newAST(newElement($3, NULL, Tenum), $5);
    }
    | lists
;

wordList: WORD wordList { strcat($1, $2); $$ = $1; }
    | ACCENTUATION wordList { strcat($1, $2); $$ = $1; }
    | ACCENTUATION
    | WORD
;