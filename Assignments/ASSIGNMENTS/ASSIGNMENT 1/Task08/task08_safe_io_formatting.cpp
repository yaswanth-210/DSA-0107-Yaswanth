#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

void printRupees(long long paise) {
    std::cout << "Rs " << (paise / 100) << '.'
              << std::setw(2) << std::setfill('0') << (paise % 100)
              << std::setfill(' ');
}

int readValidatedInt(std::istream& in, const std::string& prompt, int minVal, int maxVal) {
    int value = 0;
    while (true) {
        std::cout << prompt;
        if (in >> value) {
            if (value >= minVal && value <= maxVal) {
                return value;
            }
            std::cout << "  [Error] Out of range (" << minVal << ".." << maxVal << "). Retry.\n";
        } else {
            if (in.eof()) return minVal;
            std::cout << "  [Error] Non-numeric input detected (cin.fail()). Clearing stream...\n";
            in.clear();
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

int main() {
    int zoneIdx = readValidatedInt(std::cin, "Enter Zone (0=Inner, 1=Middle, 2=Outer): ", 0, 2);
    int hour = readValidatedInt(std::cin, "Enter Entry Hour (0-23): ", 0, 23);

    constexpr long long BASE[] = {1000, 2000, 3000};
    const std::string ZNAMES[] = {"Inner", "Middle", "Outer"};
    bool peak = (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    long long fare = BASE[zoneIdx] + (peak ? (BASE[zoneIdx] * 20) / 100 : 0);
    long long balance = 10000 - fare;

    std::cout << "\n+------------------------------------------+\n";
    std::cout << "|       NAMMA METRO JOURNEY RECEIPT        |\n";
    std::cout << "+------------------------------------------+\n";
    std::cout << "| Zone      : " << std::left << std::setw(29) << ZNAMES[zoneIdx] << "|\n";
    std::cout << "| Time      : " << std::setw(2) << std::setfill('0') << hour << ":00"
              << std::setfill(' ') << (peak ? " (PEAK)               " : " (OFF-PEAK)           ") << "|\n";
    std::cout << "| Fare Paid : "; printRupees(fare); std::cout << std::setw(21) << "" << "|\n";
    std::cout << "| Balance   : "; printRupees(balance); std::cout << std::setw(20) << "" << "|\n";
    std::cout << "+------------------------------------------+\n";
    return 0;
}
