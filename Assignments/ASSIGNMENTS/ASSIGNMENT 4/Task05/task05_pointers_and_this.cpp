#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class Cart {
    std::string items[10];
    double prices[10];
    int n = 0;
    double discount = 0; // percent

public:
    // TODO 1: add(): store item and price if there is space, and return *this for chaining
    Cart& add(const std::string& item, double price) {
        if (n < 10) {
            items[n] = item;
            prices[n] = price;
            ++n;
        }
        return *this;
    }

    // TODO 2: setDiscount(): disambiguate member from parameter with this->, return *this
    Cart& setDiscount(double discount) {
        this->discount = discount;
        return *this;
    }

    double total() const {
        double t = 0;
        for (int i = 0; i < n; ++i) t += prices[i];
        return t * (100.0 - discount) / 100.0;
    }
};

// Task 5.2: Find Cart with highest total using pointer arithmetic over array of Cart*
const Cart* findHighestCart(Cart* const* carts, int count) {
    if (carts == nullptr || count <= 0) return nullptr;
    const Cart* best = *carts;
    for (int i = 1; i < count; ++i) {
        const Cart* current = *(carts + i); // pointer arithmetic
        if (current != nullptr && current->total() > best->total()) {
            best = current;
        }
    }
    return best;
}

int main() {
    std::cout << std::fixed << std::setprecision(2);

    std::cout << "=== Task 5 Part A: Complete-the-Code & Method Chaining ===\n";
    Cart c;
    c.add("Rice", 60).add("Dal", 120).setDiscount(10);
    Cart* p = &c;
    p->add("Oil", 180);
    std::cout << p->total() << '\n';

    std::cout << "\n=== Task 5 Part B: Four Ways to Reach a Cart Object ===\n";
    // (a) Directly by object name
    Cart directCart;
    directCart.add("Milk", 55).add("Bread", 45);
    std::cout << "(a) Direct object total : Rs " << directCart.total() << '\n';

    // (b) Through a pointer using ->
    Cart* ptrCart = &directCart;
    ptrCart->add("Eggs", 80);
    std::cout << "(b) Pointer (->) total  : Rs " << ptrCart->total() << '\n';

    // (c) Through a reference
    Cart& refCart = directCart;
    refCart.setDiscount(5);
    std::cout << "(c) Reference total     : Rs " << refCart.total() << '\n';

    // (d) Dynamically allocated on heap with new and freed with delete
    Cart* heapCart = new Cart();
    heapCart->add("Olive Oil", 650).add("Almonds", 450).setDiscount(10);
    std::cout << "(d) Heap (new/delete)   : Rs " << heapCart->total() << '\n';

    std::cout << "\n=== Task 5 Part C: Highest Total Across Active Customer Pointers ===\n";
    Cart* activeCarts[3] = { &c, &directCart, heapCart };
    const Cart* topCart = findHighestCart(activeCarts, 3);
    if (topCart != nullptr) {
        std::cout << "Highest active cart total = Rs " << topCart->total() << '\n';
    }

    delete heapCart;
    return 0;
}
