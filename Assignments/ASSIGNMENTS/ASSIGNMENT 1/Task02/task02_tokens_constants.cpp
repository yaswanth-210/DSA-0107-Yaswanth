#include <iostream>

// Single source of truth using scoped enum class and compile-time constexpr constants
enum class Zone : int { Inner = 0, Middle = 1, Outer = 2 };

constexpr int ZONE_COUNT = 3;
constexpr long long BASE_FARE_PAISE[ZONE_COUNT] = {1000, 2000, 3000}; // Rs 10, Rs 20, Rs 30
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr int CONCESSION_PERCENT = 25;
constexpr long long MIN_BALANCE_PAISE = 5000; // Rs 50.00 reserve

constexpr int MORNING_PEAK_START = 8;
constexpr int MORNING_PEAK_END = 10;
constexpr int EVENING_PEAK_START = 17;
constexpr int EVENING_PEAK_END = 20;

constexpr long long calculateBasePaise(Zone z) {
    return BASE_FARE_PAISE[static_cast<int>(z)];
}

int main() {
    constexpr long long innerBase = calculateBasePaise(Zone::Inner);
    constexpr long long middleBase = calculateBasePaise(Zone::Middle);
    constexpr long long outerBase = calculateBasePaise(Zone::Outer);

    std::cout << "=== Namma Metro Compile-Time Fare Constants ===\n";
    std::cout << "Inner Base Fare  : " << innerBase << " paise (Rs " << innerBase / 100 << ")\n";
    std::cout << "Middle Base Fare : " << middleBase << " paise (Rs " << middleBase / 100 << ")\n";
    std::cout << "Outer Base Fare  : " << outerBase << " paise (Rs " << outerBase / 100 << ")\n";
    std::cout << "Peak Surcharge   : " << PEAK_SURCHARGE_PERCENT << "%\n";
    std::cout << "Concession Rate  : " << CONCESSION_PERCENT << "%\n";
    std::cout << "Minimum Reserve  : " << MIN_BALANCE_PAISE << " paise (Rs " << MIN_BALANCE_PAISE / 100 << ")\n";
    return 0;
}
