#include <iostream>
#include <stdexcept>
#include <string>

class Book {
private:
    std::string title;
    std::string author;
    int copies;

    static int validateCopies(int c) {
        if (c < 0) {
            throw std::invalid_argument("Invalid number of copies: cannot be negative (" + std::to_string(c) + ")");
        }
        return c;
    }

public:
    // 1. Target parameterised constructor (full details) using member initializer list
    Book(const std::string& bookTitle, const std::string& bookAuthor, int numCopies)
        : title(bookTitle), author(bookAuthor), copies(validateCopies(numCopies)) {}

    // 2. Overloaded constructor: donated book with title only (delegates to 3-arg constructor)
    explicit Book(const std::string& donatedTitle)
        : Book(donatedTitle, "Anonymous Donor", 1) {}

    // 3. Default constructor: empty record to be filled later (delegates to 3-arg constructor)
    Book()
        : Book("Untitled Record", "Unknown Author", 0) {}

    void setDetails(const std::string& t, const std::string& a, int c) {
        int valid = validateCopies(c);
        title = t;
        author = a;
        copies = valid;
    }

    void print() const {
        std::cout << "Book[Title=\"" << title << "\", Author=\"" << author
                  << "\", Copies=" << copies << "]\n";
    }
};

void registerBook(const Book& b) {
    b.print();
}

// Demonstrates -Wreorder when member initializer list order != declaration order
class ReorderDemo {
    int first;
    int second;
public:
    ReorderDemo(int x) : second(x), first(second + 1) {}
    void show() const {
        std::cout << "ReorderDemo -> first=" << first << " (uninitialized garbage), second=" << second << '\n';
    }
};

int main() {
    std::cout << "=== Q1: Library Book Records (Constructors & Delegation) ===\n";

    Book b1("The C++ Programming Language", "Bjarne Stroustrup", 5);
    Book b2("Clean Code");
    Book b3;

    std::cout << "Constructed objects:\n  ";
    b1.print();
    std::cout << "  ";
    b2.print();
    std::cout << "  ";
    b3.print();

    b3.setDetails("Design Patterns", "Gamma et al.", 3);
    std::cout << "After filling empty record b3:\n  ";
    b3.print();

    registerBook(Book("Effective Modern C++"));

    Book shelf[2];
    std::cout << "Array of 2 default-constructed Books created successfully.\n";

    std::cout << "\n--- Testing Invalid (Negative) Copies ---\n";
    try {
        Book invalidBook("Corrupted Record", "Unknown", -4);
        invalidBook.print();
    } catch (const std::invalid_argument& ex) {
        std::cout << "Caught expected exception: " << ex.what() << '\n';
    }

    std::cout << "\n--- Demonstrating Member Initialization Order ---\n";
    ReorderDemo rd(10);
    rd.show();

    return 0;
}
