#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Base class
class Animal {
protected:
    string name;

public:
    Animal(string n) {
        name = n;
    }

    virtual void speak() {
        cout << "Animal sound" << endl;
    }
};

// Derived class
class Dog : public Animal {

public:
    Dog(string n) : Animal(n) {}

    void speak() override {
        cout << name << " says woof!" << endl;
    }
};

// Function template
template <typename T>
T add(T a, T b) {
    return a + b;
}

/*
   Main program
*/
int main() {

    int age = 18;
    float average = 91.7;
    char grade = 'A';

    string user;

    cout << "Enter your name: ";
    cin >> user;

    cout << "Welcome " << user << endl;

    // Vector example
    vector<int> numbers;

    for (int i = 0; i < 5; i++) {
        numbers.push_back(i * 2);
    }

    cout << "Numbers: ";

    for (int n : numbers) {
        cout << n << " ";
    }

    cout << endl;

    // Conditional
    if (average >= 90 && grade == 'A') {
        cout << "Excellent!" << endl;
    }
    else {
        cout << "Good job!" << endl;
    }

    // Switch statement
    switch(age) {

        case 18:
            cout << "You are 18 years old." << endl;
            break;

        default:
            cout << "Age not recognized." << endl;
    }

    // Object example
    Dog d("Rocky");
    d.speak();

    // Template function
    int result = add<int>(5, 10);

    cout << "Result: " << result << endl;

    // Try catch
    try {
        int x = 10;
        int y = 0;

        if (y == 0) {
            throw runtime_error("Division by zero");
        }

        cout << x / y << endl;
    }

    catch(exception& e) {
        cout << e.what() << endl;
    }

    return 0;
}