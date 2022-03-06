#include <iostream>
#include <sstream>

#include "Funcs.h"
#include "test_runner.h"
#include "tests.h"
#include "main.h"

using namespace std;

//вывод unordered_set
template<class T>
inline std::ostream& operator<<(std::ostream& os, const std::unordered_set<T>& s)
{
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    os << "}";
    return os;
}

int main(int argc, char *argv[])
{
    //тесты
    test_all();

    //запуск работы
    while (cin) {
        solve(cin, cout, false);
        cout << endl;
    }
}

void solve(istream& in, ostream& out, bool silent)
{
    auto str = string();
    for (;;)
    {
        if (!silent) out << "> ";
        if (!getline(in, str)) break;
        try
        {
            //перевод в постфиксную запись
            auto tokens = to_postfix(tokenize(str));
            {
                //обработка переменных
                auto vars = tokens.get_vars_list();
                auto vars_to_vals = unordered_map<char, double>();
                if (!empty(vars)) {
                    if(!silent) out << "enter vars vals" << '\n';
                    if(!silent) out << vars << endl;
                    try {
                        auto vars_input = string();
                        getline(in, vars_input);
                        auto vin = stringstream(vars_input);

                        for (auto& var : vars) {
                            vin >> vars_to_vals[var];
                            if (!vin) throw invalid_argument("");
                        }
                        tokens.set_vars(vars_to_vals);
                    }
                    catch (...) {
                        throw invalid_argument("vars input error");
                    }
                }
                //вычисление
                auto result = tokens.eval();

                //вывод результата
                out << "postfix expr     : " << tokens << '\n';
                out << "postfix expr val : " << result << '\n';
            }
        }
        //ошибки
        catch (exception& e) {
            out << "error: " << e.what() << '\n';
        }
    }
}
