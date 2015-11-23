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
    if(file) while(!feof(file))
    {
        printf(">>> ");
        fgets(a,2048,file);
        a[strcspn(a,"\n")]=0;
        if(strcmp(a,"q") and strcmp(a,"quit") and strcmp(a,"exit")) printf("%.16f\n",parser(string(a)));
        else break;
    }
    return EXIT_SUCCESS;
}
