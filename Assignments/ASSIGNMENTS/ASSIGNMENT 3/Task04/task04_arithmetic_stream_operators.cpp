#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

class Money {
    long long paise;

public:
    Money(long long rupees = 0, int p = 0) : paise(rupees * 100 + p) {}

    // TODO 1: Overload + as a member function that returns a new Money object
    Money operator+(const Money& rhs) const {
        Money res;
        res.paise = paise + rhs.paise;
        return res;
    }

    Money operator-(const Money& rhs) const {
        if (paise < rhs.paise) {
            throw std::underflow_error("Negative Money amount not permitted in billing");
        }
        Money res;
        res.paise = paise - rhs.paise;
        return res;
    }

    Money& operator+=(const Money& rhs) {
        paise += rhs.paise;
        return *this;
    }

    // TODO 2: Overload == as a member, and << as a friend that prints "Rs 200.00"
    bool operator==(const Money& rhs) const { return paise == rhs.paise; }
    bool operator<(const Money& rhs) const { return paise < rhs.paise; }

    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        long long absP = m.paise < 0 ? -m.paise : m.paise;
        if (m.paise < 0) os << '-';
        os << "Rs " << (absP / 100) << '.' << std::setw(2) << std::setfill('0') << (absP % 100);
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Money& m) {
        long long r = 0;
        int p = 0;
        if (is >> r >> p) {
            m.paise = r * 100 + p;
        }
        return is;
    }
};

int main() {
    Money a(120, 75), b(79, 25);
    Money c = a + b;
    std::cout << c << '\n' << std::boolalpha << (c == Money(200)) << '\n';

    Money carryTest = Money(0, 75) + Money(0, 50);
    std::cout << "Carry test (Rs 0.75 + Rs 0.50) = " << carryTest << '\n';

    Money subtotal(0, 0);
    subtotal += Money(45, 50);
    subtotal += Money(19, 75);
    Money discount(10, 0);
    Money finalBill = subtotal - discount;
    std::cout << "Billing calculation (subtotal - discount) = " << finalBill
              << " | Discount < Subtotal? " << (discount < subtotal) << '\n';

    try {
        Money bad = Money(10, 0) - Money(25, 0);
        std::cout << bad << '\n';
    } catch (const std::underflow_error& e) {
        std::cout << "Negative subtraction prevented: " << e.what() << '\n';
    }

    std::istringstream iss("150 45");
    Money parsed;
    iss >> parsed;
    std::cout << "Parsed from stream ('150 45'): " << parsed << '\n';

    Money exactSum(0, 0), step(0, 10);
    double doubleSum = 0.0;
    for (int i = 0; i < 1000000; ++i) {
        exactSum += step;
        doubleSum += 0.10;
    }
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "1,000,000 x Rs 0.10 -> Money: " << exactSum
              << " | double: Rs " << doubleSum
              << " | Drift: Rs " << (doubleSum - 100000.0) << '\n';

    return 0;
}
