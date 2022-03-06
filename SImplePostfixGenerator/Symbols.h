#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stdexcept>
#include <unordered_map>

namespace Symbols {

    //операторы
    enum class op { plus, minus, div, mul, uminus };
    //ассоциативность
    enum class assign { left, right };

    //Словари, задающие операторы и их свойства

    //словарь (оператор, количество аргументов)
    static std::unordered_map<op, size_t> op_to_args_count = {
        {op::plus,      2},
        {op::minus,     2},
        {op::div,       2},
        {op::mul,       2},
        {op::uminus,    1},
    };

    //словарь (оператор, символ)
    static std::unordered_map<op, char> op_to_char = {
        {op::plus,       '+'},
        {op::minus,      '-'},
        {op::div,        '/'},
        {op::mul,        '*'},
        {op::uminus,     '!'},
    };

    //словарь (оператор, ассоциативность)
    static std::unordered_map<op, assign> op_to_assign = {
        {op::plus,         assign::left},
        {op::minus,        assign::left},
        {op::div,          assign::left},
        {op::mul,          assign::left},
        {op::uminus,       assign::right},
    };

    //словарь (оператор, приоритет)
    static std::unordered_map<op, size_t> op_to_prioritet = {
        {op::plus,         1},
        {op::minus,        1},
        {op::div,          2},
        {op::mul,          2},
        {op::uminus,       2},
    };


    //приоритет оператора
    size_t get_prioritet(op op);
    //количество аргументов оператора
    size_t get_args_count(op op);
    //ассоциативность оператора
    assign get_assign(op op);

    //проверка оператора по символу
    bool is_op(char name);
    //получение оператора по символу
    op to_op(char name);

    //std::string to_string(double val);
    std::string to_string(op val);
    //вычислить оператор
    template<class Iter>
    double op_eval(op op, Iter b, Iter e);











    //реализации шаблонных функций
    template<class Iter>
    double op_eval(op op, Iter b, Iter e)
    {
        if (get_args_count(op) != std::distance(b, e)) throw "";
        switch (op) {
        case op::div:
            if (*(b + 1) == 0) throw "division by zero";
            return *b / *(b + 1);
        case op::minus:
            return *b - *(b + 1);
        case op::mul:
            return *b * *(b + 1);
        case op::plus:
            return *b + *(b + 1);
        case op::uminus:
            return -*b;
        }
        throw std::logic_error("");
    }

}
#endif // SYMBOLS_H
