#include <iostream>
#include <map>
#include <string> 
#include <cctype>

using namespace std;

// Error namespace
namespace Error{
    int		no_of_errors;  //统计错误个数
    double error(const string& s);
}

double Error::error(const string& s)  // 打印出错信息，并统计出错次数 
{
    no_of_errors++;  
    cerr << "error: " << s << "\n";
    return 1;
}

//Lexer namespace
namespace Lexer{
    enum Token_value {   // token类型
        NAME, NUMBER, END, 
        PLUS = '+', MINUS = '-', MUL = '*', DIV = '/',
        PRINT = ';', ASSIGN = '=', LP = '(', RP = ')'
    };
    Token_value curr_tok = PRINT;  //当前token
    double		number_value;  //number的值
    string		string_value;  //string的值
    Token_value get_token();//读取下一个token

    using namespace Error;
}

Lexer::Token_value Lexer::get_token()  //读取下一个token
{
    char ch = 0;  //读取字符
    cin >> ch;
    switch (ch) {
        case 0:
        return curr_tok = END;  //结束
        case ';': 
        case '\n':
        return curr_tok = PRINT;  //打印
        case '*': 
        case '/': 
        case '+':
        case '-': 
        case '(': 
        case ')':
        case '=':
            return curr_tok = Token_value(ch);  //返回token类型
        case '0': case '1': case '2': case '3': case '4': case '5': 
        case '6': case '7': case '8': case '9': case '.':
            cin.putback(ch);  cin >> number_value;
            return curr_tok = NUMBER;  //返回number类型
        default:		// NAME, NAME = , or error
            if (isalpha(ch)) {
                string_value = ch;
                while(cin.get(ch) && isalnum(ch)) string_value.push_back(ch);
                cin.putback(ch); //cin >> string_value;
                return curr_tok = NAME;
            }
            error("bad token");
            return curr_tok = PRINT;
    }
}




// Symbol namespace
namespace Symbol{
    map<string, double>    table;   // 符号表 
}

// Parser namespace
namespace Parser{
    double prim(bool);  // 处理基本表达式
    double term(bool);  // 处理乘法和除法
    double expr(bool);  // 处理加法和减法

    using namespace Lexer;
    using namespace Error;
    using namespace Symbol;
}

double Parser::prim(bool get)  // 处理基本表达式
{
    if (get)   get_token();
    switch (curr_tok) 
    {
        case NUMBER:  // 处理number类型
            { 
                double v = number_value;
                get_token();  
                return v;
            }
        case NAME:  // 处理name类型
            { 
                double& v = table[string_value];
                if (get_token() == ASSIGN) v = expr(true);
                return v;
            }
        case MINUS:    // 处理负号
            return - prim(true);
        case LP:
            { 
                double e = expr(true);  // 处理括号
                if (curr_tok != RP) return error(") expected");
                get_token();  // eat ”)”
                return e;
            }
        default:
            return error("primary expected");  // 处理错误
    }
}

double Parser::term(bool get)  // multiply and divide
{
    double left = prim(get);
    for (;;)  // 无限循环
        switch (curr_tok) {
        case MUL:  // 处理乘法
            left *= prim(true);
            break;
        case DIV:
            if (double d = prim(true)) {  // 处理除法
            left /= d;
            break;
            }
            return error("divide by 0");  // 处理错误
        default:
            return left;
    }
}

double Parser::expr(bool get) {
    double left = term(get);
    for(;;) // 无限循环
    {
        switch (curr_tok) 
        {
            case PLUS:  // 处理加法
                left += term(true);
                break;
            case MINUS:  // 处理减法
                left -= term(true);
                break;
            default:
                return left;  // 处理错误   
        }    
    }
}


using namespace Lexer;
using namespace Error;
using namespace Symbol;
using namespace Parser;

int main()
{
    table["pi"] = 3.1415926535897932385;  // 初始化符号表
    table["e"]  = 2.7182818284590452354;
    while (cin) {
        get_token();  // 读取下一个token
        if (curr_tok == END) break;  // 结束
        if (curr_tok == PRINT) continue;  // 打印
        cout << expr(false) << '\n';  // 输出结果
    }
    return no_of_errors;  // 返回错误个数   
}
