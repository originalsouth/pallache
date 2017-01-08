#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "pallache.h"

typedef long double flt;
using namespace std;

int main(int argc,char *argv[])
{
    char a[2048];
    pallache::pallache<flt> parser;
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
            else if(!strcmp(a,"reset") or !strcmp(a,"clear"))
            {
                parser.reset();
                printf("functions and variables table reinitialized\n");
            }
            else if(!strcmp(a,"help"))
            {
                flt ans=parser["ans"];
                printf("pallache: A small and simple header only math parser library (written in C++14):\n");
                printf("\tbuiltin operators:\n");
                printf("\t\t+\taddition\n");
                printf("\t\t-\tsubstraction\n");
                printf("\t\t*\tmultiply\n");
                printf("\t\t/\tdivide\n");
                printf("\t\t**\texponantion\n");
                printf("\t\t%%\tmodulus\n");
                printf("\t\t!\tfactorial\n");
                printf("\t\t&&\tlogical and\n");
                printf("\t\t||\tlogical or\n");
                printf("\t\t^^\tlogical xor\n");
                printf("\t\t==\tlogical equal\n");
                printf("\t\t!=\tlogical inequal\n");
                printf("\t\t<\tsmaller than\n");
                printf("\t\t>\tlarger than\n");
                printf("\t\t<=\tsmaller  or equal than\n");
                printf("\t\t>=\tlarger or equal than\n");
                printf("\t\t&\tbitwise and\n");
                printf("\t\t|\tbitwise or\n");
                printf("\t\t^\tbitwise xor\n");
                printf("\tassignment operators:\n");
                printf("\t\t=\tdynamic assignment\n");
                printf("\t\t:=\tdynamic read only assignment\n");
                printf("\t\t=:\tstatic assignment\n");
                printf("\t\t::\tstatic read only assignment\n");
                printf("\tbuiltin variables:\n");
                printf("\t\tpi\t%.17Lg\n",parser["pi"]);
                printf("\t\te\t%.17Lg\n",parser["e"]);
                printf("\t\tphi\t%.17Lg\n",parser["phi"]);
                printf("\t\tnan\t%.17Lg\n",parser["nan"]);
                printf("\t\tinf\t%.17Lg\n",parser["inf"]);
                printf("\t\tminf\t%.17Lg\n",parser["minf"]);
                printf("\t\teps\t%.17Lg\n",parser["eps"]);
                printf("\t\trand_uniform_int\t%.17Lg\n",parser["rand_uniform_int"]);
                printf("\t\trand_uniform_real\t%.17Lg\n",parser["rand_uniform_real"]);
                printf("\t\trand_normal\t%.17Lg\n",parser["rand_normal"]);
                printf("\t\tans\t%.17Lg\n",ans);
                printf("\tbuiltin functions:\n");
                printf("\t\tcos, sin, tan\n");
                printf("\t\tacos, asin, atan, atan2\n");
                printf("\t\tcosh, sinh, tanh\n");
                printf("\t\tacosh, asinh, atanh\n");
                printf("\t\texp, exp2, expm1\n");
                printf("\t\tlog, log10, ilogb, log1p, log2, logb\n");
                printf("\t\tpow\n");
                printf("\t\tsqrt, cbrt\n");
                printf("\t\thypot\n");
                printf("\t\terf, erfc\n");
                printf("\t\ttgamma, lgamma\n");
                printf("\t\tceil, floor\n");
                printf("\t\tfmod\n");
                printf("\t\ttrunc, round\n");
                printf("\t\trint, nearbyint\n");
                printf("\t\tremainder\n");
                printf("\t\tabs \n");
                printf("\t\tsign, sgn\n");
                printf("\t\tbool, not\n");
                printf("\t\tdelta, kdelta\n");
                printf("\t\trand_uniform_int, rand_uniform_real, rand_normal\n");
                printf("\t\tdel\n");
                printf("\tbuiltin commands:\n");
                printf("\t\thelp, about, reset, clear\n");
                printf("\n");
                parser[pallache::to_string(ans)];
            }
            else if(!strcmp(a,"about"))
            {
                printf("pallache: A small and simple header only math parser library (written in C++14):\n");
                printf("\t/* By BC van Zuiden -- Leiden, November 2015 */\n");
                printf("\t/* Probably very buggy USE AT OWN RISK this will brick everything you own */\n");
                printf("\t/* NOBODY but YOU is liable for anything that happened in result of using this */\n");
                printf("\t/* WARNING: DON'T RUN THIS PROGRAM THIS WILL DESTROY YOUR COMPUTER AND/OR HOUSE */\n");
                printf("\t/* Any copyrighted piece of code within this code is NOT mine */\n");
                printf("\t/* Inclusion of that code is forced upon me by a scary anonymous guy with a gun*/\n");
                printf("Feel free to reuse and contribute, pull requests are very welcome! This code is (and forever will be) a work in progress.\n");
                printf("website: https://github.com/originalsouth/pallache\n");
            }
            else
            {
                try
                {
                    printf("%.17Lg\n",parser(string(a)));
                }
                catch(string errormsg)
                {
                    printf("%s in expression \"%s\"\n",errormsg.c_str(),a);
                }
            }
        }
        else break;
    }
    return EXIT_SUCCESS;
}
