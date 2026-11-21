#include <iostream>
#include <string>

class BusTicket {
    static int issued;
    static const int CAPACITY = 3;
    int seat;
    std::string name;

public:
    explicit BusTicket(const std::string& n) : seat(0), name(n) {
        if (issued < CAPACITY) {
            seat = ++issued;
        }
    }

    static int count() { return issued; }

    void print() const {
        std::cout << "  " << name << " -> "
                  << (seat > 0 ? "Seat " + std::to_string(seat) : "Waiting List") << '\n';
    }
};

int BusTicket::issued = 0;

void issueBatch(const std::string& batchLabel) {
    std::cout << "--- " << batchLabel << " ---\n";
    BusTicket batch[2] = {BusTicket(batchLabel + "-P1"), BusTicket(batchLabel + "-P2")};
    for (const auto& t : batch) t.print();
    std::cout << "Total BusTicket::count() after " << batchLabel << ": " << BusTicket::count() << "\n\n";
}

int main() {
    issueBatch("Morning-Call-1");
    issueBatch("Afternoon-Call-2");
    return 0;
}
