#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
using namespace std;

// ============================================================
//  Stack Implementations and Applications - Array-based and
//  linked-list-based stacks, plus classic stack-driven
//  algorithms: balanced parentheses, postfix evaluation, and
//  infix-to-postfix conversion
// ============================================================

// Array-based stack with dynamic resizing (no fixed capacity limit)
template <typename T>
class Stack {
private:
    T* data;
    int top;
    int capacity;

    void resize() {
        capacity *= 2;
        T* newData = new T[capacity];
        for (int i = 0; i <= top; i++) newData[i] = data[i];
        delete[] data;
        data = newData;
    }

public:
    explicit Stack(int cap = 4) : top(-1), capacity(cap) {
        data = new T[capacity];
    }

    void push(const T& val) {
        if (top == capacity - 1) resize();
        data[++top] = val;
    }

    T pop() {
        if (top == -1)
            throw underflow_error("Stack underflow");
        return data[top--];
    }

    T peek() const {
        if (top == -1)
            throw underflow_error("Stack is empty");
        return data[top];
    }

    bool isEmpty() const { return top == -1; }
    int size() const { return top + 1; }
    int currentCapacity() const { return capacity; }

    ~Stack() { delete[] data; }
};

// Linked-list based stack
template <typename T>
class LinkedStack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val, Node* n) : data(val), next(n) {}
    };
    Node* head;
    int count;

public:
    LinkedStack() : head(nullptr), count(0) {}

    void push(const T& val) {
        head = new Node(val, head);
        count++;
    }

    T pop() {
        if (!head)
            throw underflow_error("Stack underflow");
        Node* old = head;
        T val = old->data;
        head = head->next;
        delete old;
        count--;
        return val;
    }

    T peek() const {
        if (!head)
            throw underflow_error("Stack is empty");
        return head->data;
    }

    bool isEmpty() const { return head == nullptr; }
    int size() const { return count; }

    ~LinkedStack() {
        while (head) {
            Node* next = head->next;
            delete head;
            head = next;
        }
    }
};

// Check whether a string of brackets is balanced: (), [], {}
bool isBalanced(const string& expr) {
    Stack<char> s;
    for (char c : expr) {
        if (c == '(' || c == '[' || c == '{') {
            s.push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (s.isEmpty()) return false;
            char top = s.pop();
            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{'))
                return false;
        }
    }
    return s.isEmpty();
}

// Evaluate a postfix (Reverse Polish Notation) expression, tokens separated by spaces
double evaluatePostfix(const string& expr) {
    Stack<double> s;
    istringstream iss(expr);
    string token;

    while (iss >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            double b = s.pop();
            double a = s.pop();
            double result;
            if (token == "+") result = a + b;
            else if (token == "-") result = a - b;
            else if (token == "*") result = a * b;
            else {
                if (b == 0) throw runtime_error("Division by zero in postfix expression");
                result = a / b;
            }
            s.push(result);
        } else {
            s.push(stod(token));
        }
    }

    double result = s.pop();
    if (!s.isEmpty())
        throw runtime_error("Malformed postfix expression");
    return result;
}

// Return the precedence of an operator (higher number = higher precedence)
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

// Convert an infix expression (single-digit operands, no spaces) to postfix
// using the shunting-yard algorithm.
string infixToPostfix(const string& infix) {
    Stack<char> ops;
    string output;

    for (char c : infix) {
        if (isspace(c)) continue;
        if (isalnum(c)) {
            output += c;
            output += ' ';
        } else if (c == '(') {
            ops.push(c);
        } else if (c == ')') {
            while (!ops.isEmpty() && ops.peek() != '(') {
                output += ops.pop();
                output += ' ';
            }
            if (!ops.isEmpty()) ops.pop(); // remove '('
        } else { // operator
            while (!ops.isEmpty() && precedence(ops.peek()) >= precedence(c)) {
                output += ops.pop();
                output += ' ';
            }
            ops.push(c);
        }
    }
    while (!ops.isEmpty()) {
        output += ops.pop();
        output += ' ';
    }
    return output;
}

// Reverse a string using a stack
string reverseString(const string& s) {
    Stack<char> stack;
    for (char c : s) stack.push(c);
    string result;
    while (!stack.isEmpty()) result += stack.pop();
    return result;
}

// Check whether a string is a palindrome using a stack
bool isPalindromeUsingStack(const string& s) {
    string reversed = reverseString(s);
    return reversed == s;
}

int main() {
    cout << "=== Array-Based Stack ===" << endl;
    Stack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);
    cout << "Top: " << s.peek() << endl;
    cout << "Pop: " << s.pop() << endl;
    cout << "Top: " << s.peek() << endl;
    cout << "Size: " << s.size() << endl;

    cout << endl << "=== Dynamic Resizing ===" << endl;
    Stack<int> small(2);
    cout << "Initial capacity: " << small.currentCapacity() << endl;
    for (int i = 0; i < 10; i++) small.push(i);
    cout << "Capacity after pushing 10 elements: " << small.currentCapacity() << endl;
    cout << "Size: " << small.size() << endl;

    cout << endl << "=== Underflow Handling ===" << endl;
    try {
        Stack<int> empty;
        empty.pop();
    } catch (const underflow_error& e) {
        cout << "Caught: " << e.what() << endl;
    }

    cout << endl << "=== Linked-List Based Stack ===" << endl;
    LinkedStack<string> ls;
    ls.push("a");
    ls.push("b");
    ls.push("c");
    cout << "Top: " << ls.peek() << endl;
    cout << "Pop: " << ls.pop() << endl;
    cout << "Pop: " << ls.pop() << endl;
    cout << "Size: " << ls.size() << endl;

    cout << endl << "=== Balanced Parentheses ===" << endl;
    vector<string> exprs = {"(a+b)*(c-d)", "([{}])", "(()", "{[}]", "", "a+b"};
    for (const auto& e : exprs) {
        cout << "\"" << e << "\" balanced: " << (isBalanced(e) ? "yes" : "no") << endl;
    }

    cout << endl << "=== Postfix Evaluation ===" << endl;
    vector<string> postfixExprs = {
        "3 4 +",
        "5 1 2 + 4 * + 3 -",
        "2 3 *",
        "10 2 /"
    };
    for (const auto& e : postfixExprs) {
        cout << "\"" << e << "\" = " << evaluatePostfix(e) << endl;
    }

    try {
        evaluatePostfix("5 0 /");
    } catch (const runtime_error& e) {
        cout << "Caught: " << e.what() << endl;
    }

    cout << endl << "=== Infix to Postfix Conversion ===" << endl;
    vector<string> infixExprs = {"a+b*c", "(a+b)*c", "a+b*c-d/e", "(a+b)*(c+d)"};
    for (const auto& e : infixExprs) {
        cout << e << "  ->  " << infixToPostfix(e) << endl;
    }

    cout << endl << "=== String Reversal and Palindrome Check ===" << endl;
    string word = "hello";
    cout << "Reverse of \"" << word << "\": " << reverseString(word) << endl;

    vector<string> palindromeTests = {"racecar", "level", "hello", "a"};
    for (const auto& w : palindromeTests) {
        cout << "\"" << w << "\" is palindrome: " << (isPalindromeUsingStack(w) ? "yes" : "no") << endl;
    }

    return 0;
}