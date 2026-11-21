#include <iostream>
#include <string>

// Part A: Predict-the-Output snippet
namespace CarDemo {
struct Engine {
    Engine()  { std::cout << "Engine "; }
    ~Engine() { std::cout << "~Engine "; }
};
struct Vehicle {
    Vehicle()  { std::cout << "Vehicle "; }
    ~Vehicle() { std::cout << "~Vehicle "; }
};
struct Car : Vehicle {
    Engine e;
    Car()  { std::cout << "Car "; }
    ~Car() { std::cout << "~Car "; }
};
} // namespace CarDemo

// Part B: Complete-the-Code snippet
namespace CompleteCodeDemo {
class Person {
protected:
    std::string name;
    int age;

public:
    Person(const std::string& n, int a) : name(n), age(a) {}
};

class Address {
    std::string city;
    int pin;

public:
    Address(const std::string& c, int p) : city(c), pin(p) {}
    std::string str() const { return city + "-" + std::to_string(pin); }
};

class Student : public Person {
    long long regNo;
    Address home;

public:
    // TODO 1: Initialise Person base, regNo and home Address in the initializer list
    Student(const std::string& n, int a, long long reg, const std::string& c, int p)
        : Person(n, a), regNo(reg), home(c, p) {}

    void print() const {
        std::cout << regNo << ' ' << name << " (" << age << ") " << home.str() << '\n';
    }
};
} // namespace CompleteCodeDemo

// Part C: Full Traced Virtual Inheritance + Member Classes (Address & Course)
class Address {
    std::string city;
    int pin;
public:
    Address(std::string c = "Chennai", int p = 600001) : city(std::move(c)), pin(p) {
        std::cout << "  [Ctor] Address(" << city << "-" << pin << ")\n";
    }
    ~Address() {
        std::cout << "  [Dtor] ~Address(" << city << ")\n";
    }
    std::string str() const { return city + "-" + std::to_string(pin); }
};

class Course {
    std::string code;
    std::string title;
public:
    Course(std::string c = "DSA0107", std::string t = "OOP in C++")
        : code(std::move(c)), title(std::move(t)) {
        std::cout << "  [Ctor] Course(" << code << ")\n";
    }
    ~Course() {
        std::cout << "  [Dtor] ~Course(" << code << ")\n";
    }
};

class Person {
protected:
    std::string name;
    int id;
    Address addr;
public:
    Person(std::string n = "Default", int i = 0, std::string city = "Chennai", int pin = 600001)
        : name(std::move(n)), id(i), addr(std::move(city), pin) {
        std::cout << "  [Ctor] Person(" << name << ")\n";
    }
    virtual ~Person() {
        std::cout << "  [Dtor] ~Person(" << name << ")\n";
    }
};

class Student : public virtual Person {
protected:
    Course enrolled;
public:
    Student(std::string n, int i, std::string city, int pin, std::string cCode, std::string cTitle)
        : Person(std::move(n), i, std::move(city), pin),
          enrolled(std::move(cCode), std::move(cTitle)) {
        std::cout << "  [Ctor] Student(" << name << ")\n";
    }
    ~Student() override {
        std::cout << "  [Dtor] ~Student(" << name << ")\n";
    }
};

class Staff : public virtual Person {
protected:
    std::string dept;
public:
    Staff(std::string n, int i, std::string city, int pin, std::string d)
        : Person(std::move(n), i, std::move(city), pin), dept(std::move(d)) {
        std::cout << "  [Ctor] Staff(" << dept << ")\n";
    }
    ~Staff() override {
        std::cout << "  [Dtor] ~Staff(" << dept << ")\n";
    }
};

class TeachingAssistant : public Student, public Staff {
    int labHours;
public:
    // Most-derived class directly initializes the virtual base Person
    TeachingAssistant(std::string n, int i, std::string city, int pin,
                      std::string cCode, std::string cTitle, std::string d, int hrs)
        : Person(std::move(n), i, std::move(city), pin),
          Student("", 0, "", 0, std::move(cCode), std::move(cTitle)),
          Staff("", 0, "", 0, std::move(d)),
          labHours(hrs) {
        std::cout << "  [Ctor] TeachingAssistant(labHours=" << labHours << ")\n";
    }
    ~TeachingAssistant() override {
        std::cout << "  [Dtor] ~TeachingAssistant()\n";
    }
};

// Non-virtual diamond counterpart for Task 3.4 sizeof comparison
struct NonVirtualStudent : Person { Course c; };
struct NonVirtualStaff : Person { std::string d; };
struct NonVirtualTA : NonVirtualStudent, NonVirtualStaff { int labHours; };

int main() {
    std::cout << "=== Task 3 Part A: Predict-the-Output Verification ===\n";
    {
        CarDemo::Car c;
        std::cout << "| ";
    }
    std::cout << "\n\n=== Task 3 Part B: Complete-the-Code Verification ===\n";
    CompleteCodeDemo::Student s("Kavya", 19, 192311001, "Salem", 636001);
    s.print();

    std::cout << "\n=== Task 3 Part C: Construction & Destruction Trace ===\n";
    std::cout << "--- 1. Student Lifecycle ---\n";
    {
        Student st("Riya", 101, "Chennai", 600077, "DSA0107", "OOP C++");
    }
    std::cout << "--- 2. Staff Lifecycle ---\n";
    {
        Staff sf("Dr. Nair", 202, "Coimbatore", 641001, "CSE");
    }
    std::cout << "--- 3. TeachingAssistant Lifecycle (Virtual Base) ---\n";
    {
        TeachingAssistant ta("Samrakshini", 192511172, "Chennai", 600127,
                             "DSA0107", "OOP C++", "CSE", 6);
    }

    std::cout << "\n--- Task 3.4: Virtual vs. Non-Virtual sizeof ---\n";
    std::cout << "sizeof(TeachingAssistant) [virtual]     = " << sizeof(TeachingAssistant) << " bytes\n";
    std::cout << "sizeof(NonVirtualTA)      [non-virtual] = " << sizeof(NonVirtualTA) << " bytes\n";
    return 0;
}
