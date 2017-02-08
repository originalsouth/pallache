#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "pallache.h"

typedef long double flt;
using namespace std;

struct parser_padding
{
    bool perror;
    pallache::pallache<flt> parser;
    parser_padding()
    {
        perror=true;
    }
    void reset()
    {
        parser.reset();
        printf("pallache: functions and variables table reinitialized\n");
    }
    void help()
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
        printf("\t\t:help, :about, :version, :reset, :clear, :exit\n");
        printf("\n");
        parser[pallache::to_string(ans)];
    }
    void about()
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
    void version()
    {
        #ifdef TATTOO
        printf("pallache: git-hash \t %s\n",TATTOO);
        #else
        printf("pallache: git-hash \t %s\n","undefined");
        #endif
        #ifdef SHASUM
        printf("pallache: source-hash \t %s\n",SHASUM);
        #else
        printf("pallache: source-hash \t %s\n","undefined");
        #endif
    }
    void proccess(string a)
    {
        try
        {
            printf("%.17Lg\n",parser(a));
        }
        catch(string errormsg)
        {
            if(perror) printf("%s in expression \"%s\"\n",errormsg.c_str(),a.c_str());
        }
    }
} parser;

int main(int argc,char *argv[])
{
    bool prompt=true;
    char a[2048];
    FILE *file=(argc>1)?fopen(argv[1],"r"):stdin;
    if(file) while(true)
    {
        if(prompt) printf(">>> ");
        fgets(a,2048,file);
        if(feof(file))
        {
            printf("exit\n");
            break;
        }
        a[strcspn(a,"\n")]=0;
        if(strcmp(a,":quit") and strcmp(a,":q") and strcmp(a,":exit") and strcmp(a,":x"))
        {
            if(a[0]=='#') continue;
            else if(!strcmp(a,"")) continue;
            else if(!strcmp(a,":reset") or !strcmp(a,":r") or !strcmp(a,":clear") or !strcmp(a,":c")) parser.reset();
            else if(!strcmp(a,":help") or !strcmp(a,":h")) parser.help();
            else if(!strcmp(a,":about") or !strcmp(a,":a")) parser.about();
            else if(!strcmp(a,":version") or !strcmp(a,":V")) parser.version();
            else if(!strcmp(a,":silent") or !strcmp(a,":s")) parser.perror=false;
            else if(!strcmp(a,":verbose") or !strcmp(a,":v")) parser.perror=true;
            else if(!strcmp(a,":unprompt") or !strcmp(a,":up")) prompt=false;
            else if(!strcmp(a,":prompt") or !strcmp(a,":p")) prompt=true;
            else parser.proccess(string(a));
        }
        else return EXIT_SUCCESS;
    }
    else
    {
        printf("pallache: file \"%s\" not found\n",argv[1]);
        return EXIT_FAILURE;
    }
}
