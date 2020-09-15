#include "conversor.h"

void yyerror(char *s, ...)
{
    va_list ap;
    va_start(ap, s);

    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}

struct wordList *newWordList(char *word, struct wordList *next)
{
    struct wordList *p = malloc( sizeof(struct wordList) );

    if(!p)
    {
        yyerror("Sem espaco!");
        exit(0);
    }

    p->word = strdup(word);
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

struct ast *newElement
(
    struct wordList *list, char *name, TnodeType type
)
{
    struct ast *p = newAST(NULL, NULL);

    p->list = newWordList(name, list);
    p->nodeType = type;

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
            struct wordList *list = head->list;
        
            fprintf(output, "Class: %s", list->word);
            if(list->next)
            {
                fprintf(output, "[");
                while (list = list->next)
                {
                    fprintf(output, "%s", list->word);
                    if(list->next) fprintf(output, ", ");
                }
                fprintf(output, "]");
            }
            fprintf(output, "  \n");
        } break;

        case Tpackage:
        {
            struct wordList *list = head->list;
            
            fprintf(output, "Package: %s", list->word);
            if(list->next)
            {
                fprintf(output, "[");
                while (list = list->next)
                {
                    fprintf(output, "%s", list->word);
                    if(list->next) fprintf(output, ", ");
                }
                fprintf(output, "]");
            }
            fprintf(output, "  \n***  \n");
        } break;

        case Ttitle:
            fprintf(output, "# %s  \n", head->list->word);
            break;

        case Tauthor:
            fprintf(output, "### %s  \n", head->list->word);
            break;

        case Tchapter:
            fprintf(output, "#### %s  \n", head->list->word);
            break;

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