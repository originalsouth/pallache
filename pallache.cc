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
            else if(!strcmp(a,"help"))
            {
                printf("Pallache: A small and simple header only math parser library (written in C++14):\n");
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
                printf("\tbuiltin variables:\n");
                printf("\t\tpi\t%f\n",parser["pi"]);
                printf("\t\te\t%f\n",parser["e"]);
                printf("\t\tphi\t%f\n",parser["phi"]);
                printf("\t\tnan\t%f\n",parser["nan"]);
                printf("\t\tinf\t%f\n",parser["inf"]);
                printf("\t\tminf\t%f\n",parser["minf"]);
                printf("\t\teps\t%f\n",parser["eps"]);
                printf("\tbuiltin functions:\n\t\t");
                printf("cos, ");
                printf("sin, ");
                printf("tan, ");
                printf("acos, ");
                printf("asin, ");
                printf("atan, ");
                printf("atan2, ");
                printf("cosh, ");
                printf("sinh, ");
                printf("tanh, ");
                printf("acosh, ");
                printf("asinh, ");
                printf("atanh, ");
                printf("exp, ");
                printf("log, ");
                printf("log10, ");
                printf("exp2, ");
                printf("expm1, ");
                printf("ilogb, ");
                printf("log1p, ");
                printf("log2, ");
                printf("logb, ");
                printf("pow, ");
                printf("sqrt, ");
                printf("cbrt, ");
                printf("hypot, ");
                printf("erf, ");
                printf("erfc, ");
                printf("tgamma, ");
                printf("lgamma, ");
                printf("ceil, ");
                printf("floor, ");
                printf("fmod, ");
                printf("trunc, ");
                printf("round, ");
                printf("rint, ");
                printf("nearbyint, ");
                printf("remainder, ");
                printf("abs, ");
                printf("sign, ");
                printf("sgn, ");
                printf("bool, ");
                printf("delta, ");
                printf("kdelta, ");
                printf("not, ");
                printf("delvar, ");
                printf("delfunc, ");
                printf("\n");
            }
            else if(!strcmp(a,"about"))
            {
                printf("Pallache: A small and simple header only math parser library (written in C++14):\n");
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
