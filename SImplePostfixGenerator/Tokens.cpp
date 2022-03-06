#include "Tokens.h"
#include <sstream>
using namespace std;

Token::Token(double val) : 
    t_data(val)
{
}

Token::Token(char val) :
    t_data(val)
{
}

Token::Token(Symbols::op val) :
    t_data(val)
{
}

Token::Token(const Token& rh) :
    t_data(rh.t_data)
{
}

bool Token::is_constant() const
{
    return std::holds_alternative<double>(t_data);
}

bool Token::is_var() const
{
    return std::holds_alternative<char>(t_data);
}

bool Token::is_op() const
{
    return std::holds_alternative<Symbols::op>(t_data);
}

double Token::get_constant() const
{
    return std::get<double>(t_data);
}

char Token::get_var() const
{
    return std::get<char>(t_data);
}

Symbols::op Token::get_op() const
{
    return std::get<Symbols::op>(t_data);
}

Token::operator std::string() const
{
    auto os = ostringstream();
    if(holds_alternative<double>(t_data)){
        os << get<double>(t_data);
    }
    else if(holds_alternative<char>(t_data)){
        os << get<char>(t_data);
    }
    else if(holds_alternative<Symbols::op>(t_data)){
        os << to_string(get<Symbols::op>(t_data));
    }
    else {
        throw invalid_argument("");
    }
    return os.str();
}

InputToken::operator std::string() const
{
    auto os = ostringstream();
    if(holds_alternative<ex_tokens>(t_data2)){
        switch (get<ex_tokens>(t_data2)) {
            case ex_tokens::cb:
                os << ")";
                break;
            case ex_tokens::ob:
                os << "(";
                break;
        default:
            throw logic_error("");
        }
    }
    else {
        return Token::operator string();
    }
    return os.str();
}

InputToken::InputToken(double val) : 
    Token(val)
{
}

InputToken::InputToken(char val) :
    Token(val)
{
}

InputToken::InputToken(Symbols::op val) :
    Token(val)
{
}

InputToken::InputToken(ex_tokens val):
    Token(),
    t_data2(val) 
{

}

InputToken::InputToken(const InputToken& rh): 
    Token(rh), 
    t_data2(rh.t_data2) 
{

}

bool InputToken::is_op_braket() const
{
    return holds_alternative<ex_tokens>(t_data2) && get<ex_tokens>(t_data2) == ex_tokens::ob;
}

bool InputToken::is_cl_braket() const
{
    return holds_alternative<ex_tokens>(t_data2) && get<ex_tokens>(t_data2) == ex_tokens::cb;
}

ostream &operator<<(ostream &os, const Token &token)
{
    if(os){
        os << string(token);
    }
    return os;
}

Token to_token(const InputToken &token)
{
    if(holds_alternative<monostate>(token.t_data2)){
        return Token(token);
    }
    throw invalid_argument("");
}
