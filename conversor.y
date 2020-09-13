%{
    #include "conversor.h"
%}
%union {
    char *str;
    struct stringList *list;
    struct ast *a;
}

%token <str> STRING
%token CLASS PACKAGE

%type <a> configuration documentClass usePackage
%type <list> stringList

%%
latexDocument: configuration {
    callMakeOutput($1);
}
;

configuration: documentClass usePackage { $$ = newAST($1, $2); }
    | documentClass
;

documentClass: CLASS '[' stringList ']' '{' STRING '}' {
        $$ = newClass($6, $3);
    }
    | CLASS '{' STRING '}' { $$ = newClass($3, NULL); }
;

usePackage: PACKAGE '[' stringList ']' '{' STRING '}' {
    $$ = newPackage($6, $3);
}
    | PACKAGE '{' STRING '}' { $$ = newPackage($3, NULL); }
;

stringList: STRING ',' stringList { $$ = newStringList($1, $3); }
    | STRING { $$ = newStringList($1, NULL); }
;