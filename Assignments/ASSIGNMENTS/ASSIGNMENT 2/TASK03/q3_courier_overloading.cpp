#include <iostream>
#include <cmath>

struct Parcel {
    double weightKg;
    bool express;
    bool insured;
};

long long charge(double weightKg, bool express = false, bool insured = false) {
    long long basePaise = static_cast<long long>(std::llround(weightKg * 5000.0));
    if (express) basePaise += 4000;
    if (insured) basePaise += 2500;
    std::cout << "[Selected: charge(double, bool, bool)] -> ";
    return basePaise;
}

long long charge(int weightGrams, bool express = false, bool insured = false) {
    long long basePaise = (static_cast<long long>(weightGrams) * 5000) / 1000;
    if (express) basePaise += 4000;
    if (insured) basePaise += 2500;
    std::cout << "[Selected: charge(int, bool, bool)]    -> ";
    return basePaise;
}

long long charge(const Parcel& p) {
    std::cout << "[Selected: charge(const Parcel&)]      -> ";
    long long basePaise = static_cast<long long>(std::llround(p.weightKg * 5000.0));
    if (p.express) basePaise += 4000;
    if (p.insured) basePaise += 2500;
    return basePaise;
}

int main() {
    Parcel p{3.5, true, true};

    std::cout << "Call 1 charge(2)              : " << charge(2) << "p\n";
    std::cout << "Call 2 charge(2.0f)           : " << charge(2.0f) << "p\n";
    std::cout << "Call 3 charge(2.5)            : " << charge(2.5) << "p\n";
    std::cout << "Call 4 charge(1500, true)     : " << charge(1500, true) << "p\n";
    std::cout << "Call 5 charge(2.5, true, true): " << charge(2.5, true, true) << "p\n";
    std::cout << "Call 6 charge(p)              : " << charge(p) << "p\n";
    return 0;
}
