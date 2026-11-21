#include <iostream>
#include <iomanip>

// ==================== STYLE 1: PROCEDURAL ====================
namespace ProceduralMetro {
    constexpr long long MIN_BAL = 5000;

    bool canTravel(long long balancePaise, long long farePaise) {
        return farePaise >= 0 && (balancePaise - farePaise) >= MIN_BAL;
    }

    bool deduct(long long& balancePaise, long long farePaise) {
        if (!canTravel(balancePaise, farePaise)) return false;
        balancePaise -= farePaise;
        return true;
    }
}

// ==================== STYLE 2: OBJECT-ORIENTED ====================
class SmartCard {
private:
    long long balancePaise;
    static constexpr long long MIN_BALANCE_PAISE = 5000;

public:
    explicit SmartCard(long long initialPaise)
        : balancePaise(initialPaise >= 0 ? initialPaise : 0) {}

    bool canTravel(long long farePaise) const {
        return farePaise >= 0 && (balancePaise - farePaise) >= MIN_BALANCE_PAISE;
    }

    bool deduct(long long farePaise) {
        if (!canTravel(farePaise)) return false;
        balancePaise -= farePaise;
        return true;
    }

    bool recharge(long long amountPaise) {
        if (amountPaise <= 0) return false;
        balancePaise += amountPaise;
        return true;
    }

    long long getBalance() const { return balancePaise; }
};

int main() {
    // Procedural test
    long long procBalance = 8000; // Rs 80.00
    std::cout << "[Procedural] Initial: " << procBalance << "p\n";
    std::cout << "[Procedural] Deduct 2000p: " << (ProceduralMetro::deduct(procBalance, 2000) ? "OK" : "FAIL")
              << " -> Bal: " << procBalance << "p\n";
    std::cout << "[Procedural] Deduct 2000p: " << (ProceduralMetro::deduct(procBalance, 2000) ? "OK" : "FAIL")
              << " -> Bal: " << procBalance << "p\n";
    // Procedural vulnerability: external code can corrupt procBalance directly!
    procBalance = -500;
    std::cout << "[Procedural] Unprotected direct write -> Bal: " << procBalance << "p (CORRUPTED!)\n\n";

    // Object-Oriented test
    SmartCard card(8000);
    std::cout << "[OOP] Initial: " << card.getBalance() << "p\n";
    std::cout << "[OOP] Deduct 2000p: " << (card.deduct(2000) ? "OK" : "FAIL")
              << " -> Bal: " << card.getBalance() << "p\n";
    std::cout << "[OOP] Deduct 2000p: " << (card.deduct(2000) ? "OK" : "FAIL")
              << " -> Bal: " << card.getBalance() << "p\n";
    std::cout << "[OOP] Negative balance impossible; invariant preserved at " << card.getBalance() << "p\n";
    return 0;
}
