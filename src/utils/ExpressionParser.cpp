#include "ExpressionParser.h"
#include "tinyexpr.h"
#include <cmath>
#include <string>
#include <functional>
#include <sstream>

std::function<double(double)> ExpressionParser::Parse(const std::string& expression) {
    return [expression](double x) -> double {
        int error;
        te_variable vars[] = {{"x", &x}};
        te_expr *expr = te_compile(expression.c_str(), vars, 1, &error);
        
        if (expr) {
            double result = te_eval(expr);
            te_free(expr);
            return result;
        }
        return 0.0;
    };
}

bool ExpressionParser::Validate(const std::string& expression, std::string& errorMessage) {
    if (expression.empty()) {
        errorMessage = "Empty expression";
        return false;
    }
    
    double x = 0.0;
    te_variable vars[] = {{"x", &x}};
    int error;
    
    te_expr *expr = te_compile(expression.c_str(), vars, 1, &error);
    if (expr) {
        te_free(expr);
        return true;
    }
    
    errorMessage = "Invalid expression at position " + std::to_string(error);
    return false;
}