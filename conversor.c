#include "conversor.h"

unsigned chapterQuant = 0, sectionQuant = 0, subSectionQuant = 0;
unsigned listLevel = 0;

void yyerror(char *s, ...)
{
    va_list ap;
    va_start(ap, s);

    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
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

struct ast *newElement(char *name, char *next, TnodeType type)
{
    struct ast *p = newAST(NULL, NULL);

    p->text = name;
    p->secondaryText = next;

    p->nodeType = type;

    return p;
}

void callMakeOutput(struct ast *head)
{
    FILE *output = fopen("output.md", "w");

    makeOutput(head, output, 0, 0);

    destroyAST(head);

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
            fprintf(output, "Class: %s", head->text);
            if(head->secondaryText)
                fprintf(output, "[%s]", head->secondaryText);
        } break;

        case Tpackage:
        {
            fprintf(output, "  \nPackage: %s", head->text);
            if(head->secondaryText)
                fprintf(output, "[%s]", head->secondaryText);
            fprintf(output, "  \n***");
        } break;

        case Ttitle:
            fprintf(output, "  \n# %s", head->text);
            break;

        case Tauthor:
            fprintf(output, "  \n### %s", head->text);
            break;

        case Tchapter:
            {
                fprintf
                (
                    output, "  \n## %u. %s", ++chapterQuant,
                    head->text
                );
                sectionQuant = subSectionQuant = 0;
            } break;

        case Tparagraph:
            fprintf(output, "  \n%s", head->text);
            break;

        case Tbold:
            fprintf(output, " **%s**", head->text);
            break;

        case Tunderline:
            fprintf(output, " <u>%s</u>", head->text);
            break;

        case Titalic:
            fprintf(output, " *%s*", head->text);
            break;

        case Tsection:
            {
                fprintf
                (
                    output, "  \n### %u.%u. %s", chapterQuant,
                    ++sectionQuant, head->text
                );
                subSectionQuant = 0;
            } break;

        case TsubSection:
            fprintf
            (
                output, "  \n#### %u.%u.%u. %s", chapterQuant,
                sectionQuant, ++subSectionQuant, head->text
            );
            break;

        case Titem: {
            fprintf(output, "\n");
            for (size_t i = 1; i < listLevel; i++)
            {
                fprintf(output, "  ");
            }
            fprintf(output, "* %s", head->text);
        } break;

        case Tenum: {
            fprintf(output, "\n");
            for (size_t i = 1; i < listLevel; i++)
            {
                fprintf(output, "  ");
            }
            fprintf(output, "1. %s", head->text);
        } break;

        case Tbreak:
            fprintf(output, "%s", head->text);
            break;

        case TlistBegin: ++listLevel; break;

        case TlistEnd:
        {
            --listLevel;
            if(!listLevel) fprintf(output, "\n");
        } break;

        default: break;
    }
}

void destroyAST(struct ast *p)
{
    if(p) {
        if(p->secondaryText) free(p->secondaryText);
        if(p->text) free(p->text);

        if(p->nodeType == Tast)
        {
            destroyAST(p->left);
            destroyAST(p->right);
        }
        free(p);
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