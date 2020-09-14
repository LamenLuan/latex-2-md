%{
    #include "conversor.h"
%}
%union {
    char *str;
    struct stringList *list;
    struct ast *a;
}

%token <str> STRING NAME
%token CLASS PACKAGE TITLE AUTHOR

%type <a> configuration identification documentClass usePackage title author
%type <list> stringList

%%
latexDocument: configuration identification {
    callMakeOutput( newAST($1, $2) );
}
;

configuration: documentClass usePackage { $$ = newAST($1, $2); }
    | documentClass
;

identification: title author { $$ = newAST($1, $2); }
    | title
;

title: TITLE '{' NAME '}' { $$ = newElement(NULL, $3, Ttitle); }
    | TITLE '{' STRING '}' { $$ = newElement(NULL, $3, Ttitle); }
;

author: AUTHOR '{' NAME '}' { $$ = newElement(NULL, $3, Tauthor); }
    | AUTHOR '{' STRING '}' { $$ = newElement(NULL, $3, Tauthor); }
;

documentClass: CLASS '[' stringList ']' '{' STRING '}' {
        $$ = newElement($3, $6, Tclass);
    }
    | CLASS '{' STRING '}' { $$ = newElement(NULL, $3, Tclass); }
;

usePackage: PACKAGE '[' stringList ']' '{' STRING '}' {
    $$ = newElement($3, $6, Tpackage);
}
    | PACKAGE '{' STRING '}' { $$ = newElement(NULL, $3, Tpackage); }
;

stringList: STRING ',' stringList { $$ = newStringList($1, $3); }
    | STRING { $$ = newStringList($1, NULL); }
;