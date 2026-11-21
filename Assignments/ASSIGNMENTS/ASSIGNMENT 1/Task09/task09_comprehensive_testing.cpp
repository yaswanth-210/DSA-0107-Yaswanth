#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <string>

enum class Zone { Inner = 0, Middle = 1, Outer = 2, Invalid = -1 };
enum class Concession { None, Student, Senior };

class SmartCard {
private:
    long long balancePaise;
    static constexpr long long MIN_RESERVE = 5000;
public:
    explicit SmartCard(long long init) : balancePaise(init >= 0 ? init : 0) {}
    bool recharge(long long amt) {
        if (amt <= 0) return false;
        balancePaise += amt;
        return true;
    }
    bool deduct(long long fare) {
        if (fare < 0 || (balancePaise - fare) < MIN_RESERVE) return false;
        balancePaise -= fare;
        return true;
    }
    long long balance() const { return balancePaise; }
};

Zone parseZone(int code) {
    if (code == 0) return Zone::Inner;
    if (code == 1) return Zone::Middle;
    if (code == 2) return Zone::Outer;
    return Zone::Invalid;
}

bool isPeak(int hour) {
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

long long computeFare(Zone z, int hour, Concession c) {
    if (z == Zone::Invalid || hour < 0 || hour > 23) return -1;
    constexpr long long BASE[] = {1000, 2000, 3000};
    long long f = BASE[static_cast<int>(z)];
    if (isPeak(hour)) f += (f * 20) / 100;
    if (c != Concession::None) f -= (f * 25) / 100;
    return f;
}

int main() {
    int passed = 0, total = 0;
    auto check = [&](const std::string& name, bool cond) {
        ++total;
        if (cond) ++passed;
        std::cout << "[" << (cond ? "PASS" : "FAIL") << "] " << name << "\n";
    };

    // Normal cases
    check("Inner Off-Peak (1000p)", computeFare(Zone::Inner, 11, Concession::None) == 1000);
    check("Inner Peak (1200p)", computeFare(Zone::Inner, 8, Concession::None) == 1200);
    check("Middle Off-Peak (2000p)", computeFare(Zone::Middle, 14, Concession::None) == 2000);
    check("Middle Peak (2400p)", computeFare(Zone::Middle, 17, Concession::None) == 2400);
    check("Outer Off-Peak (3000p)", computeFare(Zone::Outer, 21, Concession::None) == 3000);
    check("Outer Peak (3600p)", computeFare(Zone::Outer, 19, Concession::None) == 3600);

    // Boundary cases
    check("Boundary 08:00 is Peak (1200p)", computeFare(Zone::Inner, 8, Concession::None) == 1200);
    check("Boundary 10:00 is Off-Peak (1000p)", computeFare(Zone::Inner, 10, Concession::None) == 1000);
    check("Boundary 17:00 is Peak (1200p)", computeFare(Zone::Inner, 17, Concession::None) == 1200);
    check("Boundary 20:00 is Off-Peak (1000p)", computeFare(Zone::Inner, 20, Concession::None) == 1000);

    SmartCard exactCard(6000); // Rs 60.00 - Rs 10.00 = Rs 50.00 exact
    check("Balance leaves exact Rs 50.00", exactCard.deduct(1000) && exactCard.balance() == 5000);
    check("Balance Rs 50.00 denies Rs 10.00 trip", !exactCard.deduct(1000) && exactCard.balance() == 5000);

    // Invalid cases
    check("Unknown Zone (99) rejected", computeFare(parseZone(99), 12, Concession::None) == -1);
    check("Negative Recharge (-500p) rejected", !exactCard.recharge(-500));

    std::istringstream badIn("abc\n");
    int h = 0;
    badIn >> h;
    bool caughtLetters = badIn.fail();
    badIn.clear();
    check("Letters for hour triggers cin.fail()", caughtLetters);

    // 5-Journey Hand-Calculated Sequence (Start Rs 200.00 = 20000p)
    // J1: Inner, 08:00, Regular -> 1200p
    // J2: Outer, 10:00, Regular -> 3000p
    // J3: Middle, 17:00, Student -> 2400 - 600 = 1800p
    // J4: Outer, 20:00, Senior  -> 3000 - 750 = 2250p
    // J5: Inner, 15:00, Regular -> 1000p
    // Total Fare = 1200 + 3000 + 1800 + 2250 + 1000 = 9250p (Rs 92.50), Final Bal = 10750p (Rs 107.50)
    SmartCard seqCard(20000);
    long long fares[5] = {
        computeFare(Zone::Inner, 8, Concession::None),
        computeFare(Zone::Outer, 10, Concession::None),
        computeFare(Zone::Middle, 17, Concession::Student),
        computeFare(Zone::Outer, 20, Concession::Senior),
        computeFare(Zone::Inner, 15, Concession::None)
    };
    long long totalCollected = 0;
    for (long long f : fares) {
        if (seqCard.deduct(f)) totalCollected += f;
    }
    check("5-Journey Total == 9250p (Rs 92.50)", totalCollected == 9250 && seqCard.balance() == 10750);

    std::cout << "\nSummary: " << passed << "/" << total << " tests passed.\n";
    return 0;
}
