#include <iomanip>
#include <iostream>
#include <string>

// Part A: Exact Predict-the-Output verification snippet
namespace PredictDemo {
struct Person  { Person()  { std::cout << "Person "; } };
struct Student : virtual Person { Student() { std::cout << "Student "; } };
struct Staff   : virtual Person { Staff()   { std::cout << "Staff "; } };
struct TeachingAssistant : Student, Staff {
    TeachingAssistant() { std::cout << "TA "; }
};
} // namespace PredictDemo

// Part B: Full University People Management Hierarchy
class Person {
private:
    std::string name;
    int id;
    std::string phone;

protected:
    std::string contactSummary() const {
        return name + " (ID:" + std::to_string(id) + ", Ph:" + phone + ")";
    }

public:
    Person(std::string n = "Unknown", int personId = 0, std::string ph = "N/A")
        : name(std::move(n)), id(personId), phone(std::move(ph)) {}

    virtual ~Person() = default;

    const std::string& getName() const { return name; }
    int getId() const { return id; }
    const std::string& getPhone() const { return phone; }
};

class Student : public virtual Person {
private:
    std::string programme;
    double cgpa;

public:
    Student(std::string n, int personId, std::string ph, std::string prog, double gpa)
        : Person(std::move(n), personId, std::move(ph)),
          programme(std::move(prog)), cgpa(gpa) {}

    const std::string& getProgramme() const { return programme; }
    double getCgpa() const { return cgpa; }
};

class Staff : public virtual Person {
private:
    std::string department;
    double salary;

public:
    Staff(std::string n, int personId, std::string ph, std::string dept, double sal)
        : Person(std::move(n), personId, std::move(ph)),
          department(std::move(dept)), salary(sal) {}

    const std::string& getDepartment() const { return department; }
    double getSalary() const { return salary; }
};

class TeachingAssistant : public Student, public Staff {
private:
    std::string assignedCourse;

public:
    TeachingAssistant(std::string n, int personId, std::string ph,
                      std::string prog, double gpa,
                      std::string dept, double sal, std::string course)
        : Person(std::move(n), personId, std::move(ph)),
          Student("", 0, "", std::move(prog), gpa),
          Staff("", 0, "", std::move(dept), sal),
          assignedCourse(std::move(course)) {}

    void display() const {
        std::cout << "TA: " << contactSummary()
                  << " | Prog=" << getProgramme() << " (CGPA " << getCgpa() << ")"
                  << " | Dept=" << getDepartment() << " | Stipend=Rs " << getSalary()
                  << " | Course=" << assignedCourse << '\n';
    }
};

class ResearchScholar : public Student {
private:
    double fellowshipAmount;
    std::string thesisArea;

public:
    ResearchScholar(std::string n, int personId, std::string ph,
                    std::string prog, double gpa,
                    double fellowship, std::string area)
        : Person(std::move(n), personId, std::move(ph)),
          Student("", 0, "", std::move(prog), gpa),
          fellowshipAmount(fellowship), thesisArea(std::move(area)) {}

    void display() const {
        std::cout << "Scholar: " << contactSummary()
                  << " | Prog=" << getProgramme() << " | Fellowship=Rs " << fellowshipAmount
                  << " | Thesis=" << thesisArea << '\n';
    }
};

// Alternative Concept: Composition (has-a)
struct ComposedTA {
    Student asStudent;
    Staff asStaff;
};

int main() {
    std::cout << "=== Task 2 Part A: Predict-the-Output Verification ===\n";
    PredictDemo::TeachingAssistant demoTa;
    std::cout << '\n';

    std::cout << "\n=== Task 2 Part B: University People Hierarchy ===\n";
    std::cout << std::fixed << std::setprecision(2);
    TeachingAssistant ta("Samrakshini G", 192511172, "9876543210",
                         "B.Tech CSE", 9.45, "Computer Science", 18000.0, "DSA0107 C++");
    ResearchScholar rs("Arvind K", 192400112, "9123456780",
                       "Ph.D. AI", 9.60, 37000.0, "Formal Verification");
    ta.display();
    rs.display();

    std::cout << "\n--- Size Comparison (Virtual Base vs. Composition) ---\n";
    std::cout << "sizeof(Person)            = " << sizeof(Person) << " bytes\n";
    std::cout << "sizeof(TeachingAssistant) = " << sizeof(TeachingAssistant) << " bytes (1 shared Person)\n";
    std::cout << "sizeof(ComposedTA)        = " << sizeof(ComposedTA) << " bytes (2 duplicate Persons)\n";
    return 0;
}
