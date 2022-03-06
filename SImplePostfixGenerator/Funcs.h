#ifndef FUNCS_H
#define FUNCS_H

#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>
#include "Tokens.h"

//класс вычисление по постфиксной нотации
class NExpr{
    friend std::ostream& operator<<(std::ostream& os, const NExpr& rh);
public:
    explicit NExpr(const std::vector<Token>& tokens);
    explicit NExpr(std::vector<Token>&& tokens);

    //вычислить
    double eval() const;
    //установить зачения переменных 
    //заменяет те, которые в есть и в выражении и в vars
    void set_vars(const std::unordered_map<char, double>& vars);
    //найти все переменные 
    std::unordered_set<char> get_vars_list() const;

    std::string to_string() const;
    operator std::string() const;
private:
    std::vector<Token> t_postfix;
};

//вывод NExpr
std::ostream& operator<<(std::ostream& os, const NExpr& rh);
//перевод строки в токены
std::vector<InputToken> tokenize(std::string_view input);
//перевод токенов в постфиксную запись
NExpr to_postfix(const std::vector<InputToken>& expression);


#endif // FUNCS_H
