#include <iostream>
#include <limits>
#include <new>
#include <stdexcept>
#include <string>
#include <vector>

// Part A: Predict-the-Output validation snippet
namespace PredictValidation {
void validate(double amount) {
    if (amount < 0) throw std::invalid_argument("negative");
    if (amount > 1e6) throw std::out_of_range("too large");
    std::cout << "ok ";
}

void process(double amount) {
    try {
        validate(amount);
    } catch (const std::out_of_range& e) {
        std::cout << "[log " << e.what() << "] ";
        throw;
    }
}

void run() {
    for (double a : {100.0, -5.0, 2e6}) {
        try {
            process(a);
            std::cout << "done ";
        } catch (const std::exception& e) {
            std::cout << "caught " << e.what() << ' ';
        }
    }
    std::cout << '\n';
}
} // namespace PredictValidation

// Part B: Complete-the-Code ClaimError approval & settlement
class ClaimError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

double approve(double billed, double cover) {
    // TODO 1: Throw std::invalid_argument("negative amount") if billed or cover is negative;
    // throw ClaimError("exceeds cover") if billed > cover; otherwise return billed
    if (billed < 0.0 || cover < 0.0) {
        throw std::invalid_argument("negative amount");
    }
    if (billed > cover) {
        throw ClaimError("exceeds cover");
    }
    return billed;
}

double settle(double billed, double cover) {
    try {
        return approve(billed, cover);
    } catch (const ClaimError& e) {
        // TODO 2: Print "audit: " followed by the message, then rethrow the SAME exception
        std::cout << "audit: " << e.what() << '\n';
        throw;
    }
}

struct ResourceTracker {
    std::string label;
    explicit ResourceTracker(std::string lbl) : label(std::move(lbl)) {
        std::cout << "  [Alloc] " << label << '\n';
    }
    ~ResourceTracker() {
        std::cout << "  [Free ] " << label << '\n';
    }
};

int main() {
    std::cout << "=== Task B2 Part A: Predict-the-Output Snippet ===\n";
    PredictValidation::run();

    std::cout << "\n=== Task B2 Part B: Complete-the-Code Claim Settlement ===\n";
    double cases[][2] = {{40000, 50000}, {60000, 50000}, {-1, 50000}};
    for (auto& c : cases) {
        try {
            double paid = settle(c[0], c[1]);
            std::cout << "approved " << paid << '\n';
        } catch (const ClaimError& e) {
            std::cout << "claim rejected: " << e.what() << '\n';
        } catch (const std::exception& e) {
            std::cout << "error: " << e.what() << '\n';
        }
    }

    std::cout << "\n=== Task B2 Part C: Error Stress Suite & RAII Cleanup ===\n";
    const std::vector<std::string> rawInputs = {
        "-250.0", "abc_text", "PKG_UNKNOWN", "120000.0"
    };
    for (const auto& token : rawInputs) {
        ResourceTracker guard("BillCtx(" + token + ")");
        try {
            if (token == "PKG_UNKNOWN") {
                throw std::domain_error("unknown package code: " + token);
            }
            double val = std::stod(token); // throws std::invalid_argument on non-numeric text
            if (val < 0.0) {
                throw std::invalid_argument("negative charge: " + token);
            }
            if (val > 80000.0) {
                throw ClaimError("claim exceeds policy cover");
            }
            std::cout << "  Processed valid amount: " << val << '\n';
        } catch (const std::exception& e) {
            std::cout << "  Handled std::exception -> " << e.what() << '\n';
        } catch (...) {
            std::cout << "  Top-level catch(...) recovered unknown fault\n";
        }
    }

    // Force std::bad_alloc safely using an impossible element count
    try {
        std::size_t hugeCount = std::numeric_limits<std::size_t>::max() / 2;
        volatile double* hugeBuf = new double[hugeCount];
        delete[] hugeBuf;
    } catch (const std::bad_alloc& e) {
        std::cout << "  Caught std::bad_alloc on oversized allocation (" << e.what() << ")\n";
    }

    return 0;
}
