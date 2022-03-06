#include "Funcs.h"
#include <algorithm>
#include <unordered_set>
#include <stack>
#include <cmath>
using namespace std;

inline bool is_number(char ch) {
    return ch >= '0' && ch <= '9';
}

inline bool is_simbol(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

inline bool is_open_bracket(char ch) {
    return ch == '(';
}

inline bool is_close_bracket(char ch) {
    return ch == ')';
}

std::vector<InputToken> tokenize(string_view input)
{
    if (empty(input)) 
        throw invalid_argument("empty string");
    auto result = std::vector<InputToken>();
    auto i = begin(input);
    while (i != end(input)) {
        char ch = *i;
        if(ch == ' ' || ch == char(9) || ch == '\t' || ch == '\r' || ch == '\n'){
            ++i;
            continue;
        }
        if (Symbols::is_op(ch)) {
            if(result.size() == 0 || result.back().is_op_braket() || result.back().is_op()){
                if(ch == '-'){
                    result.push_back(Symbols::to_op('!'));
                }
                else if(ch == '+'){

                }
                else{
                    result.push_back(Symbols::to_op(ch));
                }
            }
            else{
                result.push_back(Symbols::to_op(ch));
            }
        }
        else {
            if (is_number(ch)) {
                auto start_i = i;
                auto number = 0.;
                bool dot = false;
                for (; i != end(input); ++i) {
                    if (!is_number(*i)) {
                        if (*i == '.') {
                            if (dot)
                                throw invalid_argument("unexpected symbol "s + *i);
                            else
                                dot = true;
                        }
                        else break;
                    }
                }
                result.push_back(atof(string(start_i, i).c_str()));
                continue;
            }
            else if (is_simbol(ch)) {
                if(next(i) != end(input) && is_simbol(*next(i))) 
                    throw invalid_argument("unexpected symbol "s + *i);
                result.push_back(ch);
            }
            else if (is_open_bracket(ch)) {
                result.push_back(InputToken::ex_tokens::ob);
            }
            else if (is_close_bracket(ch)) {
                result.push_back(InputToken::ex_tokens::cb);
            }
            else {
                throw invalid_argument("unexpected symbol "s + *i);
            }
        }
        ++i;
    }
    return result;
}

NExpr to_postfix(const std::vector<InputToken> &expression)
{
    auto opstack = stack<InputToken>();
    auto output = vector<Token>();

    for (const auto& op : expression) {
        if (op.is_constant() || op.is_var()) {
            output.push_back(to_token(op));
        }
        else if(op.is_op_braket()){
            opstack.push(op);
        }
        else if (op.is_cl_braket()) {
            while (!empty(opstack) && !opstack.top().is_op_braket()) {
                output.push_back(to_token(opstack.top()));
                opstack.pop();
            }
            if (empty(opstack)) throw invalid_argument("close braket miss");
            opstack.pop();
        }
        else if (op.is_op()) {
            while
                (
                    !opstack.empty()
                    &&
                    opstack.top().is_op()
                    &&
                    (
                        get_prioritet(opstack.top().get_op()) > get_prioritet(op.get_op())
                        ||
                        (
                             get_prioritet(opstack.top().get_op()) == get_prioritet(op.get_op())
                             &&
                             get_assign(op.get_op()) == Symbols::assign::left
                        )
                    )
                )
            {
                output.push_back(to_token(opstack.top()));
                opstack.pop();
            }
            opstack.push(op);
        }
    }

    while (!opstack.empty()) {
        if (opstack.top().is_op_braket()) throw invalid_argument("close braket miss");
        output.push_back(to_token(opstack.top()));
        opstack.pop();
    }
    return NExpr{std::move(output)};
}

NExpr::NExpr(const std::vector<Token>& tokens) :
    t_postfix(tokens)

{

}

NExpr::NExpr(std::vector<Token>&& tokens):
    t_postfix(std::move(tokens))
{

}

void NExpr::set_vars(const std::unordered_map<char, double>& vars)
{
    for (auto& elem : t_postfix) {
        if (elem.is_var()) {
            if (auto it = vars.find(elem.get_var()); it != end(vars)) {
                elem = it->second;
            }
            else
                throw std::invalid_argument("");
        }
    }
}

std::unordered_set<char> NExpr::get_vars_list() const
{
    std::unordered_set<char> vars;
    for (auto& elem : t_postfix) {
        if (elem.is_var()) {
            vars.insert(elem.get_var());
        }
    }
    return vars;
}

std::string NExpr::to_string() const
{
    auto result = string();
    for (const auto& token : t_postfix) {
        result += token;
    }
    return result;
}

NExpr::operator std::string() const{
    return to_string();
}

double NExpr::eval() const
{
    std::vector<double> stack;
    for(const auto& elem : t_postfix){
        if(elem.is_op()){
            auto now_op = elem.get_op();
            auto args_count = get_args_count(now_op);
            if(stack.size() < args_count){
                throw invalid_argument("expr: " + Symbols::to_string(now_op) + " not enougth arguments");
            }
            double result = op_eval(now_op, end(stack) - args_count, end(stack));
            stack.erase(end(stack) - args_count, end(stack));
            stack.push_back(result);

        }
        else if(elem.is_constant()){
            stack.push_back(elem.get_constant());
        }
        else if(elem.is_var()){
            throw invalid_argument("uninit var");
        }
        else
            throw logic_error("");
    }
    if(stack.size() == 1){
        return stack.back();
    }
    throw invalid_argument("wrong expr");
}


ostream &operator<<(ostream &os, const NExpr &rh)
{
    if(os)
        os << string(rh);
    return os;
}