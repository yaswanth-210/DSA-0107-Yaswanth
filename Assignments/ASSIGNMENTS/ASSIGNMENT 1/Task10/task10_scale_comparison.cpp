#include <iostream>
#include <iomanip>
#include <climits>

void runScaleBenchmark(long long journeys) {
    // Mixed pattern: 50% Inner off-peak (1000p), 30% Middle peak (2400p), 20% Outer peak student (2700p)
    // Exact average per 10 journeys = 5*1000 + 3*2400 + 2*2700 = 5000 + 7200 + 5400 = 17600 paise
    const long long patternPaise[10] = {1000, 1000, 1000, 1000, 1000, 2400, 2400, 2400, 2700, 2700};
    const double patternRupees[10] = {10.00, 10.00, 10.00, 10.00, 10.00, 24.00, 24.00, 24.00, 27.00, 27.00};

    long long exactPaise = 0;
    int intPaise = 0;
    double doubleRupees = 0.0;
    bool intOverflowed = false;

    for (long long i = 0; i < journeys; ++i) {
        long long p = patternPaise[i % 10];
        if (!intOverflowed && intPaise > INT_MAX - static_cast<int>(p)) {
            intOverflowed = true;
        }
        intPaise += static_cast<int>(p);
        exactPaise += p;
        doubleRupees += patternRupees[i % 10];
    }

    double exactRupees = static_cast<double>(exactPaise) / 100.0;
    std::cout << "Journeys: " << std::setw(8) << journeys
              << " | Exact: Rs " << std::fixed << std::setprecision(4) << std::setw(12) << exactRupees
              << " | Double: Rs " << std::setw(12) << doubleRupees
              << " | Diff: " << std::setprecision(6) << (doubleRupees - exactRupees)
              << " | int32: " << (intOverflowed ? "OVERFLOW" : std::to_string(intPaise) + "p") << "\n";
}

int main() {
    std::cout << "=== Network Scale Precision & Overflow Benchmark ===\n";
    runScaleBenchmark(20000);
    runScaleBenchmark(500000);
    runScaleBenchmark(3000000);
    return 0;
}
