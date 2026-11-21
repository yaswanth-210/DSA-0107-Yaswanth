#include <iostream>
#include <iomanip>
#include <string>

// Adapted Design: 4 Zones (including Airport = Rs 60), Non-stacking 10% Weekend Discount, 2-Zone Monthly Pass
enum class Zone : int { Inner = 0, Middle = 1, Outer = 2, Airport = 3 };
enum class PassengerType { Regular, Student, Senior };

class FareTable {
private:
    static constexpr int ZONE_COUNT = 4;
    static constexpr long long BASE_PAISE[ZONE_COUNT] = {1000, 2000, 3000, 6000};
    static constexpr int PEAK_SURCHARGE_PCT = 20;
    static constexpr int WEEKEND_DISCOUNT_PCT = 10;
    static constexpr int CONCESSION_PCT = 25;

public:
    static bool isPeakHour(int hour, bool isWeekend) {
        if (isWeekend) return false; // Peak surcharge does not apply on weekends
        return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    }

    long long computeFare(Zone z, int hour, bool isWeekend, PassengerType pType) const {
        long long fare = BASE_PAISE[static_cast<int>(z)];
        if (isWeekend) {
            // Mutually exclusive: 10% weekend discount instead of peak surcharge
            fare -= (fare * WEEKEND_DISCOUNT_PCT) / 100;
        } else if (isPeakHour(hour, false)) {
            fare += (fare * PEAK_SURCHARGE_PCT) / 100;
        }

        if (pType != PassengerType::Regular) {
            fare -= (fare * CONCESSION_PCT) / 100;
        }
        return fare;
    }
};

class MonthlyPass {
private:
    bool active;
    Zone allowedZone1;
    Zone allowedZone2;
    int validUntilDay;

public:
    MonthlyPass() : active(false), allowedZone1(Zone::Inner), allowedZone2(Zone::Inner), validUntilDay(0) {}
    MonthlyPass(Zone z1, Zone z2, int expiryDay)
        : active(true), allowedZone1(z1), allowedZone2(z2), validUntilDay(expiryDay) {}

    bool covers(Zone z, int currentDay) const {
        return active && (currentDay <= validUntilDay) && (z == allowedZone1 || z == allowedZone2);
    }
};

class SmartCard {
private:
    std::string cardId;
    long long balancePaise;
    MonthlyPass pass;
    static constexpr long long MIN_RESERVE_PAISE = 5000;

public:
    SmartCard(std::string id, long long initialPaise, MonthlyPass p = MonthlyPass())
        : cardId(std::move(id)), balancePaise(initialPaise >= 0 ? initialPaise : 0), pass(p) {}

    bool processJourney(Zone z, int hour, bool isWeekend, int dayOfMonth,
                        PassengerType pType, const FareTable& table, long long& chargedOut) {
        if (pass.covers(z, dayOfMonth)) {
            chargedOut = 0; // Covered by monthly pass; Rs 50 reserve untouched
            return true;
        }
        long long fare = table.computeFare(z, hour, isWeekend, pType);
        if ((balancePaise - fare) < MIN_RESERVE_PAISE) return false;
        balancePaise -= fare;
        chargedOut = fare;
        return true;
    }

    long long getBalance() const { return balancePaise; }
};

int main() {
    FareTable table;
    MonthlyPass pass(Zone::Inner, Zone::Airport, 30);
    SmartCard card("NM-PASS-404", 12000, pass); // Rs 120.00 + Inner/Airport pass

    long long charged = 0;
    // 1. Airport weekday peak covered by Monthly Pass -> Rs 0.00
    card.processJourney(Zone::Airport, 9, false, 15, PassengerType::Regular, table, charged);
    std::cout << "1. Airport (Pass Covered, Day 15)   -> Charged: Rs " << charged / 100.0
              << " | Balance: Rs " << card.getBalance() / 100.0 << "\n";

    // 2. Outer weekend (not in pass) -> Rs 30 - 10% = Rs 27.00
    card.processJourney(Zone::Outer, 9, true, 16, PassengerType::Regular, table, charged);
    std::cout << "2. Outer (Weekend 9AM, No Stack)    -> Charged: Rs " << charged / 100.0
              << " | Balance: Rs " << card.getBalance() / 100.0 << "\n";

    // 3. Airport weekday peak after pass expiry (Day 31) -> Rs 60 + 20% = Rs 72.00 -> Bal 93 - 72 = 21 (<50) -> Denied!
    bool ok = card.processJourney(Zone::Airport, 9, false, 31, PassengerType::Regular, table, charged);
    std::cout << "3. Airport (Expired Pass, Day 31)   -> " << (ok ? "APPROVED" : "DENIED (Reserve < Rs 50)")
              << " | Balance: Rs " << card.getBalance() / 100.0 << "\n";
    return 0;
}
