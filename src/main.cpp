#include <iostream>
#include <stack>
#include <cmath>
#include <memory>
#include <vector>
#include <string>
#include <cassert>
#include <map>

namespace calc
{
    enum TokenType
    {
        TOKEN_ID,
        TOKEN_DIGIT,
        TOKEN_OP,
        TOKEN_BRACE,
    };

    struct Token
    {
        std::string value;
        TokenType type;
    };

    class analyser
    {
        std::string source;
        char current;

        char advance() { if(cursor < length) { char tmp = current; current = source[++cursor]; return tmp; } else { return '\0'; } }

        std::shared_ptr<Token> tokenize_word()
        {
            std::shared_ptr<Token> tp = std::make_shared<Token>();

            tp->type = TOKEN_ID;

            while(isalnum(current))
            {
                tp->value.push_back(advance());
            }

            return tp;
        }

        std::shared_ptr<Token> tokenize_digit()
        {
            std::shared_ptr<Token> tp = std::make_shared<Token>();

            tp->type = TOKEN_DIGIT;

            while(isdigit(current) || current == '.')
            {
                tp->value.push_back(advance());
            }

            return tp;
        }

        std::shared_ptr<Token> tokenize_special(TokenType t, std::string s = "\0")
        {
            std::shared_ptr<Token> tp = std::make_shared<Token>();

            tp->value = s;
            tp->type = t;

            return tp;
        }

        public:
        std::vector<std::shared_ptr<Token>> tokens;
        unsigned long int cursor;
        unsigned long int length;

        analyser(std::string expr)
        {
            source = expr;
            cursor = 0;
            current = expr.at(cursor);
            length = expr.length();
        }

        std::vector<std::shared_ptr<Token>> analyse()
        {
            while(cursor < length)
            {
                if(isalpha(current))
                {
                    tokens.push_back(tokenize_word());
                    continue;
                }
                else if(isdigit(current))
                {
                    tokens.push_back(tokenize_digit());
                    continue;
                }
                switch (current)
                {
                case ')':
                    tokens.push_back(tokenize_special(TOKEN_BRACE, std::string(1, advance())));
                    break;

                case '(':
                    tokens.push_back(tokenize_special(TOKEN_BRACE, std::string(1, advance())));
                    break;

                case '+':
                    tokens.push_back(tokenize_special(TOKEN_OP, std::string(1, advance())));
                    break;

                case '-':
                    tokens.push_back(tokenize_special(TOKEN_OP, std::string(1, advance())));
                    break;

                case '/':
                    tokens.push_back(tokenize_special(TOKEN_OP, std::string(1, advance())));
                    break;

                case '*':
                    tokens.push_back(tokenize_special(TOKEN_OP, std::string(1, advance())));
                    break;
                case ' ':
                    advance();
                    break;

                case '\t':
                    advance();
                    break;

                default:
                    assert("Unexpected symbol\n");
                    break;
                }
            }
/*
            for(auto a : tokens)
            {
                printf("token %s - %i\n", a->value.data(), a->type);
            }
*/
            return tokens;
        }

        ~analyser()
        {
            source.clear();
            current = '\0';
            cursor = 0;
            length = 0;
            tokens.clear();
        }
    };

    class solver : analyser
    {
        std::stack<long double> values;
        std::stack<std::string> operands;

        long double value;

        long int index;

        //std::shared_ptr<Token> tseek(long int offset) { return (cursor + offset < length) ? tokens.at(cursor + offset) : nullptr; }

        std::map<std::string, int> oplist = 
        {
            {"+", 1}, {"-", 1},
            {"*", 2}, {"/", 2}
        };

        bool prioritycheck(std::string op)
        {
            /*if(operands.empty())
                return true;

            std::string topop = operands.top();

            return oplist[op] > oplist[topop]*/

            return (operands.empty()) ? (oplist[op] > oplist[operands.top()]) : true;
        }

        long double processtop()
        {
            if (operands.empty() || values.size() < 2)
                assert("Not enough values or operators\n");

            long double b = values.top(); values.pop();
            long double a = values.top(); values.pop();
            std::string op = operands.top(); operands.pop();

            long double j = 0;

            if (op == "+")
            {
                j = a + b;
            }
            else if (op == "-")
            {
                j = a - b;
            }
            else if (op == "*")
            {
                j = a * b;
            }
            else if (op == "/")
            {
                if (std::isinf(a/b))
                    assert("Unexpected value\n");
                j = a / b;
            }
            values.push(j);
            return j;
        }

        public:
        void process()
        {
            for(auto a : tokens)
            {
                switch (a->type)
                {
                case TOKEN_OP:
                    while (!operands.empty() && !prioritycheck(a->value)) 
                    {
                        processtop();
                    }
                    operands.push(a->value);
                    break;
                case TOKEN_DIGIT:
                    values.push(std::stold(a->value));
                    break;
                default:
                    break;
                }
                ++index;
            }

            while (!operands.empty())
            {
                processtop();
            }

            value = values.top();
            values.pop();
        }

        long double getretval() { return value; }

        solver(std::string expr) : analyser(expr)
        {
            value = 0;
            index = 0;
            analyse();
            process();
        }
        
        ~solver()
        {

        }
    };

    long double eval(std::string expr)
    {
        solver sol(expr);


        return sol.getretval();
    }
}

int main()
{
    std::string expression;

    //calc::eval(expression);
    while(std::getline(std::cin, expression))
    {
        std::cout << "\n" << calc::eval(expression) << std::endl;
    }
    return 0;
}