#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "pallache.h"

using namespace std;

int main(int argc,char *argv[])
{
    char a[2048];
    pallache::pallache<double> parser;
    FILE *file=(argc>1)?fopen(argv[1],"r"):stdin;
    if(file) while(true)
    {
        printf(">>> ");
        fgets(a,2048,file);
        if(feof(file))
        {
            printf("exit\n");
            break;
        }
        a[strcspn(a,"\n")]=0;
        if(strcmp(a,"q") and strcmp(a,"quit") and strcmp(a,"exit"))
        {
            if(a[0]=='#') continue;
            else if(!strcmp(a,"")) continue;
            else if(!strcmp(a,"reset_all"))
            {
                parser.reset_all();
                printf("functions and variables table reinitialized\n");
            }
            else if(!strcmp(a,"reset_vars"))
            {
                parser.reset_all();
                printf("variables table reinitialized\n");
            }
            else if(!strcmp(a,"reset_funcs"))
            {
                parser.reset_all();
                printf("functions table reinitialized\n");
            }
            else
            {
                try
                {
                    printf("%.17g\n",parser(string(a)));
                }
                catch(string errormsg)
                {
                    printf("%s\n",errormsg.c_str());
                }
            }
        }
        else break;
    }
    return EXIT_SUCCESS;
}
