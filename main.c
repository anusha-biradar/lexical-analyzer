#include <stdio.h>
#include "lexer.h"

int main()
{
    FILE *fp;
    fp= fopen("input.c", "r");
    if(fp==NULL)
    {
        printf("Error : input.c file not found\n");
        return 0;
    }
    printf("\n------ Lexical Analyzer ------\n\n");
    scan_file(fp);
    fclose(fp);

    return 0;
}