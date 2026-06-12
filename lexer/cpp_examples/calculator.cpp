#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>
#include <vector>
#include <sstream>
#include <map>
using namespace std;

// ============================================================
//  Calculator 
// ============================================================

class Calculator {
private:
    vector<string> history;
    double memory = 0.0;

    void record(const string& expr, double result) {
        ostringstream oss;
        oss << expr << " = " << result;
        history.push_back(oss.str());
    }

public:
    double add(double a, double b) {
        double r = a + b;
        record(to_string(a) + " + " + to_string(b), r);
        return r;
    }

    double subtract(double a, double b) {
        double r = a - b;
        record(to_string(a) + " - " + to_string(b), r);
        return r;
    }

    double multiply(double a, double b) {
        double r = a * b;
        record(to_string(a) + " * " + to_string(b), r);
        return r;
    }

    double divide(double a, double b) {
        if (b == 0)
            throw invalid_argument("Division by zero");
        double r = a / b;
        record(to_string(a) + " / " + to_string(b), r);
        return r;
    }

    double modulo(long long a, long long b) {
        if (b == 0)
            throw invalid_argument("Modulo by zero");
        double r = a % b;
        record(to_string(a) + " % " + to_string(b), r);
        return r;
    }

    double power(double base, int exp) {
        double result = 1.0;
        bool negative = exp < 0;
        int e = negative ? -exp : exp;
        for (int i = 0; i < e; i++)
            result *= base;
        double r = negative ? 1.0 / result : result;
        record(to_string(base) + " ^ " + to_string(exp), r);
        return r;
    }

    double squareRoot(double a) {
        if (a < 0)
            throw invalid_argument("Cannot take square root of a negative number");
        double r = sqrt(a);
        record("sqrt(" + to_string(a) + ")", r);
        return r;
    }

    double factorial(int n) {
        if (n < 0)
            throw invalid_argument("Factorial is undefined for negative numbers");
        double r = 1.0;
        for (int i = 2; i <= n; i++) r *= i;
        record(to_string(n) + "!", r);
        return r;
    }

    double percentage(double value, double percent) {
        double r = value * percent / 100.0;
        record(to_string(percent) + "% of " + to_string(value), r);
        return r;
    }

    // Memory operations, like a basic physical calculator
    void memoryStore(double value) { memory = value; }
    void memoryAdd(double value) { memory += value; }
    void memorySubtract(double value) { memory -= value; }
    double memoryRecall() const { return memory; }
    void memoryClear() { memory = 0.0; }

    const vector<string>& getHistory() const { return history; }

    void clearHistory() { history.clear(); }
};

// A small recursive-descent expression evaluator supporting +, -, *, /, ^
// and parentheses, used to demonstrate combining the Calculator with parsing.
class ExpressionEvaluator {
private:
    string expr;
    size_t pos = 0;
    Calculator& calc;

    void skipWhitespace() {
        while (pos < expr.size() && isspace(expr[pos])) pos++;
    }

    char peek() {
        skipWhitespace();
        return pos < expr.size() ? expr[pos] : '\0';
    }

    char get() {
        skipWhitespace();
        return expr[pos++];
    }

    double parseNumber() {
        skipWhitespace();
        size_t start = pos;
        while (pos < expr.size() && (isdigit(expr[pos]) || expr[pos] == '.')) pos++;
        return stod(expr.substr(start, pos - start));
    }

    double parseFactor() {
        if (peek() == '(') {
            get(); // consume '('
            double result = parseExpression();
            if (peek() == ')') get(); // consume ')'
            return result;
        }
        if (peek() == '-') {
            get();
            return -parseFactor();
        }
        return parseNumber();
    }

    double parsePower() {
        double base = parseFactor();
        if (peek() == '^') {
            get();
            double exponent = parsePower(); // right-associative
            return calc.power(base, (int)exponent);
        }
        return base;
    }

    double parseTerm() {
        double result = parsePower();
        while (true) {
            char c = peek();
            if (c == '*') {
                get();
                result = calc.multiply(result, parsePower());
            } else if (c == '/') {
                get();
                result = calc.divide(result, parsePower());
            } else {
                break;
            }
        }
        return result;
    }

    double parseExpression() {
        double result = parseTerm();
        while (true) {
            char c = peek();
            if (c == '+') {
                get();
                result = calc.add(result, parseTerm());
            } else if (c == '-') {
                get();
                result = calc.subtract(result, parseTerm());
            } else {
                break;
            }
        }
        return result;
    }

public:
    ExpressionEvaluator(Calculator& c) : calc(c) {}

    double evaluate(const string& expression) {
        expr = expression;
        pos = 0;
        return parseExpression();
    }
};

int main() {
    Calculator calc;

    cout << "=== Basic Operations ===" << endl;
    cout << "10 + 5 = "  << calc.add(10, 5)      << endl;
    cout << "10 - 5 = "  << calc.subtract(10, 5) << endl;
    cout << "10 * 5 = "  << calc.multiply(10, 5) << endl;
    cout << "10 / 5 = "  << calc.divide(10, 5)   << endl;
    cout << "2 ^ 8 = "   << calc.power(2, 8)     << endl;
    cout << "2 ^ -2 = "  << calc.power(2, -2)    << endl;

    cout << endl << "=== Additional Operations ===" << endl;
    cout << "17 % 5 = "          << calc.modulo(17, 5)         << endl;
    cout << "sqrt(144) = "       << calc.squareRoot(144)       << endl;
    cout << "5! = "              << calc.factorial(5)          << endl;
    cout << "20% of 250 = "      << calc.percentage(250, 20)   << endl;

    cout << endl << "=== Error Handling ===" << endl;
    try {
        calc.divide(10, 0);
    } catch (const invalid_argument& e) {
        cout << "Caught error: " << e.what() << endl;
    }

    try {
        calc.squareRoot(-9);
    } catch (const invalid_argument& e) {
        cout << "Caught error: " << e.what() << endl;
    }

    try {
        calc.factorial(-3);
    } catch (const invalid_argument& e) {
        cout << "Caught error: " << e.what() << endl;
    }

    cout << endl << "=== Memory Functions ===" << endl;
    calc.memoryStore(100);
    cout << "Memory after store(100): " << calc.memoryRecall() << endl;
    calc.memoryAdd(50);
    cout << "Memory after add(50): " << calc.memoryRecall() << endl;
    calc.memorySubtract(30);
    cout << "Memory after subtract(30): " << calc.memoryRecall() << endl;
    calc.memoryClear();
    cout << "Memory after clear: " << calc.memoryRecall() << endl;

    cout << endl << "=== Expression Evaluator ===" << endl;
    ExpressionEvaluator evaluator(calc);
    vector<string> expressions = {
        "2 + 3 * 4",
        "(2 + 3) * 4",
        "2 ^ 3 ^ 2",      // right-associative: 2 ^ (3 ^ 2) = 512
        "10 / 2 - 3",
        "-5 + 10",
        "(1 + 2) * (3 + 4)"
    };

    for (const auto& e : expressions) {
        cout << e << " = " << evaluator.evaluate(e) << endl;
    }

    cout << endl << "=== Operation History (last 5 entries) ===" << endl;
    const auto& history = calc.getHistory();
    size_t startIdx = history.size() > 5 ? history.size() - 5 : 0;
    for (size_t i = startIdx; i < history.size(); i++) {
        cout << "  " << history[i] << endl;
    }
    cout << "Total operations recorded: " << history.size() << endl;

    return 0;
}