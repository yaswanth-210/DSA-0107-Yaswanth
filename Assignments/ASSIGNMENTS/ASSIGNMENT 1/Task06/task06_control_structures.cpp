#include <iomanip>
#include <iostream>

enum class Zone { Inner, Middle, Outer };

constexpr int BASE_PAISE[] = {1000, 2000, 3000};   // Rs 10, Rs 20, Rs 30
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;      // Rs 50 must remain on the card

bool isPeakHour(int hour)
{
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

long long fare(Zone z, bool peak)
{
    // TODO 1: Use a switch on z to pick the base fare from BASE_PAISE, then add
    //   PEAK_SURCHARGE_PERCENT % in peak hours using integer arithmetic only
    long long base = 0;
    switch (z) {
        case Zone::Inner:
            base = BASE_PAISE[0];
            break;
        case Zone::Middle:
            base = BASE_PAISE[1];
            break;
        case Zone::Outer:
            base = BASE_PAISE[2];
            break;
        default:
            return -1; // Invalid zone sentinel
    }
    if (peak) {
        base += (base * PEAK_SURCHARGE_PERCENT) / 100;
    }
    return base;
}

bool canTravel(long long balance, long long f)
{
    // TODO 2: Return true only if the balance left after paying the fare is at least
    //   MIN_BALANCE_PAISE
    return (f >= 0) && ((balance - f) >= MIN_BALANCE_PAISE);
}

int main()
{
    long long balance = 10000;                     // Rs 100.00 in paise
    struct Trip { Zone zone; bool peak; };
    Trip trips[] = {{Zone::Inner, false}, {Zone::Outer, true}, {Zone::Middle, false}};
    for (const Trip& t : trips) {
        long long f = fare(t.zone, t.peak);
        if (canTravel(balance, f)) {
            balance -= f;
            std::cout << "Fare Rs " << f / 100 << '.' << std::setw(2)
                      << std::setfill('0') << f % 100 << std::setfill(' ');
        } else {
            std::cout << "Denied (low balance)";
        }
        std::cout << "  balance Rs " << balance / 100 << '\n';
    }

    // Boundary checks: 10:00 AM (hour 10) and 8:00 PM (hour 20)
    std::cout << "\n--- Peak Boundary Checks ---\n";
    long long balanceBoundary = 12000;
    int testHours[] = {10, 20};
    for (int hr : testHours) {
        bool peak = isPeakHour(hr);
        long long f = fare(Zone::Outer, peak);
        if (canTravel(balanceBoundary, f)) {
            balanceBoundary -= f;
        }
        std::cout << "Hour " << hr << ":00 -> Peak=" << std::boolalpha << peak
                  << " | Fare Rs " << f / 100 << '.' << std::setw(2)
                  << std::setfill('0') << f % 100 << std::setfill(' ')
                  << " | Remaining Rs " << balanceBoundary / 100 << '\n';
    }
    return 0;
}
