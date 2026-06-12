#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

// ============================================================
//  Inheritance and Polymorphism - A small zoo simulation
//  demonstrating abstract base classes, virtual functions,
//  multiple inheritance via interfaces, and operator overloading
// ============================================================

// Interface for animals that can swim
class Swimmer {
public:
    virtual void swim() const = 0;
    virtual ~Swimmer() {}
};

// Interface for animals that can fly
class Flyer {
public:
    virtual void fly() const = 0;
    virtual ~Flyer() {}
};

class Animal {
protected:
    string name;
    int age;
    double weight; // kilograms

public:
    Animal(const string& n, int a, double w) : name(n), age(a), weight(w) {}

    virtual void speak() const = 0;
    virtual string getType() const = 0;

    // Virtual function with a default implementation that subclasses can override
    virtual double dailyFoodKg() const {
        return weight * 0.02; // generic default: 2% of body weight
    }

    void info() const {
        cout << getType() << " - Name: " << name
             << ", Age: " << age
             << ", Weight: " << weight << "kg"
             << ", Daily food: " << dailyFoodKg() << "kg" << endl;
    }

    const string& getName() const { return name; }
    int getAge() const { return age; }
    double getWeight() const { return weight; }

    void birthday() { age++; }

    // Overload comparison operators so animals can be sorted by weight
    bool operator<(const Animal& other) const {
        return weight < other.weight;
    }

    virtual ~Animal() { }
};

class Dog : public Animal {
public:
    Dog(const string& n, int a, double w) : Animal(n, a, w) {}
    void speak() const override { cout << name << " says: Woof!" << endl; }
    string getType() const override { return "Dog"; }
    double dailyFoodKg() const override { return weight * 0.025; }
};

class Cat : public Animal {
public:
    Cat(const string& n, int a, double w) : Animal(n, a, w) {}
    void speak() const override { cout << name << " says: Meow!" << endl; }
    string getType() const override { return "Cat"; }
    double dailyFoodKg() const override { return weight * 0.03; }
};

// Bird implements both Animal and Flyer (multiple inheritance)
class Bird : public Animal, public Flyer {
public:
    Bird(const string& n, int a, double w) : Animal(n, a, w) {}
    void speak() const override { cout << name << " says: Tweet!" << endl; }
    string getType() const override { return "Bird"; }
    void fly() const override { cout << name << " flaps its wings and flies away!" << endl; }
};

// Duck implements Animal, Flyer, and Swimmer (multiple inheritance)
class Duck : public Animal, public Flyer, public Swimmer {
public:
    Duck(const string& n, int a, double w) : Animal(n, a, w) {}
    void speak() const override { cout << name << " says: Quack!" << endl; }
    string getType() const override { return "Duck"; }
    void fly() const override { cout << name << " flies low over the pond." << endl; }
    void swim() const override { cout << name << " paddles gracefully in the water." << endl; }
};

// Fish implements Animal and Swimmer
class Fish : public Animal, public Swimmer {
public:
    Fish(const string& n, int a, double w) : Animal(n, a, w) {}
    void speak() const override { cout << name << " makes no sound (fish don't speak)." << endl; }
    string getType() const override { return "Fish"; }
    void swim() const override { cout << name << " glides through the water." << endl; }
    double dailyFoodKg() const override { return weight * 0.01; }
};

// A specialized dog breed demonstrating multi-level inheritance
class ServiceDog : public Dog {
private:
    string specialization;

public:
    ServiceDog(const string& n, int a, double w, const string& spec)
        : Dog(n, a, w), specialization(spec) {}

    void speak() const override {
        cout << name << " (" << specialization << " service dog) says: Woof! (on duty)" << endl;
    }

    void perform() const {
        cout << name << " performs its " << specialization << " duties." << endl;
    }
};

// A zoo that owns animals via smart pointers (demonstrating ownership and polymorphism)
class Zoo {
private:
    vector<unique_ptr<Animal>> animals;

public:
    void addAnimal(unique_ptr<Animal> animal) {
        animals.push_back(move(animal));
    }

    void allSpeak() const {
        for (const auto& a : animals) a->speak();
    }

    void allInfo() const {
        for (const auto& a : animals) a->info();
    }

    double totalDailyFood() const {
        double total = 0;
        for (const auto& a : animals) total += a->dailyFoodKg();
        return total;
    }

    // Demonstrate dynamic_cast to find animals that implement an interface
    void makeFliersFly() const {
        for (const auto& a : animals) {
            if (auto* flyer = dynamic_cast<Flyer*>(a.get())) {
                flyer->fly();
            }
        }
    }

    void makeSwimmersSwim() const {
        for (const auto& a : animals) {
            if (auto* swimmer = dynamic_cast<Swimmer*>(a.get())) {
                swimmer->swim();
            }
        }
    }

    Animal* heaviest() const {
        if (animals.empty()) return nullptr;
        Animal* heaviestSoFar = animals[0].get();
        for (const auto& a : animals)
            if (heaviestSoFar->getWeight() < a->getWeight())
                heaviestSoFar = a.get();
        return heaviestSoFar;
    }

    size_t count() const { return animals.size(); }
};

int main() {
    cout << "=== Building the Zoo ===" << endl;
    Zoo zoo;
    zoo.addAnimal(make_unique<Dog>("Rex", 3, 25.0));
    zoo.addAnimal(make_unique<Cat>("Whiskers", 5, 4.5));
    zoo.addAnimal(make_unique<Bird>("Tweety", 2, 0.05));
    zoo.addAnimal(make_unique<Duck>("Donald", 1, 1.2));
    zoo.addAnimal(make_unique<Fish>("Nemo", 1, 0.2));
    zoo.addAnimal(make_unique<ServiceDog>("Buddy", 4, 28.0, "guide"));

    cout << "Zoo has " << zoo.count() << " animals." << endl;

    cout << endl << "=== All Animals Speak ===" << endl;
    zoo.allSpeak();

    cout << endl << "=== Animal Info (polymorphic dailyFoodKg) ===" << endl;
    zoo.allInfo();

    cout << endl << "=== Total Daily Food Required ===" << endl;
    cout << "Total: " << zoo.totalDailyFood() << " kg" << endl;

    cout << endl << "=== Animals That Can Fly ===" << endl;
    zoo.makeFliersFly();

    cout << endl << "=== Animals That Can Swim ===" << endl;
    zoo.makeSwimmersSwim();

    cout << endl << "=== Finding the Heaviest Animal ===" << endl;
    Animal* heavy = zoo.heaviest();
    if (heavy) {
        cout << "Heaviest animal: " << heavy->getName()
             << " (" << heavy->getType() << "), weight: " << heavy->getWeight() << "kg" << endl;
    }

    cout << endl << "=== Service Dog Specific Behavior ===" << endl;
    ServiceDog buddy("Buddy", 4, 28.0, "search-and-rescue");
    buddy.speak();
    buddy.perform();
    buddy.info(); // still uses Dog's dailyFoodKg through inheritance

    cout << endl << "=== Sorting Animals by Weight ===" << endl;
    vector<Animal*> sortable = {
        new Dog("A", 1, 20.0),
        new Cat("B", 1, 4.0),
        new Bird("C", 1, 0.1),
        new Fish("D", 1, 0.3)
    };
    sort(sortable.begin(), sortable.end(), [](Animal* a, Animal* b) { return *a < *b; });
    for (Animal* a : sortable) {
        cout << "  " << a->getType() << " (" << a->getName() << "): " << a->getWeight() << "kg" << endl;
    }
    for (Animal* a : sortable) delete a;

    cout << endl << "=== Birthdays ===" << endl;
    Dog puppy("Buster", 0, 5.0);
    cout << puppy.getName() << " is " << puppy.getAge() << " years old." << endl;
    puppy.birthday();
    puppy.birthday();
    cout << puppy.getName() << " is now " << puppy.getAge() << " years old." << endl;

    return 0;
}