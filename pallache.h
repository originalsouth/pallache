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
#include <typeinfo>

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
            internal,
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
        struct functor
        {
            bool builtin;
            std::vector<std::string> var;
            std::vector<token> expr;
            functor(bool p=false)
            {
                builtin=p;
            }
            size_t dim()
            {
                return var.size();
            }
            void substitute(size_t i,X x)
            {
                PALLACHE_DEBUG_OUT("%s",var[i].c_str());
                for(token &t: expr) if(t.str==var[i]) t.str=std::to_string(x);
            }
        };
        std::unordered_map<std::string,X> variables;
        std::unordered_map<std::string,functor> functions;
        void init_var()
        {
            const char e6x9[]={0x74,0x68,0x65,0x5f,0x61,0x6e,0x73,
                               0x77,0x65,0x72,0x5f,0x74,0x6f,0x5f,
                               0x6c,0x69,0x66,0x65,0x5f,0x74,0x68,
                               0x65,0x5f,0x75,0x6e,0x69,0x76,0x65,
                               0x72,0x73,0x65,0x5f,0x61,0x6e,0x64,
                               0x5f,0x65,0x76,0x65,0x72,0x79,0x74,
                               0x68,0x69,0x6e,0x67,0x00,0x00,0x00};
            variables.clear();
            variables.emplace("pi",std::acos(-1.0));
            variables.emplace("e",std::exp(1.0));
            variables.emplace("phi",0.5*(1+std::sqrt(5.0)));
            variables.emplace(std::string(e6x9),(X)052);
            variables.emplace("nan",std::numeric_limits<X>::quiet_NaN());
            variables.emplace("inf",std::numeric_limits<X>::infinity());
            variables.emplace("minf",-std::numeric_limits<X>::infinity());
            variables.emplace("eps",std::numeric_limits<X>::epsilon());
        }
        void init_func()
        {
            functions.clear();
            functions.emplace("cos",functor(true));
            functions.emplace("sin",functor(true));
            functions.emplace("tan",functor(true));
            functions.emplace("acos",functor(true));
            functions.emplace("asin",functor(true));
            functions.emplace("atan",functor(true));
            functions.emplace("atan2",functor(true));
            functions.emplace("cosh",functor(true));
            functions.emplace("sinh",functor(true));
            functions.emplace("tanh",functor(true));
            functions.emplace("acosh",functor(true));
            functions.emplace("asinh",functor(true));
            functions.emplace("atanh",functor(true));
            functions.emplace("exp",functor(true));
            functions.emplace("log",functor(true));
            functions.emplace("log10",functor(true));
            functions.emplace("exp2",functor(true));
            functions.emplace("expm1",functor(true));
            functions.emplace("ilogb",functor(true));
            functions.emplace("log1p",functor(true));
            functions.emplace("log2",functor(true));
            functions.emplace("logb",functor(true));
            functions.emplace("pow",functor(true));
            functions.emplace("sqrt",functor(true));
            functions.emplace("cbrt",functor(true));
            functions.emplace("hypot",functor(true));
            functions.emplace("erf",functor(true));
            functions.emplace("erfc",functor(true));
            functions.emplace("tgamma",functor(true));
            functions.emplace("lgamma",functor(true));
            functions.emplace("ceil",functor(true));
            functions.emplace("floor",functor(true));
            functions.emplace("fmod",functor(true));
            functions.emplace("trunc",functor(true));
            functions.emplace("round",functor(true));
            functions.emplace("rint",functor(true));
            functions.emplace("nearbyint",functor(true));
            functions.emplace("remainder",functor(true));
            functions.emplace("abs",functor(true));
            functions.emplace("sign",functor(true));
            functions.emplace("sgn",functor(true));
            functions.emplace("bool",functor(true));
            functions.emplace("delta",functor(true));
            functions.emplace("kdelta",functor(true));
            functions.emplace("not",functor(true));
            functions.emplace("delvar",functor(true));
            functions.emplace("delfunc",functor(true));
            functions.emplace("test",functor(false));
            functions["test"].expr.push_back(token(types::number,"x"));
            functions["test"].expr.push_back(token(types::number,"y"));
            functions["test"].expr.push_back(token(types::operators,"**"));
            functions["test"].var.push_back("x");
            functions["test"].var.push_back("y");
        }
        void init()
        {
            init_var();
            init_func();
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
            if(a=="!") return 0;
            if(a=="**") return 1;
            else if(a=="*") return 2;
            else if(a=="%") return 2;
            else if(a=="/") return 2;
            else if(a=="+") return 3;
            else if(a=="-") return 3;
            else if(a=="&") return 4;
            else if(a=="^") return 6;
            else if(a=="|") return 7;
            else if(a=="&&") return 8;
            else if(a=="||") return 9;
            else return 10;
        }
        void tokenize(std::string a,std::vector<token> &tokens)
        {
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
                else if(isdigit(a[i]) or (a[i]=='-' and i+1<aSz and isdigit(a[i+1]) and (tokens.empty() or !(tokens.back().type==types::number or tokens.back().type==types::variable or tokens.back().str[0]=='!'))))
                {
                    for(;k<aSz;k++) if(!flt(a[k]) and !((a[k]=='+' or a[k]=='-') and (a[k-1]=='e' or a[k-1]=='E'))) break;
                    tokens.push_back(token(types::number,a.substr(j,k-j)));
                    i+=k-j;
                }
                else if(isalpha(a[i]) or (a[i]=='-' and i+1<aSz and isalpha(a[i+1]) and (tokens.empty() or !(tokens.back().type==types::number or tokens.back().type==types::variable or tokens.back().str[0]=='!'))))
                {
                    for(;k<aSz;k++) if(!isalnum(a[k]) and a[k]!='_') break;
                    std::string b=a.substr(j,k-j);
                    const int vtype=(functions.find(b)!=functions.end())?types::function:types::variable;
                    tokens.push_back(token(vtype,b));
                    i+=k-j;
                }
                else if(a[i]=='!')
                {
                    for(;k<aSz;k++) if(a[k]!='!') break;
                    tokens.push_back(token(types::operators,a.substr(j,k-j)));
                    i+=k-j;
                }
                else if(op(a[i]))
                {
                    for(;k<aSz;k++) if(!op(a[k]) or a[k]=='-') break;
                    std::string b=a.substr(j,k-j);
                    tokens.push_back(token(types::operators,b));
                    if(b==":=") tokens.push_back(token(types::internal,""));
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
                else throw std::string("pallache: unknown token \"")+a[i]+std::string("\"");
            }
            #ifdef PALLACHE_DEBUG
            PALLACHE_DEBUG_OUT("tokens");
            for(token x: tokens) PALLACHE_DEBUG_OUT("%s (%d)",x.str.c_str(),x.type);
            #endif
        }
        void shuntyard(std::vector<token> &tokens)
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
                    if(!stack.empty() and ptype!=types::bracket_open) throw std::string("pallache: unmatched bracket");
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
                    if(ptype!=types::bracket_open) throw std::string("pallache: unmatched bracket");
                    stack.pop();
                }
                break;
                case types::internal:
                {
                    train.push_back(t);
                }
                break;
                default:
                {
                    throw std::string("pallache: syntax error");
                }
                break;
            }
            while(!stack.empty())
            {
                train.push_back(stack.top());
                if(stack.top().type==types::bracket_open) throw std::string("pallache: unmatched bracket");
                stack.pop();
            }
            tokens=train;
            #ifdef PALLACHE_DEBUG
            PALLACHE_DEBUG_OUT("tokens");
            for(token x: tokens) PALLACHE_DEBUG_OUT("%s (%d)",x.str.c_str(),x.type);
            #endif
        }
        X rpncalc(std::vector<token> &tokens)
        {
            std::string newvar="";
            if(tokens.empty()) throw std::string("pallache: syntax error");
            else if(tokens[0].type==types::variable and tokens.back().str=="=")
            {
                tokens.pop_back();
                newvar=tokens[0].str;
                tokens.erase(tokens.begin());
            }
            else if(tokens[0].type==types::variable and tokens.back().str==":=")
            {
                functor f(false);
                tokens.pop_back();
                std::string fname=tokens[0].str;
                const size_t I=tokens.size();
                for(size_t i=1;i<I;i++)
                {
                    if(tokens[i].type==types::variable) f.var.push_back(tokens[i].str);
                    else if(tokens[i].type==types::internal)
                    {
                        i++;
                        for(;i<I;i++)
                        {
                            f.expr.push_back(tokens[i]);
                            for(std::string var: f.var) if(f.expr.back().str==var) f.expr.back().type=types::number;
                        }
                        functions.emplace(fname,f);
                        const size_t J=f.dim();
                        for(size_t j=0;j<J;j++) f.substitute(J-j-1,0.0);
                        variables["ans"]=rpncalc(f.expr);
                        return variables["ans"];
                    }
                    else throw std::string("pallache: in function definition of ")+fname+std::string(" ")+tokens[i].str+std::string(" is not a valid variable");
                }
                throw std::string("pallache: error in function definition of ")+fname;
            }
            else if(tokens[0].type==types::variable and tokens.back().str=="delvar")
            {
                if(tokens.size()>2) throw std::string("pallache: syntax error");
                else
                {
                    if(variables.find(tokens[0].str)!=variables.end())
                    {
                        X var=variables[tokens[0].str];
                        variables.erase(tokens[0].str);
                        variables["ans"]=var;
                        return var;
                    }
                    else throw std::string("pallache: variable \"")+tokens[0].str+std::string("\" does not exist");
                }
            }
            else if(tokens[0].type==types::function and tokens.back().str=="delfunc")
            {
                if(tokens.size()>2) throw std::string("pallache: syntax error");
                else
                {
                    if(functions.find(tokens[0].str)!=functions.end())
                    {
                        if(!functions[tokens[0].str].builtin)
                        {
                            functions.erase(tokens[0].str);
                            return variables["ans"];
                        }
                        else throw std::string("pallache: function \"")+tokens[0].str+std::string("\" is builtin");
                    }
                    else throw std::string("pallache: function \"")+tokens[0].str+std::string("\" does not exist");
                }
            }
            std::vector<X> x;
            for(token t: tokens)
            {
                switch(t.type)
                {
                    case types::number:
                    {
                        if(typeid(X)==typeid(float)) x.push_back(std::stof(t.str));
                        else if(typeid(X)==typeid(double)) x.push_back(std::stod(t.str));
                        else if(typeid(X)==typeid(long double)) x.push_back(std::stold(t.str));
                        else x.push_back((X)std::stold(t.str));
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
                        else throw std::string("pallache: variable \"")+t.str+std::string("\" does not exist");
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
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str=="-")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]-=x[q-1];
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str=="*")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]*=x[q-1];
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str=="/")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]/=x[q-1];
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str=="%")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=fmod(x[q-2],x[q-1]);
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str=="**")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=pow(x[q-2],x[q-1]);
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str=="!")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::tgamma(x[q-1]+1.0);
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str=="&&")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((bool)(x[q-2]) and (bool)(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str=="||")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((bool)(x[q-2]) or (bool)(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str=="^^")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((bool)(x[q-2]) xor (bool)(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str=="==")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])==(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str=="!=")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])!=(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str=="<")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])<(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str==">")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])>(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str=="<=")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])<=(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str==">=")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])>=(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str=="&")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((long int)(x[q-2])&(long int)(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str=="|")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((long int)(x[q-2])|(long int)(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error");
                        }
                        else if(t.str=="^")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((long int)(x[q-2])^(long int)(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error");
                        }
                        else throw std::string("pallache: invalid operator \"")+t.str+std::string("\"");
                    }
                    break;
                    case types::function:
                    {
                        if(functions[t.str].builtin)
                        {
                            if(t.str=="cos")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::cos(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="sin")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::sin(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="tan")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::tan(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="acos")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::acos(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="asin")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::asin(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="atan")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::atan(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="atan2")
                            {
                                const size_t q=x.size();
                                if(q>1)
                                {
                                    x[q-2]=std::atan2(x[q-2],x[q-1]);
                                    x.pop_back();
                                }
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="cosh")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::cosh(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="sinh")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::sinh(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="tanh")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::tanh(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="acosh")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::acosh(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="asinh")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::asinh(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="atanh")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::atanh(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="exp")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::exp(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="log")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::log(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="log10")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::log10(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="exp2")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::exp2(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="expm1")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::expm1(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="ilogb")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::ilogb(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="log1p")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::log1p(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="log2")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::log2(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="logb")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::logb(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="pow")
                            {
                                const size_t q=x.size();
                                if(q>1)
                                {
                                    x[q-2]=std::pow(x[q-2],x[q-1]);
                                    x.pop_back();
                                }
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="sqrt")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::sqrt(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="cbrt")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::cbrt(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="hypot")
                            {
                                const size_t q=x.size();
                                if(q>1)
                                {
                                    x[q-2]=std::hypot(x[q-2],x[q-1]);
                                    x.pop_back();
                                }
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="erf")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::erf(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="erfc")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::erfc(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="tgamma")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::tgamma(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="lgamma")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::lgamma(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="ceil")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::ceil(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="floor")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::ceil(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="fmod")
                            {
                                const size_t q=x.size();
                                if(q>1)
                                {
                                    x[q-2]=std::fmod(x[q-2],x[q-1]);
                                    x.pop_back();
                                }
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="trunc")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::trunc(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="round")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::round(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="rint")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::nearbyint(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="nearbyint")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::nearbyint(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="remainder")
                            {
                                const size_t q=x.size();
                                if(q>1)
                                {
                                    x[q-2]=std::remainder(x[q-2],x[q-1]);
                                    x.pop_back();
                                }
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="abs")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=std::abs(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="sign")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=sign(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="sgn")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=sign(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="bool")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=(X)(bool)(x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="delta")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=x[q-1]==0.0?std::numeric_limits<X>::infinity():0.0;
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="kdelta")
                            {
                                const size_t q=x.size();
                                if(q>1)
                                {
                                    x[q-2]=(X)(x[q-2]==x[q-1]);
                                    x.pop_back();
                                }
                                else throw std::string("pallache: syntax error");
                            }
                            else if(t.str=="not")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=(X)(!(bool)x[q-1]);
                                else throw std::string("pallache: syntax error");
                            }
                            else throw std::string("pallache: function definition error");
                        }
                        else
                        {
                            if(x.size()>=functions[t.str].dim())
                            {
                                functor f=functions[t.str];
                                const size_t q=x.size();
                                const size_t I=f.dim();
                                for(size_t i=0;i<I;i++)
                                {
                                    f.substitute(I-i-1,x[q-i-1]);
                                    x.pop_back();
                                }
                                for(token x: tokens) PALLACHE_DEBUG_OUT("%s (%d)",x.str.c_str(),x.type);
                                x.push_back(rpncalc(f.expr));
                            }
                            else throw std::string("pallache: syntax error");
                        }
                    }
                    break;
                    default:
                    {
                        throw std::string("pallache: syntax error");
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
                variables["ans"]=variables[newvar]=(std::abs(x[0])<std::numeric_limits<X>::epsilon())?0.0:x[0];
                return variables["ans"];
            }
            else if(x.size()==1)
            {
                variables["ans"]=(std::abs(x[0])<std::numeric_limits<X>::epsilon())?0.0:x[0];
                return variables["ans"];
            }
            else throw std::string("pallache: syntax error");
        }
        X parse_infix(std::string a)
        {
            std::vector<token> tokens;
            tokenize(a,tokens);
            shuntyard(tokens);
            return rpncalc(tokens);
        }
        X parse_postfix(std::string a)
        {
            std::vector<token> tokens;
            tokenize(a,tokens);
            return rpncalc(tokens);
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
        bool var(std::string a)
        {
            return (variables.find(a)!=variables.end());
        }
        bool func(std::string a)
        {
            return (function.find(a)!=function.end());
        }
        void var(std::string a,X x)
        {
            variables[a]=x;
        }
        void func(std::string a,std::string b)
        {
            function[a]=b;
        }
        void del_var(std::string a)
        {
            variables.erase(a);
        }
        void del_func(std::string a)
        {
            function.erase(a);
        }
        void reset_vars()
        {
            init_var();
        }
        void reset_funcs()
        {
            init_func();
        }
        void reset_all()
        {
            init();
        }
    };
};
