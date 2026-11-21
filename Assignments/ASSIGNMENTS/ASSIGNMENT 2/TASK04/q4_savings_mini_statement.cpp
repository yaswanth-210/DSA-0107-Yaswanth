#include <iostream>
#include <iomanip>
#include <string>

struct Transaction {
    char type;
    long long amountPaise;
    long long balanceAfterPaise;
};

class SavingsAccount {
private:
    static constexpr int MAX_HISTORY = 10;
    std::string accountNumber;
    long long balancePaise;
    Transaction history[MAX_HISTORY];
    int count;

    void recordTransaction(char type, long long amt) {
        if (count < MAX_HISTORY) {
            history[count++] = {type, amt, balancePaise};
        } else {
            for (int i = 1; i < MAX_HISTORY; ++i) {
                history[i - 1] = history[i];
            }
            history[MAX_HISTORY - 1] = {type, amt, balancePaise};
        }
    }

public:
    SavingsAccount(const std::string& accNo, long long initialPaise)
        : accountNumber(accNo), balancePaise(initialPaise), count(0) {}

    long long getBalancePaise() const { return balancePaise; }

    bool deposit(long long amountPaise);
    bool withdraw(long long amountPaise);
    void miniStatement() const;
};

bool SavingsAccount::deposit(long long amountPaise) {
    if (amountPaise <= 0) return false;
    balancePaise += amountPaise;
    recordTransaction('D', amountPaise);
    return true;
}

bool SavingsAccount::withdraw(long long amountPaise) {
    if (amountPaise <= 0 || amountPaise > balancePaise) return false;
    balancePaise -= amountPaise;
    recordTransaction('W', amountPaise);
    return true;
}

void SavingsAccount::miniStatement() const {
    std::cout << "=== Mini-Statement for A/C " << accountNumber << " (Last " << count << " Txns) ===\n";
    for (int i = 0; i < count; ++i) {
        std::cout << std::setw(2) << (i + 1) << ". [" << history[i].type << "] "
                  << "Rs " << (history[i].amountPaise / 100) << '.'
                  << std::setw(2) << std::setfill('0') << (history[i].amountPaise % 100)
                  << std::setfill(' ')
                  << " | Bal: Rs " << (history[i].balanceAfterPaise / 100) << '.'
                  << std::setw(2) << std::setfill('0') << (history[i].balanceAfterPaise % 100)
                  << std::setfill(' ') << '\n';
    }
}

int main() {
    SavingsAccount acc("SB-192511172", 100000); // Rs 1000.00
    for (int i = 1; i <= 12; ++i) {
        if (i % 3 == 0) acc.withdraw(i * 1000);
        else acc.deposit(i * 2000);
    }
    acc.miniStatement();
    return 0;
}
