#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Staff {
private:
    std::string name;
    int id;

protected:
    static constexpr double DA_RATE = 0.42;

public:
    Staff(std::string staffName, int staffId)
        : name(std::move(staffName)), id(staffId) {}

    virtual ~Staff() = default;

    const std::string& getName() const { return name; }
    int getId() const { return id; }

    virtual std::string role() const = 0;
    virtual double pay() const = 0;
};

class SalariedStaff : public Staff {
private:
    double basic;

protected:
    double baseWithDA() const { return basic + basic * DA_RATE; }

public:
    SalariedStaff(std::string staffName, int staffId, double basicPay)
        : Staff(std::move(staffName), staffId), basic(basicPay) {}
};

class Professor : public SalariedStaff {
private:
    double researchAllowance;

public:
    Professor(std::string staffName, int staffId, double basicPay, double research)
        : SalariedStaff(std::move(staffName), staffId, basicPay),
          researchAllowance(research) {}

    std::string role() const override { return "Professor"; }
    double pay() const override { return baseWithDA() + researchAllowance; }
};

class LabAssistant : public SalariedStaff {
public:
    LabAssistant(std::string staffName, int staffId, double basicPay)
        : SalariedStaff(std::move(staffName), staffId, basicPay) {}

    std::string role() const override { return "LabAssistant"; }
    double pay() const override { return baseWithDA(); }
};

class GuestFaculty : public Staff {
private:
    double hours;
    double hourlyRate;

public:
    GuestFaculty(std::string staffName, int staffId, double workedHours, double rate)
        : Staff(std::move(staffName), staffId), hours(workedHours), hourlyRate(rate) {}

    std::string role() const override { return "GuestFaculty"; }
    double pay() const override { return hours * hourlyRate; }
};

int main() {
    std::vector<std::unique_ptr<Staff>> payroll;
    payroll.push_back(std::make_unique<Professor>("Dr. Meenakshi", 101, 80000.0, 15000.0));
    payroll.push_back(std::make_unique<LabAssistant>("Karthik R", 204, 35000.0));
    payroll.push_back(std::make_unique<GuestFaculty>("Ananya S", 309, 24.0, 1200.0));

    std::cout << "=== Task 1: Repaired College Payroll Hierarchy ===\n";
    std::cout << std::fixed << std::setprecision(2);
    double totalOutflow = 0.0;
    for (const auto& s : payroll) {
        double p = s->pay();
        totalOutflow += p;
        std::cout << "[" << s->role() << "] ID=" << s->getId()
                  << " | Name=" << s->getName()
                  << " | Monthly Pay = Rs " << p << '\n';
    }
    std::cout << "Total Payroll Outflow = Rs " << totalOutflow << '\n';
    return 0;
}
