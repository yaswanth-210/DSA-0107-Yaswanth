#include <iostream>

bool isPeakLogical(int hour) {
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

long long computeSurcharge(long long basePaise, bool peak) {
    return peak ? (basePaise * 20) / 100 : 0;
}

long long applyConcession(long long farePaise, bool eligible) {
    if (eligible) {
        farePaise -= (farePaise * 25) / 100;
    }
    return farePaise;
}

int main() {
    std::cout << "--- Part A: Predict the Output Program ---\n";
    int x = 5, y = 3;
    std::cout << x + y * 2 << ' ' << (x + y) * 2 << '\n';

    int z = x++ + ++y;
    std::cout << z << ' ' << x << ' ' << y << '\n';

    std::cout << (x > y ? x : y) << ' ' << x % 4 * 2 << '\n';
    std::cout << 10 / 3 << ' ' << 10 % 3 << ' ' << -10 / 3 << ' ' << -10 % 3 << '\n';

    int k = 0;
    for (int i = 0; i < 10; i += 3) k += i;
    std::cout << k << '\n';

    std::cout << "\n--- Part B: Metro Precedence Expressions ---\n";
    long long base = 3000; // Rs 30.00
    bool peak = isPeakLogical(18);
    long long gross = base + computeSurcharge(base, peak);
    long long net = applyConcession(gross, true);
    std::cout << "Hour 18 Peak? " << std::boolalpha << peak
              << " | Gross: " << gross << "p | Student Net: " << net << "p\n";
    return 0;
}
