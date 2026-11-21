#include <algorithm>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class BillingError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class ClaimError : public BillingError {
public:
    using BillingError::BillingError;
};

class Bill {
protected:
    std::string patientId;
    std::string patientName;

public:
    Bill(std::string pid, std::string pname)
        : patientId(std::move(pid)), patientName(std::move(pname)) {
        if (patientId.empty() || patientName.empty()) {
            throw BillingError("missing patient record identity");
        }
    }
    virtual ~Bill() noexcept = default;

    const std::string& id() const noexcept { return patientId; }
    const std::string& name() const noexcept { return patientName; }
    virtual std::string kind() const noexcept = 0;
    virtual double total() const = 0;
};

class OutPatientBill : public Bill {
    double consultationFee;
    double diagnosticFee;

public:
    OutPatientBill(std::string pid, std::string pname, double consult, double diag)
        : Bill(std::move(pid), std::move(pname)),
          consultationFee(consult), diagnosticFee(diag) {
        if (consultationFee < 0.0 || diagnosticFee < 0.0) {
            throw BillingError("negative OP consultation or diagnostic fee");
        }
    }
    std::string kind() const noexcept override { return "OPD"; }
    double total() const override { return consultationFee + diagnosticFee; }
};

class InPatientBill : public Bill {
    int days;
    double roomPerDay;
    double procedureAndMeds;

public:
    InPatientBill(std::string pid, std::string pname, int d, double rate, double procMed)
        : Bill(std::move(pid), std::move(pname)),
          days(d), roomPerDay(rate), procedureAndMeds(procMed) {
        if (days < 0 || roomPerDay < 0.0 || procedureAndMeds < 0.0) {
            throw BillingError("negative IP days, room rate, or procedure cost");
        }
    }
    std::string kind() const noexcept override { return "IPD"; }
    double total() const override { return static_cast<double>(days) * roomPerDay + procedureAndMeds; }
};

// New Bill Type added WITHOUT editing existing classes (B4.1)
class DayCareSurgeryBill : public Bill {
    double packagePrice;
    double implantCost;

public:
    DayCareSurgeryBill(std::string pid, std::string pname, double pkg, double implant)
        : Bill(std::move(pid), std::move(pname)),
          packagePrice(pkg), implantCost(implant) {
        if (packagePrice < 0.0 || implantCost < 0.0) {
            throw BillingError("negative Day-Care package or implant charge");
        }
    }
    std::string kind() const noexcept override { return "DAYCARE"; }
    double total() const override { return packagePrice + implantCost; }
};

class Payer {
public:
    virtual ~Payer() noexcept = default;
    virtual std::string payerName() const noexcept = 0;
    virtual double amountCovered(const Bill& bill) const = 0;
};

class SelfPay : public Payer {
public:
    std::string payerName() const noexcept override { return "SelfPay"; }
    double amountCovered(const Bill& bill) const override {
        return bill.total();
    }
};

class Insurance : public Payer {
    double maxCover;
    double copayFraction;

public:
    Insurance(double cover, double copayPct)
        : maxCover(cover), copayFraction(copayPct / 100.0) {
        if (maxCover < 0.0 || copayPct < 0.0 || copayPct > 100.0) {
            throw ClaimError("invalid insurance cover or co-pay percentage");
        }
    }
    std::string payerName() const noexcept override { return "Insurance"; }
    double amountCovered(const Bill& bill) const override {
        double gross = bill.total();
        if (gross > maxCover) {
            throw ClaimError("billed amount exceeds insurance cover limit");
        }
        return gross * (1.0 - copayFraction);
    }
};

// New Payer added WITHOUT editing existing classes (B4.1)
class CorporateTieUp : public Payer {
    std::string company;
    double negotiatedCap;

public:
    CorporateTieUp(std::string comp, double cap)
        : company(std::move(comp)), negotiatedCap(cap) {
        if (negotiatedCap < 0.0) {
            throw ClaimError("negative corporate credit cap");
        }
    }
    std::string payerName() const noexcept override { return "Corporate(" + company + ")"; }
    double amountCovered(const Bill& bill) const override {
        double gross = bill.total();
        if (gross > negotiatedCap) {
            throw ClaimError("bill exceeds corporate tie-up cap");
        }
        return gross;
    }
};

struct ClaimAudit {
    std::string billId;
    std::string payerLabel;
    double coveredAmount;
};

static ClaimAudit settleClaim(const Bill& bill, const Payer& payer, std::vector<ClaimAudit>& log) {
    try {
        double covered = payer.amountCovered(bill);
        ClaimAudit entry{bill.id(), payer.payerName(), covered};
        log.push_back(entry);
        return entry;
    } catch (const ClaimError& e) {
        std::cerr << "  [AUDIT LOG] Claim rejected for " << bill.id()
                  << " (" << payer.payerName() << "): " << e.what() << '\n';
        throw; // rethrow preserving dynamic type
    }
}

int main() {
    std::cout << std::fixed << std::setprecision(2);
    std::vector<std::unique_ptr<Bill>> bills;
    std::vector<ClaimAudit> settledLog;

    bills.push_back(std::make_unique<OutPatientBill>("P-101", "Ravi", 500.0, 1200.0));
    bills.push_back(std::make_unique<InPatientBill>("P-102", "Meena", 3, 2500.0, 40000.0));
    bills.push_back(std::make_unique<DayCareSurgeryBill>("P-103", "Karthik", 18000.0, 3500.0));

    SelfPay selfPayer;
    Insurance healthPolicy(60000.0, 10.0);
    CorporateTieUp corpPlan("SIMATS-HealthNet", 30000.0);

    std::cout << "=== Task B4: Hospital Polymorphic Billing + Extensions + Exception Safety ===\n";
    for (std::size_t i = 0; i < bills.size(); ++i) {
        const Payer* activePayer = (i == 0) ? static_cast<const Payer*>(&selfPayer)
                               : (i == 1) ? static_cast<const Payer*>(&healthPolicy)
                                          : static_cast<const Payer*>(&corpPlan);
        try {
            ClaimAudit res = settleClaim(*bills[i], *activePayer, settledLog);
            std::cout << "  Settled " << bills[i]->kind() << " [" << res.billId << "] "
                      << bills[i]->name() << " | Gross=" << bills[i]->total()
                      << " | " << res.payerLabel << " Covers=" << res.coveredAmount << '\n';
        } catch (const BillingError& e) {
            std::cout << "  Rejected [" << bills[i]->id() << "]: " << e.what() << '\n';
        }
    }

    // Trigger an intentional over-cover claim exception to prove clean RAII unwinding under ASan
    try {
        InPatientBill highBill("P-999", "HighClaimTest", 5, 10000.0, 50000.0);
        settleClaim(highBill, healthPolicy, settledLog);
    } catch (const ClaimError& e) {
        std::cout << "  Top-level handler caught rethrown ClaimError: " << e.what() << '\n';
    }

    std::cout << "Total settled claims recorded = " << settledLog.size() << '\n';
    return 0;
}
