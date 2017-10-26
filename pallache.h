/* By BC van Zuiden -- Leiden, November 2015 */
/* Probably very buggy USE AT OWN RISK this will brick everything you own */
/* NOBODY but YOU is liable for anything that happened in result of using this */
/* WARNING: DON'T RUN THIS PROGRAM THIS WILL DESTROY YOUR COMPUTER AND/OR HOUSE */
/* Any copyrighted piece of code within this code is NOT mine */
/* Inclusion of that code is forced upon me by a scary anonymous guy with a gun */
/* website: https://github.com/originalsouth/pallache */
/* Thou shalt not remove this comments from this source */

#if __cplusplus < 201103L
#error "pallache: C++ compiler too old requires at least C++11 to work (update compiler)."
#endif

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <limits>
#include <random>
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
    static constexpr char e6x9[]={0x74,0x68,0x65,0x5f,0x61,0x6e,0x73,
                                  0x77,0x65,0x72,0x5f,0x74,0x6f,0x5f,
                                  0x6c,0x69,0x66,0x65,0x5f,0x74,0x68,
                                  0x65,0x5f,0x75,0x6e,0x69,0x76,0x65,
                                  0x72,0x73,0x65,0x5f,0x61,0x6e,0x64,
                                  0x5f,0x65,0x76,0x65,0x72,0x79,0x74,
                                  0x68,0x69,0x6e,0x67,0x00,0x00,0x00};
    template<class X> std::string to_string(X x)
    {
        char buffer[2048];
        snprintf(buffer,2048,"%.17Lg",(long double)x);
        return std::string(buffer);
    }
    long double to_flt(std::string a)
    {
        long double value;
        try
        {
            value=std::stold(a);
        }
        catch(std::invalid_argument arg)
        {
            throw std::string("pallache: invalid argument error");
        }
        catch(std::out_of_range oerr)
        {
            throw std::string("pallache: overflow error");
        }
        return value;
    }
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
            bool protect;
            std::vector<std::string> var;
            std::vector<token> expr;
            functor(bool b=false,bool p=false)
            {
                builtin=b;
                protect=p;
            }
            size_t dim()
            {
                return var.size();
            }
            std::string p_expr()
            {
                std::string ret;
                for(token t:expr) ret+=t.str;
                return ret;
            }
            bool is_var(std::string a)
            {
                for(std::string v:var) if(a==v) return true;
                return false;
            }
            void substitute(size_t i,X x)
            {
                PALLACHE_DEBUG_OUT("%s = %.17Lg",var[i].c_str(),(long double)x);
                for(token &t:expr) if(t.str==var[i])
                {
                    t.type=types::number;
                    t.str=to_string(x);
                }
            }
        };
        struct random_t
        {
            std::mt19937 mt;
            random_t()
            {
                std::random_device dev;
                mt.seed(dev());
            }
            void seed(X seed)
            {
                mt.seed(seed);
            }
            X uniform_int(X a,X b)
            {
                std::uniform_int_distribution<int> uniform_int_dist((int)a,(int)b);
                return (X)uniform_int_dist(mt);
            }
            X uniform_real(X a,X b)
            {
                std::uniform_real_distribution<X> uniform_real_dist(a,b);
                return uniform_real_dist(mt);
            }
            X normal(X mu,X sigma)
            {
                std::normal_distribution<X> normal_dist(mu,sigma);
                return normal_dist(mt);
            }

        } random;
        X ans;
        std::unordered_map<std::string,functor> functions;
        void init()
        {
            ans=0.0L;
            functions.clear();
            functions.emplace("cos",functor(true,true));
            functions.emplace("sin",functor(true,true));
            functions.emplace("tan",functor(true,true));
            functions.emplace("acos",functor(true,true));
            functions.emplace("asin",functor(true,true));
            functions.emplace("atan",functor(true,true));
            functions.emplace("atan2",functor(true,true));
            functions.emplace("cosh",functor(true,true));
            functions.emplace("sinh",functor(true,true));
            functions.emplace("tanh",functor(true,true));
            functions.emplace("acosh",functor(true,true));
            functions.emplace("asinh",functor(true,true));
            functions.emplace("atanh",functor(true,true));
            functions.emplace("exp",functor(true,true));
            functions.emplace("log",functor(true,true));
            functions.emplace("log10",functor(true,true));
            functions.emplace("exp2",functor(true,true));
            functions.emplace("expm1",functor(true,true));
            functions.emplace("ilogb",functor(true,true));
            functions.emplace("log1p",functor(true,true));
            functions.emplace("log2",functor(true,true));
            functions.emplace("logb",functor(true,true));
            functions.emplace("pow",functor(true,true));
            functions.emplace("sqrt",functor(true,true));
            functions.emplace("cbrt",functor(true,true));
            functions.emplace("hypot",functor(true,true));
            functions.emplace("erf",functor(true,true));
            functions.emplace("erfc",functor(true,true));
            functions.emplace("tgamma",functor(true,true));
            functions.emplace("lgamma",functor(true,true));
            functions.emplace("ceil",functor(true,true));
            functions.emplace("floor",functor(true,true));
            functions.emplace("fmod",functor(true,true));
            functions.emplace("trunc",functor(true,true));
            functions.emplace("round",functor(true,true));
            functions.emplace("rint",functor(true,true));
            functions.emplace("nearbyint",functor(true,true));
            functions.emplace("remainder",functor(true,true));
            functions.emplace("abs",functor(true,true));
            functions.emplace("sign",functor(true,true));
            functions.emplace("sgn",functor(true,true));
            functions.emplace("bool",functor(true,true));
            functions.emplace("delta",functor(true,true));
            functions.emplace("kdelta",functor(true,true));
            functions.emplace("not",functor(true,true));
            functions.emplace("rand_uniform_int",functor(true,true));
            functions.emplace("rand_uniform_real",functor(true,true));
            functions.emplace("rand_normal",functor(true,true));
            functions.emplace("del",functor(true,true));
            functions.emplace("pi",functor(true,true));
            functions.emplace("e",functor(true,true));
            functions.emplace(std::string(e6x9),functor(true,true));
            functions.emplace("phi",functor(true,true));
            functions.emplace("nan",functor(true,true));
            functions.emplace("inf",functor(true,true));
            functions.emplace("minf",functor(true,true));
            functions.emplace("eps",functor(true,true));
            functions.emplace("ans",functor(true,true));
        }
        X sign(X x)
        {
            if(x>0.0L) return 1.0L;
            else if(x<0.0L) return -1.0L;
            else return 0.0L;
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
            static const std::string ops="+-*/^&|!=%:<>";
            const size_t opsSz=ops.size();
            for(size_t i=0;i<opsSz;i++) if(x==ops[i]) return true;
            return false;
        }
        int order(std::string a)
        {
            if(a=="!") return 1;
            else if(a=="**") return 2;
            else if(a=="*") return 3;
            else if(a=="%") return 3;
            else if(a=="/") return 3;
            else if(a=="+") return 4;
            else if(a=="-") return 4;
            else if(a=="&") return 5;
            else if(a=="^") return 7;
            else if(a=="|") return 8;
            else if(a=="&&") return 9;
            else if(a=="||") return 10;
            else if(a=="=" or a==":=" or a=="=:" or a=="::") return 11;
            else return 10;
        }
        bool test_digit(size_t i,const size_t aSz,std::string a,std::vector<token> &tokens)
        {
            if(isdigit(a[i])) return true;
            else if(i+1<aSz)
            {
                if(a[i]=='.' and isdigit(a[i+1])) return true;
                else if(a[i]=='-' and (isdigit(a[i+1]) or a[i+1]=='.') and (tokens.empty() or !(tokens.back().type==types::number or tokens.back().type==types::variable or tokens.back().type==types::function or tokens.back().type==types::bracket_close or tokens.back().str[0]=='!'))) return true;
                else return false;
            }
            else return false;
        }
        bool test_variable(size_t i,const size_t aSz,std::string a,std::vector<token> &tokens)
        {
            if(isalpha(a[i])) return true;
            else if(i+1<aSz)
            {
                if(a[i]=='-' and isalpha(a[i+1]) and (tokens.empty() or !(tokens.back().type==types::number or tokens.back().type==types::variable or tokens.back().type==types::function or tokens.back().type==types::bracket_close or tokens.back().str[0]=='!'))) return true;
                else return false;
            }
            else return false;
        }
        bool test_fact(size_t i,const size_t aSz,std::string a)
        {
            if(a[i]=='!')
            {
                size_t k=i+1;
                for(;k<aSz;k++) if(a[k]!='!') break;
                size_t l=0,m=k;
                for(;l<aSz;m++)
                {
                    if(a[m]=='=') l++;
                    else break;
                }
                if(l==1) k--;
                if(k-i==0) return false;
                else return true;
            }
            else return false;
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
                else if(test_digit(i,aSz,a,tokens))
                {
                    size_t dots=0;
                    size_t eE=0;
                    for(;k<aSz;k++)
                    {
                        if(a[k]=='.') dots++;
                        if(a[k]=='e' or a[k]=='E') eE++;
                        if((!flt(a[k]) and !((a[k]=='+' or a[k]=='-') and (a[k-1]=='e' or a[k-1]=='E'))) or dots>1 or eE>1) break;
                    }
                    tokens.push_back(token(types::number,a.substr(j,k-j)));
                    i+=k-j;
                }
                else if(test_variable(i,aSz,a,tokens))
                {
                    for(;k<aSz;k++) if(!isalnum(a[k]) and a[k]!='_') break;
                    std::string b=a.substr(j,k-j);
                    i+=k-j;
                    tokens.push_back(token(types::variable,b));
                }
                else if(test_fact(i,aSz,a))
                {
                    for(;k<aSz;k++) if(a[k]!='!') break;
                    size_t l=0,m=k;
                    for(;l<aSz;m++)
                    {
                        if(a[m]=='=') l++;
                        else break;
                    }
                    if(l==1) k--;
                    if(k-j!=0)
                    {
                        tokens.push_back(token(types::operators,a.substr(j,k-j)));
                        i+=k-j;
                    }
                }
                else if(op(a[i]))
                {
                    for(;k<aSz;k++) if(!op(a[k]) or a[k]=='-') break;
                    std::string b=a.substr(j,k-j);
                    tokens.push_back(token(types::operators,b));
                    if(b==":=" or b=="=" or b=="=:" or b=="::") tokens.push_back(token(types::internal,"internal"));
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
            const size_t tSz=tokens.size();
            if(tSz==0) throw std::string("pallache: input parsed empty");
            for(size_t i=0;i<tSz-1;i++) if(tokens[i].type==types::variable and tokens[i+1].type==types::bracket_open) tokens[i].type=types::function;
            #ifdef PALLACHE_DEBUG
            PALLACHE_DEBUG_OUT("tokens");
            for(token x:tokens) PALLACHE_DEBUG_OUT("%s (%d)",x.str.c_str(),x.type);
            #endif
        }
        bool test_op(token &t,std::stack<token> &stack)
        {
            if(stack.empty()) return false;
            else
            {
                if(stack.top().type==types::function) return true;
                else if(stack.top().type==types::operators)
                {
                    if(stack.top().str=="**" and t.str=="**") return false;
                    else if (order(t.str)>=order(stack.top().str)) return true;
                    else return false;
                }
                else return false;
            }
        }
        void shuntyard(std::vector<token> &tokens)
        {
            std::stack<token> stack;
            std::vector<token> train;
            for(token t:tokens) switch(t.type)
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
                    while(test_op(t,stack))
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
                    throw std::string("pallache: unknown types in syntax");
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
            for(token x:tokens) PALLACHE_DEBUG_OUT("%s (%d)",x.str.c_str(),x.type);
            #endif
        }
        X rpncalc(std::vector<token> &tokens)
        {
            if(tokens.empty()) throw std::string("pallache: syntax error");
            else if(tokens.back().str==":=" or tokens.back().str=="=" or tokens.back().str=="=:" or tokens.back().str=="::")
            {
                const size_t I=tokens.size();
                bool p=false;
                bool stat=(tokens.back().str.size()>1 and tokens.back().str[1]==':');
                functor f(false,tokens.back().str[0]==':'?true:false);
                tokens.pop_back();
                std::string fname;
                if(tokens[0].type==types::internal) throw std::string("pallache: cannot assign nothingness");
                for(size_t i=1;i<I;i++) if(tokens[i].type==types::internal)
                {
                    fname=tokens[i-1].str;
                    if(!(tokens[i-1].type==types::variable or tokens[i-1].type==types::function)) throw std::string("pallache: cannot assign \"")+fname+std::string("\" as it is invalid");
                    else if(fname[0]=='-')
                    {
                        p=true;
                        fname.erase(0,1);
                    }
                    f.expr=std::vector<token>(&tokens[i+1],&tokens[I-1]);
                    if(i>1) for(size_t j=0;j<i-1;j++)
                    {
                        if(tokens[j].type==types::variable) f.var.push_back(tokens[j].str);
                        else throw std::string("pallache: function \"")+fname+std::string("\" can only have variables as arguments");
                    }
                    break;
                }
                if(p)
                {
                    f.expr.push_back(token(types::number,"-1"));
                    f.expr.push_back(token(types::operators,"*"));
                }
                functor f_old=functor(false,false);
                if(functions.find(fname)!=functions.end())
                {
                    if(functions[fname].builtin) throw std::string("pallache: cannot change \"")+fname+std::string("\" because it is builtin");
                    else if(functions[fname].protect) throw std::string("pallache: cannot change \"")+fname+std::string("\" because it is protected delete it first");
                    else
                    {
                        f_old=functions[fname];
                        functions.erase(fname);
                    }
                }
                #ifdef PALLACHE_DEBUG
                PALLACHE_DEBUG_OUT("old function variables");
                for(std::string x:f_old.var) PALLACHE_DEBUG_OUT("%s",x.c_str());
                PALLACHE_DEBUG_OUT("old function exression");
                for(token x:f_old.expr) PALLACHE_DEBUG_OUT("%s (%d)",x.str.c_str(),x.type);
                PALLACHE_DEBUG_OUT("function variables");
                for(std::string x:f.var) PALLACHE_DEBUG_OUT("%s",x.c_str());
                PALLACHE_DEBUG_OUT("function exression");
                for(token x:f.expr) PALLACHE_DEBUG_OUT("%s (%d)",x.str.c_str(),x.type);
                #endif
                functions.emplace(fname,f);
                if(!stat)
                {
                    std::string fn;
                    std::unordered_set<std::string> set;
                    std::stack<std::string> stack;
                    stack.push(fname);
                    set.emplace(fname);
                    while(!stack.empty())
                    {
                        fn=stack.top();
                        stack.pop();
                        for(token t:functions[fn].expr)
                        {
                            if(functions[fn].is_var(t.str)) continue;
                            else if(functions[t.str].builtin) continue;
                            else if(t.str==fname or t.str=="-"+fname)
                            {
                                #ifdef PALLACHE_DEBUG
                                std::string expr=functions[fn].p_expr();
                                #endif
                                if(f_old.expr.size()) functions[fname]=f_old;
                                else functions.erase(fname);
                                #ifdef PALLACHE_DEBUG
                                throw std::string("pallache: unsupported recursion in function definition \"")+fname+std::string("\" detected in variable \"")+fn+std::string("=")+expr+std::string("\"");
                                #else
                                throw std::string("pallache: unsupported recursion in function definition \"")+fname+std::string("\" detected in variable \"")+fn+std::string("\"");
                                #endif
                            }
                            else if(functions.find(t.str)!=functions.end() and set.find(t.str)==set.end())
                            {
                                stack.push(t.str);
                                set.emplace(t.str);
                            }
                        }
                    }
                }
                const size_t J=f.dim();
                for(size_t j=0;j<J;j++) f.substitute(J-j-1,0.0L);
                #ifdef PALLACHE_DEBUG
                PALLACHE_DEBUG_OUT("substituted function exression");
                for(token x:f.expr) PALLACHE_DEBUG_OUT("%s (%d)",x.str.c_str(),x.type);
                #endif
                try
                {
                    ans=rpncalc(f.expr);
                }
                catch(std::string a)
                {
                    if(f_old.expr.size()) functions[fname]=f_old;
                    else functions.erase(fname);
                    throw std::string("pallache: error in function definition of \"")+fname+std::string("\" as a \"")+a+std::string("\" occured");
                }
                if(f.dim()==0 and stat)
                {
                    functions[fname].expr.clear();
                    functions[fname].expr.push_back(token(types::number,to_string(ans)));
                }
                else if(stat)
                {
                    bool cont;
                    for(token &t:functions[fname].expr) if(t.type==types::variable)
                    {
                        cont=false;
                        for(std::string var:functions[fname].var) if(t.str==var) cont=true;
                        if(cont) continue;
                        std::vector<token> expr;
                        expr.push_back(t);
                        t=token(types::number,to_string(rpncalc(expr)));
                    }
                }
                return ans;
            }
            else if((tokens[0].type==types::variable or tokens[0].type==types::function) and (tokens.back().str=="del" or tokens.back().str=="-del"))
            {
                if(tokens.size()>2) throw std::string("pallache: syntax error \"")+tokens.back().str+std::string("\" only takes one argument");
                else
                {
                    if(functions.find(tokens[0].str)!=functions.end())
                    {
                        if(!functions[tokens[0].str].builtin)
                        {
                            functor f=functions[tokens[0].str];
                            const size_t I=f.dim();
                            for(size_t i=0;i<I;i++) f.substitute(I-i-1,0.0);
                            try
                            {
                                ans=rpncalc(f.expr);
                            }
                            catch(std::string a)
                            {
                                (void) a;
                            }
                            functions.erase(tokens[0].str);
                            return tokens.back().str[0]!='-'?ans:-ans;
                        }
                        else throw std::string("pallache: cannot delete \"")+tokens[0].str+std::string("\" because it is builtin");
                    }
                    else throw std::string("pallache: cannot delete \"")+tokens[0].str+std::string("\" because it does not exist");
                }
            }
            std::vector<X> x;
            for(token t:tokens)
            {
                switch(t.type)
                {
                    case types::number:
                    {
                        x.push_back((X)to_flt(t.str));
                    }
                    break;
                    case types::variable:
                    {
                        X p=1.0L;
                        if(t.str[0]=='-')
                        {
                            p=-1.0L;
                            t.str.erase(0,1);
                        }
                        if(t.str=="pi") x.push_back(p*std::acos(-1.0L));
                        else if(t.str=="e") x.push_back(p*std::exp(1.0L));
                        else if(t.str=="phi") x.push_back(p*0.5L*(1.0L+std::sqrt(5.0L)));
                        else if(t.str==std::string(e6x9)) x.push_back(p*(X)052);
                        else if(t.str=="nan") x.push_back(p*std::numeric_limits<X>::quiet_NaN());
                        else if(t.str=="inf") x.push_back(p*std::numeric_limits<X>::infinity());
                        else if(t.str=="minf") x.push_back(-p*std::numeric_limits<X>::infinity());
                        else if(t.str=="eps") x.push_back(p*std::numeric_limits<X>::epsilon());
                        else if(t.str=="rand_uniform_int") x.push_back(p*random.uniform_int(0.0L,1.0L));
                        else if(t.str=="rand_uniform_real") x.push_back(p*random.uniform_real(0.0L,1.0L));
                        else if(t.str=="rand_normal") x.push_back(p*random.normal(0.0L,1.0L));
                        else if(t.str=="ans") x.push_back(p*ans);
                        else if(functions.find(t.str)!=functions.end())
                        {
                            functor f=functions[t.str];
                            x.push_back(p*rpncalc(f.expr));
                        }
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
                            else throw std::string("pallache: syntax error operator \"+\" requires two arguments");
                        }
                        else if(t.str=="-")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]-=x[q-1];
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error operator \"-\" requires two arguments");
                        }
                        else if(t.str=="*")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]*=x[q-1];
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error operator \"*\" requires two arguments");
                        }
                        else if(t.str=="/")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]/=x[q-1];
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error operator \"/\" requires two arguments");
                        }
                        else if(t.str=="%")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=fmod(x[q-2],x[q-1]);
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error operator \"%\" requires two arguments");
                        }
                        else if(t.str=="**")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=pow(x[q-2],x[q-1]);
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error operator \"**\" requires two arguments");
                        }
                        else if(t.str=="!")
                        {
                            const size_t q=x.size();
                            if(q>0) x[q-1]=std::tgamma(x[q-1]+1.0L);
                            else throw std::string("pallache: syntax error operator \"!\" requires one argument");
                        }
                        else if(t.str=="&&")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((bool)(x[q-2]) and (bool)(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error operator \"&&\" requires two arguments");
                        }
                        else if(t.str=="||")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((bool)(x[q-2]) or (bool)(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error operator \"||\" requires two arguments");
                        }
                        else if(t.str=="^^")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((bool)(x[q-2]) xor (bool)(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error operator \"^^\" requires two arguments");
                        }
                        else if(t.str=="==")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])==(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error operator \"==\" requires two arguments");
                        }
                        else if(t.str=="!=")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])!=(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error operator \"!=\" requires two arguments");
                        }
                        else if(t.str=="<")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])<(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error operator \"<\" requires two arguments");
                        }
                        else if(t.str==">")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])>(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error operator \">\" requires two arguments");
                        }
                        else if(t.str=="<=")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])<=(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error operator \"<=\" requires two arguments");
                        }
                        else if(t.str==">=")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((x[q-2])>=(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error operator \">=\" requires two arguments");
                        }
                        else if(t.str=="&")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((long int)(x[q-2])&(long int)(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error operator \"&\" requires two arguments");
                        }
                        else if(t.str=="|")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((long int)(x[q-2])|(long int)(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error operator \"|\" requires two arguments");
                        }
                        else if(t.str=="^")
                        {
                            const size_t q=x.size();
                            if(q>1)
                            {
                                x[q-2]=(X)((long int)(x[q-2])^(long int)(x[q-1]));
                                x.pop_back();
                            }
                            else throw std::string("pallache: syntax error operator \"^\" requires two arguments");
                        }
                        else throw std::string("pallache: operator \"")+t.str+std::string("\" is not defined");
                    }
                    break;
                    case types::function:
                    {
                        X p=1.0L;
                        if(t.str[0]=='-')
                        {
                            p=-1.0L;
                            t.str.erase(0,1);
                        }
                        if(functions[t.str].builtin)
                        {
                            if(t.str=="cos")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::cos(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="sin")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::sin(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="tan")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::tan(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="acos")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::acos(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="asin")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::asin(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="atan")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::atan(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="atan2")
                            {
                                const size_t q=x.size();
                                if(q>1)
                                {
                                    x[q-2]=p*std::atan2(x[q-2],x[q-1]);
                                    x.pop_back();
                                }
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 2");
                            }
                            else if(t.str=="cosh")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::cosh(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="sinh")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::sinh(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="tanh")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::tanh(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="acosh")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::acosh(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="asinh")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::asinh(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="atanh")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::atanh(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="exp")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::exp(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="log")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::log(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="log10")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::log10(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="exp2")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::exp2(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="expm1")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::expm1(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="ilogb")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::ilogb(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="log1p")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::log1p(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="log2")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::log2(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="logb")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::logb(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="pow")
                            {
                                const size_t q=x.size();
                                if(q>1)
                                {
                                    x[q-2]=p*std::pow(x[q-2],x[q-1]);
                                    x.pop_back();
                                }
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="sqrt")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::sqrt(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="cbrt")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::cbrt(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="hypot")
                            {
                                const size_t q=x.size();
                                if(q>1)
                                {
                                    x[q-2]=p*std::hypot(x[q-2],x[q-1]);
                                    x.pop_back();
                                }
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 2");
                            }
                            else if(t.str=="erf")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::erf(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="erfc")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::erfc(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="tgamma")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::tgamma(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="lgamma")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::lgamma(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="ceil")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::ceil(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="floor")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::floor(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="fmod")
                            {
                                const size_t q=x.size();
                                if(q>1)
                                {
                                    x[q-2]=p*std::fmod(x[q-2],x[q-1]);
                                    x.pop_back();
                                }
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 2");
                            }
                            else if(t.str=="trunc")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::trunc(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="round")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::round(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="rint")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::nearbyint(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="nearbyint")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::nearbyint(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="remainder")
                            {
                                const size_t q=x.size();
                                if(q>1)
                                {
                                    x[q-2]=p*std::remainder(x[q-2],x[q-1]);
                                    x.pop_back();
                                }
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 2");
                            }
                            else if(t.str=="abs")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*std::abs(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="sign")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*sign(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="sgn")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*sign(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="bool")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*(X)(bool)(x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="delta")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=p*(x[q-1]==0.0L?std::numeric_limits<X>::infinity():0.0L);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 1");
                            }
                            else if(t.str=="kdelta")
                            {
                                const size_t q=x.size();
                                if(q>1)
                                {
                                    x[q-2]=(X)p*(x[q-2]==x[q-1]);
                                    x.pop_back();
                                }
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 2");
                            }
                            else if(t.str=="not")
                            {
                                const size_t q=x.size();
                                if(q>0) x[q-1]=(X)p*(!(bool)x[q-1]);
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 2");
                            }
                            else if(t.str=="rand_uniform_int")
                            {
                                const size_t q=x.size();
                                if(q>1)
                                {
                                    if(x[q-2]<x[q-1]) x[q-2]=p*random.uniform_int(x[q-2],x[q-1]);
                                    else x[q-2]=p*random.uniform_int(x[q-1],x[q-2]);
                                    x.pop_back();
                                }
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 2");
                            }
                            else if(t.str=="rand_uniform_real")
                            {
                                const size_t q=x.size();
                                if(q>1)
                                {
                                    if(x[q-2]<x[q-1]) x[q-2]=p*random.uniform_real(x[q-2],x[q-1]);
                                    else x[q-2]=p*random.uniform_real(x[q-1],x[q-2]);
                                    x.pop_back();
                                }
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 2");
                            }
                            else if(t.str=="rand_normal")
                            {
                                const size_t q=x.size();
                                if(q>1)
                                {
                                    x[q-2]=p*random.normal(x[q-2],x[q-1]);
                                    x.pop_back();
                                }
                                else throw std::string("pallache: the function \"")+t.str+std::string("\" has dimesionality 2");
                            }
                            else throw std::string("pallache: error builtin function \"")+t.str+std::string("\" seems not to be defined");
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
                                #ifdef PALLACHE_DEBUG
                                PALLACHE_DEBUG_OUT("function");
                                for(token x:tokens) PALLACHE_DEBUG_OUT("%s (%d)",x.str.c_str(),x.type);
                                #endif
                                x.push_back(p*rpncalc(f.expr));
                            }
                            else throw std::string("pallache: the custom defined function \"")+t.str+std::string("\" has dimensionality ")+to_string(functions[t.str].dim());
                        }
                    }
                    break;
                    default:
                    {
                        throw std::string("pallache: the token \"")+t.str+std::string("\" is unknown");
                    }
                    break;
                }
            }
            #ifdef PALLACHE_DEBUG
            PALLACHE_DEBUG_OUT("values");
            for(X f:x) PALLACHE_DEBUG_OUT("%.17Lg",(long double)f);
            #endif
            if(x.size()==1)
            {
                ans=x[0];
                return ans;
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
            return (functions.find(a)!=functions.end() and functions[a].dim()==0);
        }
        bool func(std::string a)
        {
            return (functions.find(a)!=functions.end());
        }
        void var(std::string a,X x,bool protect=false)
        {
            if(protect) parse_infix(a+std::string(":=")+to_string(x));
            else parse_infix(a+std::string("=")+to_string(x));
        }
        void func(std::string a,std::string b,bool protect=false,bool dynamic=true)
        {
            if(protect)
            {
                if(dynamic) parse_infix(a+std::string(":=")+b);
                else parse_infix(a+std::string("::")+b);
            }
            else
            {
                if(dynamic) parse_infix(a+std::string("=")+b);
                else parse_infix(a+std::string("=:")+b);
            }
        }
        void del(std::string a)
        {
            if(var(a)) parse_infix(std::string("del(")+a+std::string(")"));
        }
        void reset()
        {
            init();
        }
    };
};
