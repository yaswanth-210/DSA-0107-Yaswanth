#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

using Clock = std::chrono::steady_clock;

// 1. Polymorphic (virtual) hierarchy
struct VBill {
    virtual ~VBill() = default;
    virtual double total() const = 0;
};

struct VOutPatient : VBill {
    double c, t;
    VOutPatient(double a, double b) : c(a), t(b) {}
    double total() const override { return c + t; }
};

struct VInPatient : VBill {
    int d;
    double r, s;
    VInPatient(int days, double rate, double surg) : d(days), r(rate), s(surg) {}
    double total() const override { return static_cast<double>(d) * r + s; }
};

// 2. Monolithic struct + enum + switch
enum class BillKind { OP, IP };

struct SwitchBill {
    BillKind kind;
    double c, t;
    int d;
    double r, s;
    double total() const {
        switch (kind) {
            case BillKind::OP: return c + t;
            case BillKind::IP: return static_cast<double>(d) * r + s;
        }
        return 0.0;
    }
};

// 3. std::variant + std::visit alternative
struct VarOP {
    double c, t;
    double total() const { return c + t; }
};

struct VarIP {
    int d;
    double r, s;
    double total() const { return static_cast<double>(d) * r + s; }
};

using BillVariant = std::variant<VarOP, VarIP>;

template <typename Fn>
double benchVirtual(std::size_t n, Fn&& elapsedSink) {
    std::vector<std::unique_ptr<VBill>> bills;
    bills.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        if (i % 2 == 0) bills.push_back(std::make_unique<VOutPatient>(500.0, 1200.0));
        else bills.push_back(std::make_unique<VInPatient>(3, 2500.0, 40000.0));
    }
    auto t0 = Clock::now();
    volatile double acc = 0.0;
    double sum = 0.0;
    for (const auto& b : bills) {
        sum += b->total();
    }
    acc = sum;
    auto t1 = Clock::now();
    elapsedSink(std::chrono::duration<double, std::milli>(t1 - t0).count());
    return acc;
}

template <typename Fn>
double benchSwitch(std::size_t n, Fn&& elapsedSink) {
    std::vector<SwitchBill> bills;
    bills.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        if (i % 2 == 0) bills.push_back(SwitchBill{BillKind::OP, 500.0, 1200.0, 0, 0.0, 0.0});
        else bills.push_back(SwitchBill{BillKind::IP, 0.0, 0.0, 3, 2500.0, 40000.0});
    }
    auto t0 = Clock::now();
    volatile double acc = 0.0;
    double sum = 0.0;
    for (const auto& b : bills) {
        sum += b.total();
    }
    acc = sum;
    auto t1 = Clock::now();
    elapsedSink(std::chrono::duration<double, std::milli>(t1 - t0).count());
    return acc;
}

template <typename Fn>
double benchVariant(std::size_t n, Fn&& elapsedSink) {
    std::vector<BillVariant> bills;
    bills.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        if (i % 2 == 0) bills.push_back(VarOP{500.0, 1200.0});
        else bills.push_back(VarIP{3, 2500.0, 40000.0});
    }
    auto t0 = Clock::now();
    volatile double acc = 0.0;
    double sum = 0.0;
    for (const auto& v : bills) {
        sum += std::visit([](const auto& item) { return item.total(); }, v);
    }
    acc = sum;
    auto t1 = Clock::now();
    elapsedSink(std::chrono::duration<double, std::milli>(t1 - t0).count());
    return acc;
}

bool checkReturnCode(double amt, double cover) {
    return (amt >= 0.0 && cover >= 0.0 && amt <= cover);
}

void checkException(double amt, double cover) {
    if (amt < 0.0 || cover < 0.0) throw std::invalid_argument("negative");
    if (amt > cover) throw std::domain_error("over cover");
}

double benchValidationReturnCode(std::size_t n, int invalidPct) {
    std::vector<double> amounts(n, 25000.0);
    std::size_t badCount = (n * static_cast<std::size_t>(invalidPct)) / 100;
    for (std::size_t i = 0; i < badCount; ++i) amounts[i] = 90000.0;

    auto t0 = Clock::now();
    volatile std::size_t rejected = 0;
    std::size_t rej = 0;
    for (double a : amounts) {
        if (!checkReturnCode(a, 50000.0)) ++rej;
    }
    rejected = rej;
    auto t1 = Clock::now();
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

double benchValidationExceptions(std::size_t n, int invalidPct) {
    std::vector<double> amounts(n, 25000.0);
    std::size_t badCount = (n * static_cast<std::size_t>(invalidPct)) / 100;
    for (std::size_t i = 0; i < badCount; ++i) amounts[i] = 90000.0;

    auto t0 = Clock::now();
    volatile std::size_t rejected = 0;
    std::size_t rej = 0;
    for (double a : amounts) {
        try {
            checkException(a, 50000.0);
        } catch (const std::exception&) {
            ++rej;
        }
    }
    rejected = rej;
    auto t1 = Clock::now();
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

int main() {
    std::cout << std::fixed << std::setprecision(3);

    // B3.1: Small dataset of 10 bills
    double msV10 = 0.0, msS10 = 0.0;
    double sumV10 = benchVirtual(10, [&](double ms) { msV10 = ms; });
    double sumS10 = benchSwitch(10, [&](double ms) { msS10 = ms; });
    std::cout << "=== B3.1: 10-Bill Correctness Check ===\n";
    std::cout << "Virtual sum = " << sumV10 << " | Switch sum = " << sumS10 << '\n';

    // B3.2 & Variant comparison across scales
    std::cout << "\n=== B3.2 & Variant: Dispatch Timing (ms) ===\n";
    for (std::size_t n : {1000ULL, 1000000ULL, 10000000ULL}) {
        double msV = 0.0, msS = 0.0, msVar = 0.0;
        benchVirtual(n, [&](double ms) { msV = ms; });
        benchSwitch(n, [&](double ms) { msS = ms; });
        if (n == 10000000ULL) {
            benchVariant(n, [&](double ms) { msVar = ms; });
            std::cout << "N=" << n << " -> virtual=" << msV << " ms, switch=" << msS
                      << " ms, std::visit=" << msVar << " ms\n";
        } else {
            std::cout << "N=" << n << " -> virtual=" << msV << " ms, switch=" << msS << " ms\n";
        }
    }

    std::cout << "\n--- Element Sizes ---\n";
    std::cout << "sizeof(SwitchBill)  = " << sizeof(SwitchBill) << " bytes\n";
    std::cout << "sizeof(BillVariant) = " << sizeof(BillVariant) << " bytes\n";

    // B3.3: Exception cost at 0%, 1%, 50% invalid on 1,000,000 claims
    std::cout << "\n=== B3.3: Validation Timing on N=1,000,000 Claims (ms) ===\n";
    for (int pct : {0, 1, 50}) {
        double msRet = benchValidationReturnCode(1000000ULL, pct);
        double msExc = benchValidationExceptions(1000000ULL, pct);
        std::cout << "Invalid=" << pct << "% -> return-code=" << msRet
                  << " ms, exceptions=" << msExc << " ms\n";
    }
    return 0;
}
