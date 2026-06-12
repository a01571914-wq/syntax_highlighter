#include <iostream>
#include <vector>
#include <map>
#include <chrono>
#include <cmath>
using namespace std;
using namespace std::chrono;

// ============================================================
//  Fibonacci - Multiple implementations and analyses
// ============================================================

// Naive recursive fibonacci - exponential time complexity
long long fibRecursive(int n) {
    if (n <= 1) return n;
    return fibRecursive(n - 1) + fibRecursive(n - 2);
}

// Recursive fibonacci with memoization (top-down dynamic programming)
long long fibMemo(int n, map<int, long long>& memo) {
    if (n <= 1) return n;
    auto it = memo.find(n);
    if (it != memo.end()) return it->second;
    long long result = fibMemo(n - 1, memo) + fibMemo(n - 2, memo);
    memo[n] = result;
    return result;
}

long long fibMemo(int n) {
    map<int, long long> memo;
    return fibMemo(n, memo);
}

// Bottom-up dynamic programming fibonacci using a table
long long fibDP(int n) {
    if (n <= 1) return n;
    vector<long long> dp(n + 1);
    dp[0] = 0;
    dp[1] = 1;
    for (int i = 2; i <= n; i++)
        dp[i] = dp[i - 1] + dp[i - 2];
    return dp[n];
}

// Space-optimized iterative fibonacci using only two variables
long long fibIterative(int n) {
    if (n <= 1) return n;
    long long prev = 0, curr = 1;
    for (int i = 2; i <= n; i++) {
        long long next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}

// Fibonacci using matrix exponentiation: [[1,1],[1,0]]^n = [[F(n+1),F(n)],[F(n),F(n-1)]]
struct Matrix2x2 {
    long long a, b, c, d;
};

Matrix2x2 multiplyMatrix(const Matrix2x2& m1, const Matrix2x2& m2) {
    return {
        m1.a * m2.a + m1.b * m2.c,
        m1.a * m2.b + m1.b * m2.d,
        m1.c * m2.a + m1.d * m2.c,
        m1.c * m2.b + m1.d * m2.d
    };
}

Matrix2x2 matrixPower(Matrix2x2 base, int exp) {
    Matrix2x2 result = {1, 0, 0, 1}; // identity matrix
    while (exp > 0) {
        if (exp % 2 == 1) result = multiplyMatrix(result, base);
        base = multiplyMatrix(base, base);
        exp /= 2;
    }
    return result;
}

long long fibMatrix(int n) {
    if (n == 0) return 0;
    Matrix2x2 base = {1, 1, 1, 0};
    Matrix2x2 result = matrixPower(base, n - 1);
    return result.a; // F(n) sits in the top-left entry after exponentiation
}

// Fibonacci using Binet's formula (closed-form, approximate for large n)
long long fibBinet(int n) {
    static const double phi = (1.0 + sqrt(5.0)) / 2.0;
    static const double psi = (1.0 - sqrt(5.0)) / 2.0;
    return (long long)llround((pow(phi, n) - pow(psi, n)) / sqrt(5.0));
}

// Generate the first n Fibonacci numbers as a vector (iterative)
vector<long long> fibSequence(int n) {
    vector<long long> result;
    result.reserve(n);
    long long prev = 0, curr = 1;
    for (int i = 0; i < n; i++) {
        result.push_back(prev);
        long long next = prev + curr;
        prev = curr;
        curr = next;
    }
    return result;
}

// Check whether a number is a Fibonacci number using the perfect square test:
// n is Fibonacci iff (5*n^2 + 4) or (5*n^2 - 4) is a perfect square
bool isPerfectSquare(long long x) {
    if (x < 0) return false;
    long long s = (long long)sqrt((double)x);
    for (long long candidate = max(0LL, s - 1); candidate <= s + 1; candidate++)
        if (candidate * candidate == x) return true;
    return false;
}

bool isFibonacciNumber(long long n) {
    long long x = 5 * n * n;
    return isPerfectSquare(x + 4) || isPerfectSquare(x - 4);
}

// Sum of first n Fibonacci numbers (F(0) + F(1) + ... + F(n-1)) = F(n+1) - 1
long long fibSum(int n) {
    if (n <= 0) return 0;
    return fibIterative(n + 1) - 1;
}

// Generic timing helper
template <typename Func>
long long timeFunction(Func f, const string& name) {
    auto start = high_resolution_clock::now();
    long long result = f();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << name << " = " << result << " (took " << duration.count() << " microseconds)" << endl;
    return result;
}

int main() {
    int n = 10;

    cout << "=== Fibonacci(" << n << ") Using Different Approaches ===" << endl;
    cout << "Recursive:        " << fibRecursive(n) << endl;
    cout << "Memoized:         " << fibMemo(n) << endl;
    cout << "Dynamic Prog:     " << fibDP(n) << endl;
    cout << "Iterative:        " << fibIterative(n) << endl;
    cout << "Matrix Power:     " << fibMatrix(n) << endl;
    cout << "Binet's Formula:  " << fibBinet(n) << endl;

    cout << endl << "=== Sequences (first " << n << " numbers) ===" << endl;
    cout << "Fibonacci (recursive): ";
    for (int i = 0; i < n; i++)
        cout << fibRecursive(i) << " ";
    cout << endl;

    cout << "Fibonacci (DP):        ";
    for (int i = 0; i < n; i++)
        cout << fibDP(i) << " ";
    cout << endl;

    cout << "Fibonacci (sequence):  ";
    vector<long long> seq = fibSequence(n);
    for (long long v : seq) cout << v << " ";
    cout << endl;

    cout << endl << "=== Larger Values (n = 50) ===" << endl;
    cout << "fibIterative(50) = " << fibIterative(50) << endl;
    cout << "fibMatrix(50)    = " << fibMatrix(50) << endl;
    cout << "fibMemo(50)      = " << fibMemo(50) << endl;
    cout << "fibBinet(50)     = " << fibBinet(50) << " (approximate via floating point)" << endl;

    cout << endl << "=== Performance Comparison (n = 25) ===" << endl;
    int perfN = 25;
    timeFunction([&]() { return fibRecursive(perfN); }, "fibRecursive(25)");
    timeFunction([&]() { return fibMemo(perfN); }, "fibMemo(25)     ");
    timeFunction([&]() { return fibDP(perfN); }, "fibDP(25)       ");
    timeFunction([&]() { return fibIterative(perfN); }, "fibIterative(25)");
    timeFunction([&]() { return fibMatrix(perfN); }, "fibMatrix(25)   ");

    cout << endl << "=== Sum of first 10 Fibonacci numbers ===" << endl;
    cout << "Sum = " << fibSum(10) << " (should equal F(0)+...+F(9))" << endl;
    long long manualSum = 0;
    for (int i = 0; i < 10; i++) manualSum += fibIterative(i);
    cout << "Manual sum check: " << manualSum << endl;

    cout << endl << "=== Checking if numbers are Fibonacci numbers ===" << endl;
    for (long long candidate : {0LL, 1LL, 2LL, 4LL, 5LL, 8LL, 13LL, 20LL, 21LL, 100LL, 144LL}) {
        cout << candidate << " is Fibonacci: " << (isFibonacciNumber(candidate) ? "yes" : "no") << endl;
    }

    return 0;
}