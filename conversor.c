#include "conversor.h"

void yyerror(char *s, ...)
{
    va_list ap;
    va_start(ap, s);

    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}

struct stringList *newStringList(char *str, struct stringList *next)
{
    struct stringList *p = malloc( sizeof(struct stringList) );

    if(!p)
    {
        yyerror("Sem espaco!");
        exit(0);
    }

    p->string = strdup(str);
    p->next = next;

    return p;
}

struct ast *newAST(struct ast *left, struct ast *right)
{
    struct ast *p = malloc( sizeof(struct ast) );

    if(!p)
    {
        yyerror("Sem espaco!");
        exit(0);
    }

    p->left = left;
    p->right = right;
    p->nodeType = Tast;

    return p;
}

struct ast *newClass(struct stringList *list, char *name)
{
    struct ast *p = newAST(NULL, NULL);

    p->list = newStringList(name, list);
    p->nodeType = Tclass;

    return p;
}

struct ast *newPackage(struct stringList *list, char *name)
{
    struct ast *p = newAST(NULL, NULL);

    p->list = newStringList(name, list);
    p->nodeType = Tpackage;

    return p;
}

void callMakeOutput(struct ast *head)
{
    FILE *output = fopen("output.md", "w");

    makeOutput(head, output);

    fclose(output);
}

void makeOutput(struct ast *head, FILE *output)
{
    switch (head->nodeType)
    {
        case Tast:
        {
            makeOutput(head->left, output);
            makeOutput(head->right, output);
        } break;

        case Tclass:
        {
            struct stringList *list = head->list;
        
            fprintf(output, "Class: %s", list->string);
            if(list->next)
            {
                fprintf(output, "[");
                while (list = list->next)
                {
                    fprintf(output, "%s", list->string);
                    if(list->next) fprintf(output, ", ");
                }
                fprintf(output, "]");
            }
            fprintf(output, "  \n");
        } break;

        case Tpackage:
        {
            struct stringList *list = head->list;
            
            fprintf(output, "Package: %s", list->string);
            if(list->next)
            {
                fprintf(output, "[");
                while (list = list->next)
                {
                    fprintf(output, "%s", list->string);
                    if(list->next) fprintf(output, ", ");
                }
                fprintf(output, "]");
            }
            fprintf(output, "  \n***  \n");
        } break;

        default: break;
    }
}

int main()
{
    FILE *inputFile = fopen("input.tex", "r");
    
    if(!inputFile)
    {
        yyerror("Bad input");
        return -1;
    }

    yyin = inputFile;
    yyparse();

    fclose(inputFile);
}