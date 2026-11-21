#include <iostream>
#include <string>

enum class Zone { Inner = 0, Middle = 1, Outer = 2 };
enum class PassengerType { Regular, Student, Senior };

class FareTable {
private:
    static constexpr long long BASE_PAISE[3] = {1000, 2000, 3000};
    static constexpr int PEAK_SURCHARGE_PCT = 20;
    static constexpr int CONCESSION_PCT = 25;

public:
    static bool isPeakHour(int hour) {
        return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    }

    long long computeFarePaise(Zone z, int hour, PassengerType pType) const {
        long long fare = BASE_PAISE[static_cast<int>(z)];
        if (isPeakHour(hour)) {
            fare += (fare * PEAK_SURCHARGE_PCT) / 100;
        }
        if (pType == PassengerType::Student || pType == PassengerType::Senior) {
            fare -= (fare * CONCESSION_PCT) / 100;
        }
        return fare;
    }
};

class SmartCard {
private:
    std::string cardId;
    long long balancePaise;
    static constexpr long long MIN_RESERVE_PAISE = 5000; // Rs 50.00

public:
    SmartCard(std::string id, long long initialPaise)
        : cardId(id), balancePaise(initialPaise < 0 ? 0 : initialPaise) {}

    bool canTravel(long long farePaise) const {
        return farePaise >= 0 && (balancePaise - farePaise) >= MIN_RESERVE_PAISE;
    }

    bool deductFare(long long farePaise) {
        if (!canTravel(farePaise)) return false;
        balancePaise -= farePaise;
        return true;
    }

    long long getBalancePaise() const { return balancePaise; }
    std::string getCardId() const { return cardId; }
};

class Journey {
private:
    Zone farthestZone;
    int entryHour;
    PassengerType passengerType;

public:
    Journey(Zone z, int hr, PassengerType pt)
        : farthestZone(z), entryHour(hr), passengerType(pt) {}

    bool process(SmartCard& card, const FareTable& table) const {
        if (entryHour < 0 || entryHour > 23) return false;
        long long fare = table.computeFarePaise(farthestZone, entryHour, passengerType);
        return card.deductFare(fare);
    }
};

int main() {
    FareTable table;
    SmartCard card("NM-192511172", 12000); // Rs 120.00
    Journey j1(Zone::Outer, 9, PassengerType::Regular);   // Peak: Rs 36.00 -> Bal Rs 84.00
    Journey j2(Zone::Middle, 10, PassengerType::Student); // Off-peak + 25% off: Rs 15.00 -> Bal Rs 69.00
    Journey j3(Zone::Outer, 18, PassengerType::Regular);  // Peak: Rs 36.00 -> Would leave Rs 33 (< 50) -> Denied

    std::cout << "Card ID: " << card.getCardId() << " | Initial: Rs " << card.getBalancePaise() / 100.0 << "\n";
    std::cout << "Journey 1 (Outer, 09:00, Regular): " << (j1.process(card, table) ? "APPROVED" : "DENIED")
              << " | Balance: Rs " << card.getBalancePaise() / 100.0 << "\n";
    std::cout << "Journey 2 (Middle, 10:00, Student): " << (j2.process(card, table) ? "APPROVED" : "DENIED")
              << " | Balance: Rs " << card.getBalancePaise() / 100.0 << "\n";
    std::cout << "Journey 3 (Outer, 18:00, Regular): " << (j3.process(card, table) ? "APPROVED" : "DENIED")
              << " | Balance: Rs " << card.getBalancePaise() / 100.0 << "\n";
    return 0;
}
