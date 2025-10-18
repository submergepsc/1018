#pragma once

#include <stack>
#include <string>

// 解析并计算中缀表达式的工具类，支持括号和 ^/*/-/+ 运算。
class AlgorithmExpression {
public:
    AlgorithmExpression() = default;
    ~AlgorithmExpression() = default;

    int calculate_expression(const std::string &expression);

private:
    static bool isOperator(char c);
    static int precedence(char op);
};