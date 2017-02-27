#define BufSz 4096

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "pallache.h"

typedef long double flt;
using namespace std;

char* strtokar(char *source,char delim,unsigned int pos)
{
    static char dest[BufSz];
    unsigned int i=0,dlim=0;
    for(;dlim<pos;i++)
    {
        if(source[i]==delim) dlim++;
        else if(source[i]==0)
        {
            dest[0]=0;
            return dest;
        }
    }
    strcpy(dest,&source[i]);
    for(i=0;dest[i]>0;i++) if(dest[i]==delim) dest[i]=0;
    return dest;
}

struct parser_padding
{
    bool perror;
    bool prompt;
    bool interfaced;
    pallache::pallache<flt> parser;
    parser_padding()
    {
        perror=true;
        prompt=true;
        interfaced=false;
    }
    void reset()
    {
        parser.reset();
        printf("pallache: functions and variables table reinitialized\n");
    }
    void help()
    {
        flt ans=parser["ans"];
        printf("pallache: A small and simple header only math parser library:\n");
        printf("\tbuilt-in operators:\n");
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
        printf("\tbuilt-in variables:\n");
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
        printf("\tbuilt-in functions:\n");
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
        printf("\tbuilt-in commands:\n");
        printf("\t\t:quit (:q), :exit (:x)\n");
        printf("\t\t:help (:h)\n");
        printf("\t\t:about (:a)\n");
        printf("\t\t:version (:V)\n");
        printf("\t\t:silent (:s)\n");
        printf("\t\t:verbose (:v) (default)\n");
        printf("\t\t:unprompt (:u)\n");
        printf("\t\t:prompt (:p) (default)\n");
        printf("\tcommand line: pallache [argmument1] [argument2] [...]\n");
        printf("\tcommand line arguments:\n");
        printf("\t\t[file]\n");
        printf("\t\t- (stdin) (default)\n");
        printf("\t\t--reset (-r)\n");
        printf("\t\t--help (-h)\n");
        printf("\t\t--about (-a)\n");
        printf("\t\t--version (-V)\n");
        printf("\t\t--silent (-s)\n");
        printf("\t\t--verbose (-v) (default)\n");
        printf("\t\t--unprompt (-u)\n");
        printf("\t\t--prompt (-p) (default)\n");
        printf("\t\t--calc (-c) \"expr1;expr2;...\"\n");
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
    bool pre_proccess(char *a)
    {
        if(strcmp(a,":quit") and strcmp(a,":q") and strcmp(a,":exit") and strcmp(a,":x"))
        {
            if(a[0]=='#') return true;
            else if(!strcmp(a,"")) return true;
            else if(!strcmp(a,":reset") or !strcmp(a,":r") or !strcmp(a,":clear") or !strcmp(a,":c")) reset();
            else if(!strcmp(a,":help") or !strcmp(a,":h")) help();
            else if(!strcmp(a,":about") or !strcmp(a,":a")) about();
            else if(!strcmp(a,":version") or !strcmp(a,":V")) version();
            else if(!strcmp(a,":silent") or !strcmp(a,":s")) perror=false;
            else if(!strcmp(a,":verbose") or !strcmp(a,":v")) perror=true;
            else if(!strcmp(a,":unprompt") or !strcmp(a,":u")) prompt=false;
            else if(!strcmp(a,":prompt") or !strcmp(a,":p")) prompt=true;
            else proccess(string(a));
        }
        else return false;
        return true;
    }
    void line_proccess(char *a)
    {
        char *b;
        interfaced=true;
        size_t I=0,A=strlen(a);
        for(size_t i=0;i<A;i++) if(a[i]==';') I++;
        if(I) for(size_t i=0;i<=I;i++)
        {
            b=strtokar(a,';',i);
            if(pre_proccess(b)) continue;
            else exit(EXIT_SUCCESS);
        }
        else if(!pre_proccess(a)) exit(EXIT_SUCCESS);
    }
    void file_proccess(FILE *file)
    {
        interfaced=true;
        char a[BufSz];
        if(file) while(true)
        {
            if(prompt and file==stdin) printf(">>> ");
            fgets(a,BufSz,file);
            if(feof(file))
            {
                if(prompt) printf(":x\n");
                break;
            }
            a[strcspn(a,"\n")]=0;
            if(prompt and file!=stdin and a[0]!='#') printf(">>> %s\n",a);
            line_proccess(a);
        }
    }
} parser;

int main(int argc,char *argv[])
{
    if(argc==1) parser.file_proccess(stdin);
    else
    {
        for(int i=1;i<argc;i++)
        {
            if(argv[i][0]=='-')
            {
                if(!strcmp(argv[i],"-")) parser.file_proccess(stdin);
                else if(argv[i][1]=='r' or (argv[i][1]=='-' and !strcmp(&argv[i][2],"reset"))) parser.reset();
                else if(argv[i][1]=='h' or (argv[i][1]=='-' and !strcmp(&argv[i][2],"help"))) parser.help(),parser.interfaced=true;
                else if(argv[i][1]=='a' or (argv[i][1]=='-' and !strcmp(&argv[i][2],"about"))) parser.about(),parser.interfaced=true;
                else if(argv[i][1]=='V' or (argv[i][1]=='-' and !strcmp(&argv[i][2],"version"))) parser.version(),parser.interfaced=true;
                else if(argv[i][1]=='s' or (argv[i][1]=='-' and !strcmp(&argv[i][2],"silent"))) parser.perror=false;
                else if(argv[i][1]=='v' or (argv[i][1]=='-' and !strcmp(&argv[i][2],"verbose"))) parser.perror=true;
                else if(argv[i][1]=='u' or (argv[i][1]=='-' and !strcmp(&argv[i][2],"unprompt"))) parser.prompt=false;
                else if(argv[i][1]=='p' or (argv[i][1]=='-' and !strcmp(&argv[i][2],"prompt"))) parser.prompt=true;
                else if(argv[i][1]=='c' or (argv[i][1]=='-' and !strcmp(&argv[i][2],"calc"))) parser.line_proccess(argv[++i]);
                else
                {
                    printf("pallache: \"%s\" is not a supported argument\n",argv[i]);
                    continue;
                }
            }
            else
            {
                FILE *file=fopen(argv[i],"r");
                if(!file)
                {
                    printf("pallache: unable to open input file \"%s\"\n",argv[i]);
                    continue;
                }
                parser.file_proccess(file);
            }
        }
    }
    if(!parser.interfaced) parser.file_proccess(stdin);
    return EXIT_SUCCESS;
}
