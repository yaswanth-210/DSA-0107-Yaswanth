#include <iostream>

int main() {
    std::cout << "--- Part A: Original Implicit Conversion Snippet ---\n";
    int a = 7, b = 2;
    double c = a / b;
    double d = static_cast<double>(a) / b;
    std::cout << c << ' ' << d << '\n';

    char ch = 'A' + 2;
    std::cout << ch << ' ' << ch + 1 << '\n';

    bool flag = 5;
    std::cout << flag << ' ' << std::boolalpha << flag << '\n';

    unsigned int u = 1;
    int s = -1;
    std::cout << (s < u ? "s < u" : "s >= u") << '\n';

    std::cout << "\n--- Part B: Safe Explicit Casts & Brace Initialization ---\n";
    int safeA{7};
    int safeB{2};
    double safeDiv{static_cast<double>(safeA) / static_cast<double>(safeB)};
    char nextCh{static_cast<char>('A' + 2)};
    int promotedCh{static_cast<int>(nextCh) + 1};
    bool explicitFlag{5 != 0};
    bool safeSignedCompare{s < static_cast<int>(u)};

    std::cout << "safeDiv: " << safeDiv << ", nextCh: " << nextCh
              << ", promotedCh: " << promotedCh << ", explicitFlag: " << explicitFlag
              << ", s < static_cast<int>(u): " << safeSignedCompare << '\n';
    return 0;
}
