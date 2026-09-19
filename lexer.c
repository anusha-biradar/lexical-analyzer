#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

char *data_kwords[]={"const","volatile","extern","auto","register","static","signed","unsigned","short","long","double","char","int","float","struct","union","enum","void","typedef",""};
char *non_data_kwords[]={"goto","return","continue","break","if","else","for","while","do","switch","case","default","sizeof",""};
char operators[]="+ - * / % = < > ! & | ^ ~";
char symbols[]="() {} [] , ; : ";

//relational operators
char rel_op[6][3]={"<",">","<=",">=","==","!="};

int brace=0;
int paren=0;

int check_keyword(char *word)
{
    int i;

    for(i=0; data_kwords[i][0]!='\0'; i++)
    {
        if(strcmp(word, data_kwords[i])==0)
            return 1;
    }

    for(i=0; non_data_kwords[i][0]!='\0'; i++)
    {
        if(strcmp(word, non_data_kwords[i]) ==0)
            return 1;
    }

    return 0;
}

int check_relop(char *word)
{
    int i;

    for(i=0; i<6;i++)
    {
        if(strcmp(word, rel_op[i])==0)
            return 1;
    }

    return 0;
}

int check_operator(char ch)
{
    int i;
    for(i=0; operators[i]!='\0'; i++)
    {
        if(ch== operators[i])
            return 1;
    }

    return 0;
}

int check_symbol(char ch)
{
    int i;

    for(i=0; symbols[i]!='\0'; i++)
    {
        if(ch== symbols[i])
            return 1;
    }

    return 0;
}

void print_token(char *word,char *type)
{
    printf("%-15s : %s\n", word, type);
}

void check_braces(char ch)
{
    if(ch=='{')
        brace++;

    if(ch=='}')
    {
        if(brace== 0)
            printf("Error : closing brace without opening brace\n");
        else
            brace--;
    }
}

void check_parens(char ch)
{
    if(ch=='(')
        paren++;

    if(ch==')')
    {
        if(paren==0)
            printf("\nError : closing parenthesis without opening parenthesis\n\n");
        else
            paren--;
    }
}

int check_number(char *word)
{
    int i;
    int dot= 0;

    //variable should not start with number
    if(isdigit(word[0]))
    {
        for(i=1; word[i]!='\0'; i++)
        {
            if(isalpha(word[i]) || word[i]=='_')
            {
                if(word[1]=='b' || word[1]=='B' || word[1]=='x' || word[1]=='X')
                    break;

                if(i==1)
                    printf("\nError : variable should not start with a number -> %s\n\n",word);
                else
                    printf("\nError : Decimal number should not contain alphabets -> %s\n\n",word);

                return 0;
            }
        }
    }

    //binary number
    if(word[0]=='0' && (word[1]=='b' || word[1]=='B'))
    {
        if(word[2]== '\0')
        {
            printf("\nError : invalid binary number -> %s\n\n",word);
            return 0;
        }

        for(i=2; word[i]!='\0'; i++)
        {
            if(word[i]!='0' && word[i]!='1')
            {
                printf("\nError : invalid binary number -> %s\n\n", word);
                return 0;
            }
        }

        return 1;
    }

    //hexadecimal number
    if(word[0]=='0' && (word[1]=='x' || word[1]=='X'))
    {
        if(word[2]=='\0')
        {
            printf("\nError : invalid hexadecimal number -> %s\n\n", word);
            return 0;
        }

        for(i=2;word[i]!='\0';i++)
        {
            if(!isxdigit(word[i]))
            {
                printf("\nError : invalid hexadecimal number -> %s\n\n", word);
                return 0;
            }
        }

        return 1;
    }

    //octal number
    if(word[0]=='0' && word[1]!='.' && word[1]!= '\0')
    {
        for(i=1; word[i]!='\0'; i++)
        {
            if(word[i]<'0' || word[i]>'7')
            {
                printf("\nError : invalid octal number -> %s\n\n", word);
                return 0;
            }
        }
        
        return 1;
    }

    //decimal number
    for(i=0; word[i]!='\0'; i++)
    {
        if(word[i]=='.')
        {
            dot++;

            if(dot > 1)
            {
                printf("\nError : invalid floating number -> %s\n\n",word);
                return 0;
            }
        }
        else if(!isdigit(word[i]))
        {
            printf("\nError : decimal number should not contain alphabets -> %s\n\n", word);
            return 0;
        }
    }

    return 1;
}

void scan_file(FILE *fp)
{
    int ch;
    int next;
    int i;
    int line=1;
    int square=0;
    char word[100];
    char op[3];

    while((ch=fgetc(fp))!= EOF)
    {
        if(ch=='\n')
        {
            if(square!= 0)
            {
                printf("\nError : square bracket should be opened and closed in the same line\n\n");
                square =0;
            }

            line++;
            continue;
        }

        if(isspace(ch))
            continue;

        //header file
        if(ch=='#')
        {
            i=0;
            word[i++]=ch;

            while((ch=fgetc(fp))!=EOF && ch!='\n')
                word[i++]=ch;

            word[i]='\0';

            print_token(word,"header");

            line++;
            continue;
        }

        //comments
        if(ch=='/')
        {
            next=fgetc(fp);

            //single line comment
            if(next=='/')
            {
                i=0;
                word[i++]=ch;
                word[i++]=next;

                while((ch=fgetc(fp))!=EOF && ch!='\n')
                    word[i++]=ch;

                word[i]='\0';

                print_token(word,"comment");

                if(ch=='\n')
                    line++;

                continue;
            }

            //multi line comment
            if(next=='*')
            {
                int closed= 0;
                i=0;
                word[i++]= ch;
                word[i++]= next;

                while((ch=fgetc(fp))!=EOF)
                {
                    word[i++]= ch;

                    if(ch=='*')
                    {
                        next=fgetc(fp);

                        if(next=='/')
                        {
                            word[i++]=next;
                            closed=1;
                            break;
                        }

                        if(next!=EOF)
                            ungetc(next,fp);
                    }

                    if(ch=='\n')
                        line++;
                }

                word[i]='\0';

                if(closed)
                    print_token(word,"comment");
                else
                    printf("\nError : multi line comment is not closed\n\n");

                continue;
            }

            if(next!=EOF)
                ungetc(next,fp);

            print_token("/","operator");
            continue;
        }

            //for keyword or identifier
            if(isalpha(ch) || ch=='_')
            {
                i=0;
                word[i++]= ch;

                while((ch= fgetc(fp))!=EOF && (isalnum(ch) || ch=='_'))
                    word[i++]=ch;

                word[i]= '\0';

                if(ch!= EOF)
                    ungetc(ch,fp);

                if(check_keyword(word))
                    print_token(word, "keyword");
                else
                    print_token(word, "identifier");

                continue;
            }

        //constants
        if(isdigit(ch))
        {
            i=0;
            word[i++]=ch;

            while((ch=fgetc(fp))!= EOF && (isalnum(ch) || ch=='.'))
                word[i++]=ch;

            word[i]='\0';

            if(ch!=EOF)
                ungetc(ch, fp);

            if(check_number(word))
                print_token(word,"constant");

            continue;
        }

        //string
        if(ch=='"')
        {
            i=0;
            word[i++]=ch;

            while((ch=fgetc(fp))!=EOF)
            {
                word[i++]= ch;

                if(ch=='"')
                    break;

                if(ch=='\n')
                    line++;
            }

            word[i]='\0';

            if(word[i-1]!='"')
                printf("\nError : string is not closed\n\n");
            else
                print_token(word,"string");

            continue;
        }

        //character
        if(ch=='\'')
        {
            i=0;
            word[i++]=ch;

            while((ch=fgetc(fp))!=EOF)
            {
                word[i++]=ch;

                if(ch=='\'')
                    break;
            }

            word[i]='\0';

            if(word[i-1]!='\'')
                printf("\nError : character is not closed\n");
            else
                print_token(word,"character");

            continue;
        }

        //curly braces
        if(ch=='{' || ch=='}')
        {
            check_braces(ch);

            if(ch=='{')
                print_token("{","o_brace");
            else
                print_token("}","c_brace");

            continue;
        }

        //parentheses
        if(ch=='(' || ch==')')
        {
            check_parens(ch);

            if(ch=='(')
                print_token("(","open_brace");
            else
                print_token(")","close_brace");

            continue;
        }

        //square brackets
        if(ch=='[')
        {
            square++;
            print_token("[","sq_o_brace");
            continue;
        }

        if(ch==']')
        {
            if(square== 0)
                printf("\nError : closing square bracket without opening bracket\n\n");
            else
            {
                square--;
                print_token("]", "sq_o_brace");
            }
            continue;
        }

        //operators
        if(check_operator(ch))
        {
            op[0]=ch;
            op[1]='\0';

            if(ch=='<' || ch=='>' || ch=='=' || ch=='!')
            {
                next=fgetc(fp);

                if(next=='=')
                {
                    op[1]='=';
                    op[2]='\0';

                    if(check_relop(op))
                        print_token(op,"relational_operator");
                    else
                        print_token(op, "operator");

                    continue;
                }

                if(next!=EOF)
                    ungetc(next,fp);

                if(ch=='<'|| ch=='>')
                    print_token(op, "relational_operator");
                else
                    print_token(op, "operator");

                continue;
            }

            print_token(op, "operator");
            continue;
        }

        //special_characters
        if(check_symbol(ch))
        {
            op[0]=ch;
            op[1]='\0';

            if(ch==';')
                print_token(op,"st_terminator");
            else
                print_token(op,"special_character");
            continue;
        }

        printf("\nError : invalid character '%c' at line %d\n\n", ch, line);
    }

    if(square!=0)
        printf("\nError : square bracket should be opened and closed in the same line\n\n");

    if(brace!=0)
        printf("\nError : curly braces are not properly closed\n\n");

    if(paren!=0)
        printf("\nError : parentheses are not properly closed\n\n");
}