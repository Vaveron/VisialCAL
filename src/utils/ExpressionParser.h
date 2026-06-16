#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

#include <string>
#include <functional>
#include <cmath>

class ExpressionParser {
public:
    static std::function<double(double)> Parse(const std::string& expression);
    static bool Validate(const std::string& expression, std::string& errorMessage);
};

#endif