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

typedef enum nodeType
{
    Tast,
    Tclass,
    Tpackage,
    Ttitle,
    Tauthor
}TnodeType;

struct ast
{
    enum nodeType nodeType;
    struct stringList *list;
    struct ast *right, *left;
};

struct stringList *newStringList(char *, struct stringList *);

// Non leaf nodes
struct ast *newAST(struct ast *, struct ast *);

// Leaf nodes
struct ast *newElement(struct stringList *, char *, TnodeType);

// To open the output and call a recursive function
void callMakeOutput(struct ast *head);
void makeOutput(struct ast *, FILE *);