#ifndef TOKENS_H
#define TOKENS_H
#include <string>
#include <variant>
#include "Symbols.h"

//Токен вычисления
class Token{
public:
    Token(double val);
    Token(char val);
    Token(Symbols::op val);
    Token(const Token& rh);

    virtual ~Token() = default;

    //является ли константой
    bool is_constant()const;
    //является ли переменной
    bool is_var()const;
    //является ли оператором
    bool is_op()const;

    //получить данные константы
    double get_constant() const;
    //получить данные переменной
    char get_var() const;
    //получить данные оператора
    Symbols::op get_op() const;

    virtual operator std::string() const;
protected:
    Token() = default;
private:
    std::variant<double, char, Symbols::op, std::monostate> t_data = std::monostate{};
};

//Токен вывода
class InputToken : public Token
{
    friend Token to_token(const InputToken& token);
public:
    enum class ex_tokens {ob, cb};
public:
    InputToken(double val);
    InputToken(char val);
    InputToken(Symbols::op val);
    InputToken(ex_tokens val);
    InputToken(const InputToken& rh);

    //является ли открывающей скобкой
    bool is_op_braket()const;
    //является ли закрывающей скобкой
    bool is_cl_braket()const;

    virtual operator std::string() const override;
private:
    std::variant<ex_tokens, std::monostate> t_data2 = std::monostate{};
};

std::ostream& operator<<(std::ostream& os, const Token& token);
Token to_token(const InputToken& token);

#endif // TOKENS_H
