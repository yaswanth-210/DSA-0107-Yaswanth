#include <chrono>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class Time {
private:
    int totalSeconds;

public:
    explicit Time(int h = 0, int m = 0, int s = 0)
        : totalSeconds(h * 3600 + m * 60 + s) {}

    Time& operator++() { ++totalSeconds; return *this; }
    Time operator++(int) { Time old = *this; ++totalSeconds; return old; }

    int seconds() const { return totalSeconds; }
    bool operator==(const Time& rhs) const { return totalSeconds == rhs.totalSeconds; }
    bool operator<(const Time& rhs) const { return totalSeconds < rhs.totalSeconds; }

    friend std::ostream& operator<<(std::ostream& os, const Time& t) {
        int h = t.totalSeconds / 3600;
        int m = (t.totalSeconds % 3600) / 60;
        int s = t.totalSeconds % 60;
        return os << h << ':' << std::setw(2) << std::setfill('0') << m
                  << ':' << std::setw(2) << std::setfill('0') << s;
    }
};

class Playlist {
private:
    std::vector<std::string> tracks;

public:
    void addTrack(const std::string& title) { tracks.push_back(title); }
    int size() const { return static_cast<int>(tracks.size()); }

    std::string& operator[](int index) {
        if (index < 0 || index >= static_cast<int>(tracks.size())) {
            throw std::out_of_range("Playlist index out of bounds: " + std::to_string(index));
        }
        return tracks[index];
    }

    const std::string& operator[](int index) const {
        if (index < 0 || index >= static_cast<int>(tracks.size())) {
            throw std::out_of_range("Const Playlist index out of bounds: " + std::to_string(index));
        }
        return tracks[index];
    }

    const std::string& uncheckedAt(int index) const { return tracks[index]; }
};

struct Counter {
    int v;
    Counter& operator++() { ++v; return *this; }
    Counter operator++(int) { Counter old = *this; ++v; return old; }
};

int main() {
    std::cout << "=== Q5 Part A: Predict the Output Verification ===\n";
    Counter c{5};
    Counter a = c++;
    Counter b = ++c;
    std::cout << a.v << ' ' << b.v << ' ' << c.v << '\n';

    std::cout << "\n=== Q5 Part B: Time Roll-over & Increment Verification ===\n";
    Time t(0, 59, 59);
    Time tPost = t++;
    Time tPre = ++t;
    std::cout << "tPost (t++ from 0:59:59) = " << tPost << '\n';
    std::cout << "tPre  (++t after rollover)= " << tPre << '\n';
    std::cout << "tPost < tPre ? " << std::boolalpha << (tPost < tPre)
              << " | tPre == t ? " << (tPre == t) << '\n';

    std::cout << "\n=== Q5 Part C: Playlist Subscript & Bounds Checking ===\n";
    Playlist pl;
    pl.addTrack("Track 1 - Moonlight Sonata");
    pl.addTrack("Track 2 - Clair de Lune");
    pl[1] = "Track 2 - Nocturne Op.9 No.2";

    const Playlist& constPl = pl;
    std::cout << "constPl[0]: " << constPl[0] << '\n';
    std::cout << "pl[1]     : " << pl[1] << '\n';

    for (int badIdx : {-1, pl.size()}) {
        try {
            std::cout << pl[badIdx] << '\n';
        } catch (const std::out_of_range& ex) {
            std::cout << "Caught range error for index " << badIdx << ": " << ex.what() << '\n';
        }
    }

    constexpr int N = 10000000;
    volatile int sink = 0;
    Time bench1(0, 0, 0), bench2(0, 0, 0);

    auto s1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) sink = (++bench1).seconds();
    auto e1 = std::chrono::high_resolution_clock::now();

    auto s2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) sink = (bench2++).seconds();
    auto e2 = std::chrono::high_resolution_clock::now();

    auto s3 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) sink = static_cast<int>(constPl[i & 1].size());
    auto e3 = std::chrono::high_resolution_clock::now();

    auto s4 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) sink = static_cast<int>(constPl.uncheckedAt(i & 1).size());
    auto e4 = std::chrono::high_resolution_clock::now();

    std::cout << "\n--- 10,000,000 Operations Benchmark ---\n";
    std::cout << "Prefix  ++t            : " << std::chrono::duration<double, std::milli>(e1 - s1).count() << " ms\n";
    std::cout << "Postfix t++            : " << std::chrono::duration<double, std::milli>(e2 - s2).count() << " ms\n";
    std::cout << "Checked Playlist[i]    : " << std::chrono::duration<double, std::milli>(e3 - s3).count() << " ms\n";
    std::cout << "Unchecked Playlist[i]  : " << std::chrono::duration<double, std::milli>(e4 - s4).count() << " ms\n";

    return 0;
}
