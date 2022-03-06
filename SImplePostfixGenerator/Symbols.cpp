#include "Symbols.h"
#include <sstream>
using namespace std;


bool Symbols::is_op(char name)
{
    return find_if(begin(op_to_char), end(op_to_char), [&name](const auto& p) {return p.second == name; }) != end(op_to_char);
}

Symbols::op Symbols::to_op(char name)
{
    auto it = find_if(begin(op_to_char), end(op_to_char), [&name](const auto& p) {return p.second == name; });
    if(it  != end(op_to_char)){
        return it->first;
    }
    throw invalid_argument("");
}

size_t Symbols::get_prioritet(op op)
{
    if(auto it = op_to_prioritet.find(op); it != end(op_to_prioritet)){
        return it->second;
    }
    throw logic_error("");
}

string Symbols::to_string(op val)
{
    if(auto it = op_to_char.find(val); it != end(op_to_char)){
        return string() + it->second;
    }
    throw logic_error("");
}

size_t Symbols::get_args_count(op op)
{
    if(auto it = op_to_args_count.find(op); it != end(op_to_args_count)){
        return it->second;
    }
    throw logic_error("");
}

Symbols::assign Symbols::get_assign(op op)
{
    if(auto it = op_to_assign.find(op); it != end(op_to_assign)){
        return it->second;
    }
    throw logic_error("");
}
