#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* interfacing */
extern int yylineno;
extern FILE *yyin;
void yyerror(char *s, ...);

struct stringList
{
    char *string;
    struct stringList *next;
};

enum nodeType
{
    Tast,
    Tclass,
    Tpackage
};

struct ast
{
    enum nodeType nodeType;
    struct stringList *list;
    struct ast *right, *left;
};

struct stringList *newStringList(char *, struct stringList *);

struct ast *newAST(struct ast *, struct ast *);
struct ast *newClass(char *, struct stringList *);
struct ast *newPackage(char *, struct stringList *);

// To open the output and call a recursive function
void callMakeOutput(struct ast *head);
void makeOutput(struct ast *, FILE *);