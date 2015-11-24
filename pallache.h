/* By BC van Zuiden -- Leiden, November 2015 */
/* Probably very buggy USE AT OWN RISK this will brick everything you own */
/* NOBODY but YOU is liable for anything that happened in result of using this */
/* WARNING: DON'T RUN THIS PROGRAM THIS WILL DESTROY YOUR COMPUTER AND/OR HOUSE */
/* Any copyrighted piece of code within this code is NOT mine */
/* Inclusion of that code is forced upon me by a scary anonymous guy with a gun */
/* Thou shalt not remove this comments from this source */

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <limits>

#ifdef PALLACHE_DEBUG
#define PALLACHE_DEBUG_OUT(str,...)\
{\
    char buffer[2048];\
    const size_t n=snprintf(buffer,2048,"\033[1m\033[31mpallache-debug:\033[0m\033[1m\033[32m %s:%d \033[0min\033[37m %s: \033[0m",__FILE__,__LINE__,__func__);\
    snprintf(buffer+n,2048-n,str,##__VA_ARGS__);\
    fprintf(stderr,"%s\n",buffer);\
}
#else
#define PALLACHE_DEBUG_OUT(str,...) ;
#endif

namespace pallache
{
    template<class X> class pallache
    {
        private:
        enum types
        {
            bracket_open,
            bracket_close,
            number,
            variable,
            operators,
            function,
            comma,
        };
        struct token
        {
            int type;
            std::string str;
            token(int t,std::string a)
            {
                type=t;
                str=a;
            }
        };
        std::vector<token> tokens;
        std::unordered_map<std::string,X> variables;
        std::unordered_map<std::string,std::string> functions;
        void init()
        {
            variables.clear();
            variables["pi"]=std::acos(-1.0);
            variables["e"]=std::exp(1.0);
            variables["phi"]=0.5*(1+std::sqrt(5.0));
            variables["nan"]=std::numeric_limits<X>::quiet_NaN();
            variables["inf"]=std::numeric_limits<X>::infinity();
            variables["minf"]=-std::numeric_limits<X>::infinity();
            variables["eps"]=-std::numeric_limits<X>::epsilon();
            functions.clear();
            functions["cos"]="";
            functions["sin"]="";
            functions["tan"]="";
            functions["acos"]="";
            functions["asin"]="";
            functions["atan"]="";
            functions["atan2"]="";
            functions["cosh"]="";
            functions["sinh"]="";
            functions["tanh"]="";
            functions["acosh"]="";
            functions["asinh"]="";
            functions["atanh"]="";
            functions["exp"]="";
            functions["log"]="";
            functions["log10"]="";
            functions["exp2"]="";
            functions["expm1"]="";
            functions["ilogb"]="";
            functions["log1p"]="";
            functions["log2"]="";
            functions["logb"]="";
            functions["pow"]="";
            functions["sqrt"]="";
            functions["cbrt"]="";
            functions["hypot"]="";
            functions["erf"]="";
            functions["erfc"]="";
            functions["tgamma"]="";
            functions["lgamma"]="";
            functions["ceil"]="";
            functions["floor"]="";
            functions["fmod"]="";
            functions["trunc"]="";
            functions["round"]="";
            functions["rint"]="";
            functions["nearbyint"]="";
            functions["remainder"]="";
            functions["abs"]="";
            functions["sign"]="";
            functions["sgn"]="";
            functions["bool"]="";
            functions["delta"]="";
            functions["kdelta"]="";
            functions["not"]="";
            functions["delvar"]="";
        }
        X sign(X x)
        {
            if(x>0.0) return 1.0;
            else if(x<0.0) return -1.0;
            else return 0.0;
        }
        bool flt(char x)
        {
            if(isdigit(x)) return true;
            else if(x=='.') return true;
            else if(x=='e' or x=='E') return true;
            else return false;
        }
        bool op(char x)
        {
            const static std::string ops="+-*/^&|!=%:<>";
            const size_t opsSz=ops.size();
            for(size_t i=0;i<opsSz;i++) if(x==ops[i]) return true;
            return false;
        }
        int order(std::string a)
        {
            if(a=="**") return 0;
            else if(a=="!") return 0;
            else if(a=="*") return 1;
            else if(a=="%") return 1;
            else if(a=="/") return 1;
            else if(a=="+") return 2;
            else if(a=="-") return 2;
            else if(a=="&") return 3;
            else if(a=="^") return 5;
            else if(a=="|") return 6;
            else if(a=="&&") return 7;
            else if(a=="||") return 8;
            else return 9;
        }
        bool preproccess(std::string a)
        {
            if(a=="reset")
            {
                init();
                return false;
            }
            else return true;
        }
        void tokenize(std::string a)
        {
            tokens.clear();
            const size_t aSz=a.size();
            for(size_t i=0;i<aSz;)
            {
                size_t k=i+1;
                const size_t j=i;
                if(a[i]==' ' or a[i]=='\t')
                {
                    i++;
                    continue;
                }
                else if(isdigit(a[i]) or (a[i]=='-' and i+1<aSz and isdigit(a[i+1]) and (tokens.empty() or tokens.back().type!=types::number)))
                {
                    for(;k<aSz;k++) if(!flt(a[k]) and !((a[k]=='+' or a[k]=='-') and (a[k-1]=='e' or a[k-1]=='E'))) break;
                    tokens.push_back(token(types::number,a.substr(j,k-j)));
                    i+=k-j;
                }
                else if(isalpha(a[i]) or (a[i]=='-' and i+1<aSz and isalpha(a[i+1]) and (tokens.empty() or tokens.back().type!=types::number)))
                {
                    for(;k<aSz;k++) if(!isalnum(a[k]) and a[k]!='_') break;
                    std::string b=a.substr(j,k-j);
                    const int vtype=(functions.find(b)!=functions.end())?types::function:types::variable;
                    tokens.push_back(token(vtype,b));
                    i+=k-j;
                }
                else if(op(a[i]))
                {
                    for(;k<aSz;k++) if(!op(a[k]) or a[k]=='-') break;
                    tokens.push_back(token(types::operators,a.substr(j,k-j)));
                    i+=k-j;
                }
                else if(a[i]=='(')
                {
                    tokens.push_back(token(types::bracket_open,a.substr(j,k-j)));
                    i+=k-j;
                }
                else if(a[i]==')')
                {
                    tokens.push_back(token(types::bracket_close,a.substr(j,k-j)));
                    i+=k-j;
                }
                else if(a[i]==',')
                {
                    tokens.push_back(token(types::comma,a.substr(j,k-j)));
                    i+=k-j;
                }
                else break;
            }
            #ifdef PALLACHE_DEBUG
            PALLACHE_DEBUG_OUT("tokens");
            for(token x: tokens) PALLACHE_DEBUG_OUT("%s (%d)",x.str.c_str(),x.type);
            #endif
        }
        void shuntyard()
        {
            std::stack<token> stack;
            std::vector<token> train;
            for(token t: tokens) switch(t.type)
            {
                case types::number:
                {
                    train.push_back(t);
                }
                break;
                case types::variable:
                {
                    train.push_back(t);
                }
                break;
                case types::operators:
                {
                    if(!stack.empty() and stack.top().type==types::operators and order(t.str)>=order(stack.top().str))
                    {
                        train.push_back(stack.top());
                        stack.pop();
                    }
                    stack.push(t);
                }
                break;
                case types::function:
                {
                    stack.push(t);
                }
                break;
                case types::comma:
                {
                    int ptype=-1;
                    while(!stack.empty() and (ptype=stack.top().type)!=types::bracket_open)
                    {
                        train.push_back(stack.top());
                        stack.pop();
                    }
                    if(!stack.empty() and ptype!=types::bracket_open)
                    {
                        tokens.clear();
                        tokens.push_back(token(types::variable,"nan"));
                        return;
                    }
                }
                break;
                case types::bracket_open:
                {
                    stack.push(t);
                }
                break;
                case types::bracket_close:
                {
                    int ptype=-1;
                    while(!stack.empty() and (ptype=stack.top().type)!=types::bracket_open)
                    {
                        train.push_back(stack.top());
                        stack.pop();
                    }
                    if(!stack.empty() and ptype!=types::bracket_open)
                    {
                        tokens.clear();
                        tokens.push_back(token(types::variable,"nan"));
                        return;
                    }
                    stack.pop();
                }
                break;
                default:
                {
                    train.push_back(t);
                }
                break;
            }
            while(!stack.empty())
            {
                train.push_back(stack.top());
                if(stack.top().type==types::bracket_open)
                {
                    tokens.clear();
                    tokens.push_back(token(types::variable,"nan"));
                    return;
                }
                stack.pop();
            }
            tokens=train;
            #ifdef PALLACHE_DEBUG
            PALLACHE_DEBUG_OUT("tokens");
            for(token x: tokens) PALLACHE_DEBUG_OUT("%s (%d)",x.str.c_str(),x.type);
            #endif
        }
        X rpncalc()
        {
            std::string newvar="";
            if(tokens.empty()) return std::numeric_limits<X>::quiet_NaN();
            else if(tokens[0].type==types::variable and tokens.back().str=="=")
            {
                tokens.pop_back();
                newvar=tokens[0].str;
                tokens.erase(tokens.begin());
            }
            else if(tokens[0].type==types::variable and tokens.back().str=="delvar")
            {
                if(tokens.size()>2) return std::numeric_limits<X>::quiet_NaN();
                else
                {
                    if(variables.find(tokens[0].str)!=variables.end())
                    {
                        X var=variables[tokens[0].str];
                        variables.erase(tokens[0].str);
                        return var;
                    }
                    else return std::numeric_limits<X>::quiet_NaN();
                }

            }
            //TODO: implement function definitions
            std::vector<X> x;
            for(token t: tokens)
            {
                switch(t.type)
                {
                    case types::number:
                    {
                        x.push_back(std::stod(t.str)); //FIXME: not typesafe
                    }
                    break;
                    case types::variable:
                    {
                        X p;
                        if(t.str[0]=='-')
                        {
                            p=-1.0;
                            t.str.erase(0,1);
                        }
                        else p=1.0;
                        if(variables.find(t.str)!=variables.end()) x.push_back(p*variables[t.str]);
                        else x.push_back(std::numeric_limits<X>::quiet_NaN());
                    }
                    break;
                    case types::operators:
                    {
                        if(t.str=="+")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]+=x[q-1];
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="-")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]-=x[q-1];
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="*")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]*=x[q-1];
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="/")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]/=x[q-1];
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="%")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=fmod(x[q-2],x[q-1]);
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="**")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=pow(x[q-2],x[q-1]);
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="!")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::tgamma(x[q-1]+1.0);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="&&")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((bool)(x[q-2]) and (bool)(x[q-1]));
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="||")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((bool)(x[q-2]) or (bool)(x[q-1]));
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="^^")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((bool)(x[q-2]) xor (bool)(x[q-1]));
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="==")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])==(x[q-1]));
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="!=")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])!=(x[q-1]));
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="<")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])<(x[q-1]));
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str==">")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])>(x[q-1]));
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="<=")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])<=(x[q-1]));
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str==">=")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])>=(x[q-1]));
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="&")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((long int)(x[q-2])&(long int)(x[q-1]));
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="|")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((long int)(x[q-2])|(long int)(x[q-1]));
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="^")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((long int)(x[q-2])^(long int)(x[q-1]));
                                x.pop_back();
                            }
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                    }
                    break;
                    case types::function:
                    {
                        if(t.str=="cos")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::cos(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="sin")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::sin(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="tan")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::tan(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="acos")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::acos(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="asin")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::asin(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="atan")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::atan(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="atan2")
                        {
                            const size_t q=x.size();
                            if(q>1) x[q-2]=std::atan2(x[q-2],x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="cosh")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::cosh(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="sinh")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::sinh(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="tanh")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::tanh(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="acosh")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::acosh(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="asinh")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::asinh(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="atanh")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::atanh(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="exp")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::exp(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="log")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::log(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="log10")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::log10(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="exp2")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::exp2(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="expm1")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::expm1(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="ilogb")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::ilogb(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="log1p")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::log1p(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="log2")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::log2(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="logb")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::logb(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="pow")
                        {
                            const size_t q=x.size();
                            if(q>1) x[q-2]=std::pow(x[q-2],x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="sqrt")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::sqrt(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="cbrt")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::cbrt(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="hypot")
                        {
                            const size_t q=x.size();
                            if(q>1) x[q-2]=std::hypot(x[q-2],x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="erf")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::erf(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="erfc")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::erfc(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="tgamma")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::tgamma(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="lgamma")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::lgamma(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="ceil")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::ceil(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="floor")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::ceil(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="fmod")
                        {
                            const size_t q=x.size();
                            if(q>1) x[q-2]=std::fmod(x[q-2],x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="trunc")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::trunc(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="round")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::round(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="rint")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::nearbyint(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="nearbyint")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::nearbyint(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="remainder")
                        {
                            const size_t q=x.size();
                            if(q>1) x[q-2]=std::remainder(x[q-2],x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="abs")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::abs(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="sign")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=sign(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="sgn")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=sign(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="bool")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=(X)(bool)(x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="delta")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=x[q-1]==0.0?std::numeric_limits<X>::infinity():0.0;
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="kdelta")
                        {
                            const size_t q=x.size();
                            if(q>1) x[q-2]=(X)(x[q-2]==x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                        else if(t.str=="not")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=(X)(!(bool)x[q-1]);
                            else return std::numeric_limits<X>::quiet_NaN();
                        }
                    }
                    break;
                    default:
                    {
                        return std::numeric_limits<X>::quiet_NaN();
                    }
                    break;
                }
            }
            #ifdef PALLACHE_DEBUG
            PALLACHE_DEBUG_OUT("values");
            for(X f: x) PALLACHE_DEBUG_OUT("%f",f);
            #endif
            if(!newvar.empty())
            {
                variables[newvar]=(std::abs(x[0])<std::numeric_limits<X>::epsilon())?0.0:x[0];
                return variables[newvar];
            }
            else if(!x.empty()) return (std::abs(x[0])<std::numeric_limits<X>::epsilon())?0.0:x[0];
            else return std::numeric_limits<X>::quiet_NaN();
        }
        X parse_infix(std::string a)
        {
            if(preproccess(a))
            {
                tokenize(a);
                shuntyard();
                return rpncalc();
            }
            else return 0.0;
        }
        X parse_postfix(std::string a)
        {
            if(preproccess(a))
            {
                tokenize(a);
                return rpncalc();
            }
            else return 0.0;
        }
        public:
        pallache()
        {
            init();
        }
        X operator()(std::string a)
        {
            return parse_infix(a);
        }
        X operator[](std::string a)
        {
            return parse_postfix(a);
        }
    };
};
