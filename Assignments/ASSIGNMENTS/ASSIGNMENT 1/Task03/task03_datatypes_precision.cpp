#include <iostream>
#include <iomanip>
#include <climits>

enum class Zone { Inner, Middle, Outer };

int main() {
    std::cout << "=== System Data Type Sizes (bytes) ===\n";
    std::cout << "bool        : " << sizeof(bool) << "\n";
    std::cout << "char        : " << sizeof(char) << "\n";
    std::cout << "int         : " << sizeof(int) << " (INT_MAX = " << INT_MAX << ")\n";
    std::cout << "unsigned int: " << sizeof(unsigned int) << " (UINT_MAX = " << UINT_MAX << ")\n";
    std::cout << "long long   : " << sizeof(long long) << " (LLONG_MAX = " << LLONG_MAX << ")\n";
    std::cout << "float       : " << sizeof(float) << "\n";
    std::cout << "double      : " << sizeof(double) << "\n";
    std::cout << "enum Zone   : " << sizeof(Zone) << "\n\n";

    // Network scale overflow check: 3,000,000 journeys * 3600 paise (Rs 36 peak Outer)
    long long dailyJourneys = 3000000LL;
    long long maxFarePaise = 3600LL;
    long long networkTotalPaise = dailyJourneys * maxFarePaise;
    std::cout << "Network Total (3M peak Outer journeys): " << networkTotalPaise << " paise\n";
    std::cout << "Exceeds INT_MAX (" << INT_MAX << ")? "
              << (networkTotalPaise > INT_MAX ? "YES -> Overflow in 32-bit int!" : "NO") << "\n\n";

    // Rounding drift demonstration: summing Rs 10.10 (1010 paise) 3,000,000 times
    double doubleSum = 0.0;
    long long paiseSum = 0;
    for (int i = 0; i < 3000000; ++i) {
        doubleSum += 10.10;
        paiseSum += 1010;
    }
    double exactRupees = static_cast<double>(paiseSum) / 100.0;
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Expected Exact Total : Rs " << exactRupees << "\n";
    std::cout << "Accumulated double   : Rs " << doubleSum << "\n";
    std::cout << "Floating-Point Drift : Rs " << (doubleSum - exactRupees) << "\n";
    return 0;
}
