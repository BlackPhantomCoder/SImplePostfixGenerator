#include "tests.h"
#include <sstream>
#include "main.h"
using namespace std;

void test_all()
{
    TestRunner runner;
    RUN_TEST(runner, test_numbers);
    RUN_TEST(runner, test_operator);
    RUN_TEST(runner, test_vars);
    RUN_TEST(runner, test_brakets);
}

void test_template(string in, const string& expr, double expr_val) {
    auto sin = istringstream(move(in));
    auto sout = ostringstream();
    auto expr_val_output = ostringstream();
    expr_val_output << expr_val;
    solve(sin, sout);
    ASSERT_EQUAL(
        sout.str(),
        "postfix expr     : "s + expr + "\n"
        "postfix expr val : " + expr_val_output.str() + "\n"
    );
}

void test_numbers()
{
    test_template("1", "1", 1);
    test_template("2.", "2", 2);
    test_template("7000.09", "7000.09", 7000.09);
}

void test_vars()
{
    test_template("y\n 7\n", "7", 7);
    test_template("c+b+a\n3 2 1\n", "32+1+", 6);
    {
        auto sin = istringstream("aa");
        auto sout = ostringstream();
        solve(sin, sout);
        ASSERT_EQUAL(
            sout.str(),
            "error: unexpected symbol a\n"
        );
    }
}

void test_operator()
{
    test_template("100-100.01", "100100.01-", -0.01);
    test_template("200+200.02", "200200.02+", 400.02);
    test_template("5*5", "55*", 25);
    test_template("5/5", "55/", 1);
    test_template("-100", "100!", -100);
    test_template("+100", "100", 100);
}

void test_brakets()
{
    test_template("2*7+3", "27*3+", 17);
    test_template("2*(7+3)", "273+*", 20);
    test_template("2*(y+3)/a\n1 7\n", "273+*1/", 20);
}
