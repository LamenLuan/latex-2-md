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
        $$ = newClass($3, $6);
    }
    | CLASS '{' STRING '}' { $$ = newClass(NULL, $3); }
;

usePackage: PACKAGE '[' stringList ']' '{' STRING '}' {
    $$ = newPackage($3, $6);
}
    | PACKAGE '{' STRING '}' { $$ = newPackage(NULL, $3); }
;

stringList: STRING ',' stringList { $$ = newStringList($1, $3); }
    | STRING { $$ = newStringList($1, NULL); }
;