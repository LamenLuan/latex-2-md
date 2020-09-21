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

unsigned chapterQuant, sectionQuant, subSectionQuant;

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
    Tsection,
    TsubSection,
    Tparagraph,
    Tbold,
    Tunderline,
    Titalic,
    Titem,
    Tenum,
    Tbreak
}TnodeType;

struct ast
{
    enum nodeType nodeType;
    struct wordList *list;
    struct ast *right, *left;
    unsigned tabOrder;
};

struct wordList *newWordList(char *, struct wordList *);

// Non leaf nodes
struct ast *newAST(struct ast *, struct ast *);

// Leaf nodes
struct ast *newElement(char *, char *, TnodeType);

// To open the output and call a recursive function
void callMakeOutput(struct ast *);
void makeOutput(struct ast *, FILE *, unsigned *, unsigned *);
