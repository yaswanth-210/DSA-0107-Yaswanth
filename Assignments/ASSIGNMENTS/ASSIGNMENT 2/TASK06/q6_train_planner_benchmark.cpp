#include <iostream>
#include <chrono>
#include <cstdlib>

class Time {
    int h, m;

public:
    Time(int hh, int mm) : h(hh), m(mm) {}

    friend inline int minutesBetweenInline(const Time& a, const Time& b) {
        return std::abs((a.h * 60 + a.m) - (b.h * 60 + b.m));
    }

    friend inline bool earlierInline(const Time& a, const Time& b) {
        return (a.h * 60 + a.m) < (b.h * 60 + b.m);
    }

    friend int minutesBetweenNoInline(const Time& a, const Time& b);
    friend bool earlierNoInline(const Time& a, const Time& b);
};

__attribute__((noinline))
int minutesBetweenNoInline(const Time& a, const Time& b) {
    return std::abs((a.h * 60 + a.m) - (b.h * 60 + b.m));
}

__attribute__((noinline))
bool earlierNoInline(const Time& a, const Time& b) {
    return (a.h * 60 + a.m) < (b.h * 60 + b.m);
}

int main() {
    Time morning(11, 30), afternoon(12, 15), identical(11, 30);

    std::cout << "Across noon (11:30 vs 12:15): diff=" << minutesBetweenInline(morning, afternoon)
              << "m, earlier=" << std::boolalpha << earlierInline(morning, afternoon) << '\n';
    std::cout << "Same time   (11:30 vs 11:30): diff=" << minutesBetweenInline(morning, identical)
              << "m, earlier=" << earlierInline(morning, identical) << "\n\n";

    constexpr long long N = 10'000'000LL;
    volatile long long sink = 0;

    auto t1 = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; ++i) {
        sink += minutesBetweenNoInline(morning, afternoon);
        sink += earlierNoInline(morning, afternoon) ? 1 : 0;
    }
    auto t2 = std::chrono::high_resolution_clock::now();

    auto t3 = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; ++i) {
        sink += minutesBetweenInline(morning, afternoon);
        sink += earlierInline(morning, afternoon) ? 1 : 0;
    }
    auto t4 = std::chrono::high_resolution_clock::now();

    double msNoInline = std::chrono::duration<double, std::milli>(t2 - t1).count();
    double msInline   = std::chrono::duration<double, std::milli>(t4 - t3).count();

    std::cout << "10,000,000 calls (noinline) : " << msNoInline << " ms\n";
    std::cout << "10,000,000 calls (inline)   : " << msInline << " ms\n";
    (void)sink;
    return 0;
}
