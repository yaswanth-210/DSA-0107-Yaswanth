#include <iostream>
#include <chrono>
#include <string>

struct Txn {
    char type;
    long long amountPaise;
};

class ShiftingMiniStatement {
    static constexpr int CAP = 50;
    Txn data[CAP];
    int count = 0;

public:
    void add(char type, long long amt) {
        if (count < CAP) {
            data[count++] = {type, amt};
        } else {
            for (int i = 1; i < CAP; ++i) data[i - 1] = data[i];
            data[CAP - 1] = {type, amt};
        }
    }
    long long latestAmount() const { return count > 0 ? data[count - 1].amountPaise : 0; }
};

class CircularMiniStatement {
    static constexpr int CAP = 50;
    Txn data[CAP];
    int head = 0;
    int count = 0;

public:
    void add(char type, long long amt) {
        int writeIdx = (head + count) % CAP;
        data[writeIdx] = {type, amt};
        if (count < CAP) {
            ++count;
        } else {
            head = (head + 1) % CAP;
        }
    }

    long long latestAmount() const {
        if (count == 0) return 0;
        int lastIdx = (head + count - 1) % CAP;
        return data[lastIdx].amountPaise;
    }

    void printRecent(int limit = 5) const {
        int show = count < limit ? count : limit;
        for (int i = count - show; i < count; ++i) {
            int idx = (head + i) % CAP;
            std::cout << "  [" << data[idx].type << "] Rs " << (data[idx].amountPaise / 100) << '\n';
        }
    }
};

int main() {
    constexpr long long OPS = 5'000'000LL;
    ShiftingMiniStatement orig;
    CircularMiniStatement redesigned;

    auto s1 = std::chrono::high_resolution_clock::now();
    for (long long i = 1; i <= OPS; ++i) orig.add('D', i * 100);
    auto e1 = std::chrono::high_resolution_clock::now();

    auto s2 = std::chrono::high_resolution_clock::now();
    for (long long i = 1; i <= OPS; ++i) redesigned.add('D', i * 100);
    auto e2 = std::chrono::high_resolution_clock::now();

    double msOrig = std::chrono::duration<double, std::milli>(e1 - s1).count();
    double msRing = std::chrono::duration<double, std::milli>(e2 - s2).count();

    std::cout << "5,000,000 Transactions (Capacity = 50):\n";
    std::cout << "  Original Shifting Buffer : " << msOrig << " ms (Latest: Rs " << orig.latestAmount() / 100 << ")\n";
    std::cout << "  Circular Ring Buffer     : " << msRing << " ms (Latest: Rs " << redesigned.latestAmount() / 100 << ")\n";
    std::cout << "\nLast 5 Transactions in Redesigned Ring Buffer:\n";
    redesigned.printRecent(5);
    return 0;
}
