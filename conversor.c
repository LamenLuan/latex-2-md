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

    makeOutput(head, output, 0, 0);

    fclose(output);
}

void makeOutput
(
    struct ast *head, FILE *output, unsigned *sections,
    unsigned *subSections
)
{
    switch (head->nodeType)
    {
        case Tast:
        {
            makeOutput(head->left, output, sections, subSections);
            makeOutput(head->right, output, sections, subSections);
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
        } break;

        case Tpackage:
        {
            struct wordList *list = head->list;
            
            fprintf(output, "  \nPackage: %s", list->word);
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
            fprintf(output, "  \n***");
        } break;

        case Ttitle:
            fprintf(output, "  \n# %s", head->list->word);
            break;

        case Tauthor:
            fprintf(output, "  \n### %s", head->list->word);
            break;

        case Tchapter:
            fprintf(output, "  \n#### %s", head->list->word);
            break;

        case Tparagraph:
            fprintf(output, "  \n%s", head->list->word);
            break;

        case Tbold:
            fprintf(output, " **%s**", head->list->word);
            break;

        case Tunderline:
            fprintf(output, " <u>%s</u>", head->list->word);
            break;

        case Titalic:
            fprintf(output, " *%s*", head->list->word);
            break;

        case Tsection: (output, "  \n## %s", head->list->word);
            break;

        case TsubSection:
            fprintf(output, "  \n### %s", head->list->word);
            break;

        case Titem: {
            fprintf(output, "\n* %s", head->list->word);
        } break;

        case Tenum: {
            fprintf(output, "\n1. %s", head->list->word);
        } break;

        case Tbreak:
            fprintf(output, "%s", head->list->word);
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