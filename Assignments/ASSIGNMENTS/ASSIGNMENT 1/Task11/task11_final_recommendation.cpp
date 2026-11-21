#include <iostream>
#include <iomanip>
#include <string>

enum class Zone : int { Inner = 0, Middle = 1, Outer = 2 };
enum class RiderCategory { Regular, Student, Senior };

class FarePolicy {
private:
    static constexpr int NUM_ZONES = 3;
    static constexpr long long BASE_FARES[NUM_ZONES] = {1000, 2000, 3000};
    static constexpr int PEAK_PCT = 20;
    static constexpr int CONCESSION_PCT = 25;

public:
    static constexpr bool isPeak(int hour) {
        return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    }

    static constexpr long long quote(Zone z, int hour, RiderCategory cat) {
        long long amount = BASE_FARES[static_cast<int>(z)];
        if (isPeak(hour)) {
            amount += (amount * PEAK_PCT) / 100;
        }
        if (cat != RiderCategory::Regular) {
            amount -= (amount * CONCESSION_PCT) / 100;
        }
        return amount;
    }
};

class SmartCard {
private:
    std::string id;
    long long balancePaise;
    static constexpr long long MIN_RESERVE = 5000;

public:
    SmartCard(std::string cardId, long long startPaise)
        : id(std::move(cardId)), balancePaise(startPaise >= 0 ? startPaise : 0) {}

    bool tapInOut(Zone z, int hour, RiderCategory cat, long long& chargedOut) {
        if (hour < 0 || hour > 23) return false;
        long long fare = FarePolicy::quote(z, hour, cat);
        if ((balancePaise - fare) < MIN_RESERVE) return false;
        balancePaise -= fare;
        chargedOut = fare;
        return true;
    }

    long long getBalance() const { return balancePaise; }
};

int main() {
    SmartCard card("NM-2026-991", 15000);
    long long charged = 0;
    if (card.tapInOut(Zone::Outer, 9, RiderCategory::Student, charged)) {
        std::cout << "Gate Open | Charged: Rs " << charged / 100 << '.'
                  << std::setw(2) << std::setfill('0') << charged % 100
                  << " | Remaining: Rs " << card.getBalance() / 100 << '.'
                  << std::setw(2) << card.getBalance() % 100 << '\n';
    }
    return 0;
}
