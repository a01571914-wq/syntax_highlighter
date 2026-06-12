#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
using namespace std;

// ============================================================
//  Matrix Operations - Multiplication, addition, transpose,
//  determinant, identity, and a Matrix class wrapper
// ============================================================

typedef vector<vector<double>> Matrix;

// Create an n x m matrix filled with a given value
Matrix createMatrix(int rows, int cols, double fill = 0.0) {
    return Matrix(rows, vector<double>(cols, fill));
}

// Create an n x n identity matrix
Matrix identity(int n) {
    Matrix I = createMatrix(n, n);
    for (int i = 0; i < n; i++) I[i][i] = 1.0;
    return I;
}

// Matrix addition - requires matching dimensions
Matrix add(const Matrix& A, const Matrix& B) {
    if (A.size() != B.size() || A[0].size() != B[0].size())
        throw invalid_argument("Matrix dimensions must match for addition");

    int rows = (int)A.size(), cols = (int)A[0].size();
    Matrix C = createMatrix(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

// Matrix subtraction
Matrix subtract(const Matrix& A, const Matrix& B) {
    if (A.size() != B.size() || A[0].size() != B[0].size())
        throw invalid_argument("Matrix dimensions must match for subtraction");

    int rows = (int)A.size(), cols = (int)A[0].size();
    Matrix C = createMatrix(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}

// Matrix multiplication - A is m x n, B is n x p, result is m x p
Matrix multiply(const Matrix& A, const Matrix& B) {
    int m = (int)A.size();
    int n = (int)A[0].size();
    int p = (int)B[0].size();

    if ((int)B.size() != n)
        throw invalid_argument("Matrix dimensions are incompatible for multiplication");

    Matrix C = createMatrix(m, p);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < p; j++)
            for (int k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

// Scalar multiplication
Matrix scalarMultiply(const Matrix& A, double scalar) {
    int rows = (int)A.size(), cols = (int)A[0].size();
    Matrix C = createMatrix(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            C[i][j] = A[i][j] * scalar;
    return C;
}

// Transpose: rows become columns and vice versa
Matrix transpose(const Matrix& A) {
    int rows = (int)A.size(), cols = (int)A[0].size();
    Matrix T = createMatrix(cols, rows);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            T[j][i] = A[i][j];
    return T;
}

// Compute the determinant of a square matrix using cofactor expansion (recursive)
double determinant(const Matrix& A) {
    int n = (int)A.size();
    if (n == 1) return A[0][0];
    if (n == 2) return A[0][0] * A[1][1] - A[0][1] * A[1][0];

    double det = 0.0;
    for (int col = 0; col < n; col++) {
        // Build the minor matrix by removing row 0 and column 'col'
        Matrix minor = createMatrix(n - 1, n - 1);
        for (int i = 1; i < n; i++) {
            int minorCol = 0;
            for (int j = 0; j < n; j++) {
                if (j == col) continue;
                minor[i - 1][minorCol] = A[i][j];
                minorCol++;
            }
        }
        double sign = (col % 2 == 0) ? 1.0 : -1.0;
        det += sign * A[0][col] * determinant(minor);
    }
    return det;
}

// Raise a square matrix to an integer power using exponentiation by squaring
Matrix power(const Matrix& A, int exp) {
    int n = (int)A.size();
    Matrix result = identity(n);
    Matrix base = A;
    while (exp > 0) {
        if (exp % 2 == 1) result = multiply(result, base);
        base = multiply(base, base);
        exp /= 2;
    }
    return result;
}

// Check whether a matrix is square
bool isSquare(const Matrix& A) {
    return A.size() == A[0].size();
}

// Check whether a matrix is symmetric (equals its transpose)
bool isSymmetric(const Matrix& A) {
    if (!isSquare(A)) return false;
    int n = (int)A.size();
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (A[i][j] != A[j][i]) return false;
    return true;
}

// Compute the trace (sum of diagonal elements) of a square matrix
double trace(const Matrix& A) {
    double sum = 0.0;
    for (size_t i = 0; i < A.size(); i++) sum += A[i][i];
    return sum;
}

void printMatrix(const Matrix& M, const string& label = "") {
    if (!label.empty()) cout << label << ":" << endl;
    for (const auto& row : M) {
        for (double val : row)
            cout << setw(8) << val << " ";
        cout << endl;
    }
}

int main() {
    Matrix A = {{1, 2}, {3, 4}};
    Matrix B = {{5, 6}, {7, 8}};

    printMatrix(A, "Matrix A");
    printMatrix(B, "Matrix B");

    cout << endl << "=== Basic Operations ===" << endl;
    printMatrix(add(A, B), "A + B");
    printMatrix(subtract(A, B), "A - B");
    printMatrix(multiply(A, B), "A * B");
    printMatrix(scalarMultiply(A, 2.0), "A * 2");

    cout << endl << "=== Transpose ===" << endl;
    Matrix C = {{1, 2, 3}, {4, 5, 6}};
    printMatrix(C, "Matrix C (2x3)");
    printMatrix(transpose(C), "Transpose of C (3x2)");

    cout << endl << "=== Identity Matrix ===" << endl;
    printMatrix(identity(3), "3x3 Identity");

    cout << endl << "=== Determinant ===" << endl;
    cout << "det(A) = " << determinant(A) << endl;
    Matrix D = {{6, 1, 1}, {4, -2, 5}, {2, 8, 7}};
    printMatrix(D, "Matrix D (3x3)");
    cout << "det(D) = " << determinant(D) << endl;

    Matrix Identity4 = identity(4);
    cout << "det(Identity4) = " << determinant(Identity4) << endl;

    cout << endl << "=== Matrix Power ===" << endl;
    Matrix square = {{1, 1}, {1, 0}}; // Fibonacci matrix
    printMatrix(power(square, 5), "Fibonacci-style matrix ^ 5");

    cout << endl << "=== Properties ===" << endl;
    cout << "Is A square? " << (isSquare(A) ? "yes" : "no") << endl;
    cout << "Is C square? " << (isSquare(C) ? "yes" : "no") << endl;

    Matrix symMatrix = {{1, 2, 3}, {2, 5, 6}, {3, 6, 9}};
    printMatrix(symMatrix, "Symmetric candidate");
    cout << "Is symmetric? " << (isSymmetric(symMatrix) ? "yes" : "no") << endl;
    cout << "Is A symmetric? " << (isSymmetric(A) ? "yes" : "no") << endl;

    cout << endl << "=== Trace ===" << endl;
    cout << "trace(A) = " << trace(A) << endl;
    cout << "trace(D) = " << trace(D) << endl;

    cout << endl << "=== Error Handling ===" << endl;
    try {
        Matrix E = {{1, 2, 3}};
        multiply(A, E); // incompatible dimensions: A is 2x2, E is 1x3
    } catch (const invalid_argument& e) {
        cout << "Caught error: " << e.what() << endl;
    }

    try {
        Matrix F = {{1, 2}, {3, 4}, {5, 6}}; // 3x2
        add(A, F); // A is 2x2, F is 3x2
    } catch (const invalid_argument& e) {
        cout << "Caught error: " << e.what() << endl;
    }

    return 0;
}