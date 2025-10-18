#include "AlgorithmExpression.h"

#include <cctype>
#include <cmath>
#include <stdexcept>

namespace {
int intPow(int base, int exponent)
{
    if (exponent < 0) {
        throw std::runtime_error("negative exponent is not supported");
    }

    int result = 1;
    int current = base;
    int power = exponent;
    while (power > 0) {
        if (power & 1) {
            result *= current;
        }
        current *= current;
        power >>= 1;
    }
    return result;
}

void applyTop(std::stack<int> &values, std::stack<char> &ops)
{
    if (values.size() < 2 || ops.empty()) {
        throw std::runtime_error("invalid expression");
    }

    int rhs = values.top();
    values.pop();
    int lhs = values.top();
    values.pop();
    char op = ops.top();
    ops.pop();

    switch (op) {
    case '+':
        values.push(lhs + rhs);
        break;
    case '-':
        values.push(lhs - rhs);
        break;
    case '*':
        values.push(lhs * rhs);
        break;
    case '/':
        if (rhs == 0) {
            throw std::runtime_error("division by zero");
        }
        values.push(lhs / rhs);
        break;
    case '^':
        values.push(intPow(lhs, rhs));
        break;
    default:
        throw std::runtime_error("unsupported operator");
    }
}

} // namespace

int AlgorithmExpression::calculate_expression(const std::string &expression)
{
    std::string compact;
    compact.reserve(expression.size());
    for (char c : expression) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            compact.push_back(c);
        }
    }

    if (compact.empty()) {
        throw std::runtime_error("expression is empty");
    }

    // 处理一元 +/-，通过在其前面补 0 的方式化为二元运算。
    auto needsZeroPadding = [&](std::size_t index) {
        if (index == 0) {
            return true;
        }
        char prev = compact[index - 1];
        return prev == '(' || isOperator(prev);
    };

    std::string normalized;
    normalized.reserve(compact.size() + 4);
    for (std::size_t i = 0; i < compact.size(); ++i) {
        char c = compact[i];
        if ((c == '+' || c == '-') && needsZeroPadding(i)) {
            normalized.push_back('0');
        }
        normalized.push_back(c);
    }

    std::stack<int> values;
    std::stack<char> ops;

    auto precedenceGreater = [](char lhs, char rhs) {
        int lp = AlgorithmExpression::precedence(lhs);
        int rp = AlgorithmExpression::precedence(rhs);
        if (lhs == '^' && rhs == '^') {
            return lp > rp; // 右结合
        }
        return lp >= rp;
    };

    for (std::size_t i = 0; i < normalized.size();) {
        char c = normalized[i];
        if (std::isdigit(static_cast<unsigned char>(c))) {
            int value = c - '0';
            ++i;
            while (i < normalized.size() && std::isdigit(static_cast<unsigned char>(normalized[i]))) {
                value = value * 10 + (normalized[i] - '0');
                ++i;
            }
            values.push(value);
        } else if (c == '(') {
            ops.push(c);
            ++i;
        } else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                applyTop(values, ops);
            }
            if (ops.empty()) {
                throw std::runtime_error("mismatched parentheses");
            }
            ops.pop();
            ++i;
        } else if (isOperator(c)) {
            while (!ops.empty() && ops.top() != '(' && precedenceGreater(ops.top(), c)) {
                applyTop(values, ops);
            }
            ops.push(c);
            ++i;
        } else {
            throw std::runtime_error("invalid character in expression");
        }
    }

    while (!ops.empty()) {
        if (ops.top() == '(') {
            throw std::runtime_error("mismatched parentheses");
        }
        applyTop(values, ops);
    }

    if (values.size() != 1) {
        throw std::runtime_error("invalid expression");
    }

    return values.top();
}

bool AlgorithmExpression::isOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int AlgorithmExpression::precedence(char op)
{
    switch (op) {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    default:
        return -1;
    }
}

