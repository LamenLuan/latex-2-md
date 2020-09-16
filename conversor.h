#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* interfacing */
extern int yylineno;
extern FILE *yyin;
extern int yylex();
extern int yyparse();
void yyerror(char *s, ...);

struct wordList
{
    char *word;
    struct wordList *next;
};

typedef enum nodeType
{
    Tast,
    Tclass,
    Tpackage,
    Ttitle,
    Tauthor,
    Tchapter,
    Tparagraph,
    Tbold,
    Tunderline,
    Titalic
}TnodeType;

struct ast
{
    enum nodeType nodeType;
    struct wordList *list;
    struct ast *right, *left;
};

struct wordList *newWordList(char *, struct wordList *);

// Non leaf nodes
struct ast *newAST(struct ast *, struct ast *);

// Leaf nodes
struct ast *newElement(struct wordList *, char *, TnodeType);

// To open the output and call a recursive function
void callMakeOutput(struct ast *head);
void makeOutput(struct ast *, FILE *);