#pragma once
#include "stack.hpp"
#include <string>
#include <sstream>
#include <stdexcept>
#include <cctype>

class PostfixCalculator {
public:
    // Вычисляет значение выражения в постфиксной (обратной польской) нотации.
    // Токены разделяются пробелами.
    // Поддерживаются операции: +  -  *  /
    double evaluate(const std::string& expr) {
        Stack<double> stack;
        std::istringstream iss(expr);
        std::string token;

        while (iss >> token) {
            if (isOperator(token)) {
                if (stack.size() < 2)
                    throw std::invalid_argument(
                        "PostfixCalculator: недостаточно операндов для операции '" + token + "'");

                double b = stack.pop();   // правый операнд
                double a = stack.pop();   // левый операнд
                stack.push(applyOp(token[0], a, b));
            } else {
                // Попытка распарсить число
                std::size_t pos = 0;
                double num = std::stod(token, &pos);
                if (pos != token.size())
                    throw std::invalid_argument(
                        "PostfixCalculator: неизвестный токен '" + token + "'");
                stack.push(num);
            }
        }

        if (stack.size() != 1)
            throw std::invalid_argument(
                "PostfixCalculator: некорректное выражение (остались лишние операнды в стеке)");

        return stack.pop();
    }

private:
    static bool isOperator(const std::string& t) {
        return t.size() == 1 && std::string("+-*/").find(t[0]) != std::string::npos;
    }

    static double applyOp(char op, double a, double b) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/':
                if (b == 0.0)
                    throw std::domain_error("PostfixCalculator: деление на ноль");
                return a / b;
            default:
                throw std::invalid_argument(
                    std::string("PostfixCalculator: неподдерживаемая операция '") + op + "'");
        }
    }
};