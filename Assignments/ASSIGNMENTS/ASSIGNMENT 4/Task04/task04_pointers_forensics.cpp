#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

// Part A: Predict-the-Output verification snippet
namespace PredictDemo {
struct Account {
    double rate() const { return 3.5; }
    virtual double vrate() const { return 3.5; }
    virtual ~Account() = default;
};

struct SeniorAccount : Account {
    double rate() const { return 4.0; }
    double vrate() const override { return 4.0; }
};
} // namespace PredictDemo

// Part B: Repaired Bank Account Hierarchy (Virtual Functions + Virtual Destructor)
class Account {
protected:
    std::string holder;

public:
    explicit Account(std::string h = "Standard Customer") : holder(std::move(h)) {}

    virtual double rate() const { return 3.5; }
    virtual std::string type() const { return "Regular Account"; }

    virtual ~Account() {
        std::cout << "  [~Account] Cleaned up base resources for " << holder << '\n';
    }
};

class SeniorAccount : public Account {
private:
    double* history;

public:
    explicit SeniorAccount(std::string h = "Senior Citizen")
        : Account(std::move(h)), history(new double[100]{}) {}

    // Rule of Three: disable accidental shallow copies
    SeniorAccount(const SeniorAccount&) = delete;
    SeniorAccount& operator=(const SeniorAccount&) = delete;

    double rate() const override { return 4.0; }
    std::string type() const override { return "Senior Account"; }

    ~SeniorAccount() override {
        delete[] history;
        std::cout << "  [~SeniorAccount] Released double[100] history buffer\n";
    }
};

int main() {
    std::cout << "=== Task 4 Part A: Predict-the-Output Verification ===\n";
    PredictDemo::SeniorAccount s;
    PredictDemo::Account* p = &s;
    PredictDemo::Account copy = s;
    std::cout << p->rate() << ' ' << p->vrate() << ' ' << copy.vrate() << '\n';

    std::cout << "\n=== Task 4 Part B: Repaired Bank Polymorphic Hierarchy ===\n";
    std::cout << std::fixed << std::setprecision(1);

    // Using std::unique_ptr<Account> eliminates slicing and manual delete errors
    std::vector<std::unique_ptr<Account>> accounts;
    accounts.push_back(std::make_unique<Account>("Arun Kumar"));
    accounts.push_back(std::make_unique<SeniorAccount>("Lakshmi Devi"));

    for (const auto& acc : accounts) {
        std::cout << acc->type() << " -> Interest Rate = " << acc->rate() << "%\n";
    }

    std::cout << "\n--- Destruction Trace (Virtual Destructor Invokes Derived First) ---\n";
    accounts.clear();
    return 0;
}
