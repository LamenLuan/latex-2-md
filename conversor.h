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
unsigned listLevel;

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
    Tbreak,
    TlistBegin,
    TlistEnd
}TnodeType;

struct ast
{
    enum nodeType nodeType;
    char *text, *secondaryText;
    struct ast *right, *left;
    unsigned tabOrder;
};

// Non leaf nodes
struct ast *newAST(struct ast *, struct ast *);

// Leaf nodes
struct ast *newElement(char *, char *, TnodeType);

// To open the output and call a recursive function
void callMakeOutput(struct ast *);
void makeOutput(struct ast *, FILE *, unsigned *, unsigned *);

void destroyAST(struct ast *);