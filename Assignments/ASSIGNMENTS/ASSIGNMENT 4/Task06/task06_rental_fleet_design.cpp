#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Member class representing a calendar/service due date
class Date {
    int day;
    int month;
    int year;

public:
    Date(int d = 1, int m = 1, int y = 2026) : day(d), month(m), year(y) {}
    std::string str() const {
        return (day < 10 ? "0" : "") + std::to_string(day) + "/" +
               (month < 10 ? "0" : "") + std::to_string(month) + "/" +
               std::to_string(year);
    }
};

// Abstract Base Class
class Vehicle {
private:
    std::string regNo;
    double dailyRate;
    bool rented;

protected:
    static constexpr double LATE_PENALTY_MULTIPLIER = 1.5;

public:
    Vehicle(std::string reg, double rate)
        : regNo(std::move(reg)), dailyRate(rate), rented(false) {}

    virtual ~Vehicle() = default;

    const std::string& getRegNo() const { return regNo; }
    double getDailyRate() const { return dailyRate; }
    bool isRented() const { return rented; }

    bool rentOut() {
        if (rented) return false;
        rented = true;
        return true;
    }

    double returnVehicle(int daysRented, int lateDays, double usageUnits) {
        if (!rented) return 0.0;
        rented = false;
        double baseCharge = calculateCharge(daysRented, usageUnits);
        double lateFee = (lateDays > 0) ? (lateDays * dailyRate * LATE_PENALTY_MULTIPLIER) : 0.0;
        return baseCharge + lateFee;
    }

    virtual std::string category() const = 0;
    virtual double calculateCharge(int days, double usageUnits) const = 0;

    virtual void printDetails(std::ostream& os) const {
        os << "[" << category() << "] Reg=" << regNo
           << " | Rate=Rs " << std::fixed << std::setprecision(2) << dailyRate << "/day"
           << " | Status=" << (rented ? "RENTED" : "AVAILABLE");
    }
};

// Non-motorized derived class
class Bicycle : public Vehicle {
    bool hasGear;

public:
    Bicycle(std::string reg, double rate, bool geared)
        : Vehicle(std::move(reg), rate), hasGear(geared) {}

    std::string category() const override { return "Bicycle"; }

    double calculateCharge(int days, double /*usageUnits*/) const override {
        return days * getDailyRate() + (hasGear ? 50.0 : 0.0);
    }

    void printDetails(std::ostream& os) const override {
        Vehicle::printDetails(os);
        os << " | Gear=" << (hasGear ? "Yes" : "No");
    }
};

// Intermediate derived class for motorized vehicles
class MotorVehicle : public Vehicle {
private:
    double energyLevelPct;
    Date serviceDue;

public:
    MotorVehicle(std::string reg, double rate, double levelPct, Date due)
        : Vehicle(std::move(reg), rate), energyLevelPct(levelPct), serviceDue(due) {}

    void printDetails(std::ostream& os) const override {
        Vehicle::printDetails(os);
        os << " | Energy=" << energyLevelPct << "% | ServiceDue=" << serviceDue.str();
    }
};

class TwoWheeler : public MotorVehicle {
    double perLitreRate;

public:
    TwoWheeler(std::string reg, double rate, double fuelPct, Date due, double fuelRate)
        : MotorVehicle(std::move(reg), rate, fuelPct, due), perLitreRate(fuelRate) {}

    std::string category() const override { return "TwoWheeler"; }

    double calculateCharge(int days, double litresUsed) const override {
        return days * getDailyRate() + litresUsed * perLitreRate;
    }
};

class Car : public MotorVehicle {
private:
    int seatingCapacity;
    double perLitreRate;

protected:
    int getSeatingCapacity() const { return seatingCapacity; }

public:
    Car(std::string reg, double rate, double fuelPct, Date due, int seats, double fuelRate = 102.0)
        : MotorVehicle(std::move(reg), rate, fuelPct, due),
          seatingCapacity(seats), perLitreRate(fuelRate) {}

    std::string category() const override { return "Car"; }

    double calculateCharge(int days, double litresUsed) const override {
        return days * getDailyRate() + litresUsed * perLitreRate;
    }

    void printDetails(std::ostream& os) const override {
        MotorVehicle::printDetails(os);
        os << " | Seats=" << seatingCapacity;
    }
};

class ElectricCar : public Car {
private:
    double batteryKWh;
    double perKWhRate;

public:
    ElectricCar(std::string reg, double rate, double chargePct, Date due,
                int seats, double kwhCap, double kwhRate)
        : Car(std::move(reg), rate, chargePct, due, seats, 0.0),
          batteryKWh(kwhCap), perKWhRate(kwhRate) {}

    std::string category() const override { return "ElectricCar"; }

    double calculateCharge(int days, double kwhUsed) const override {
        return days * getDailyRate() + kwhUsed * perKWhRate;
    }

    void printDetails(std::ostream& os) const override {
        Car::printDetails(os);
        os << " | Battery=" << batteryKWh << "kWh";
    }
};

// Stage 5 Extension: New vehicle type added WITHOUT modifying any existing class!
class ElectricBus : public ElectricCar {
    bool wheelchairRamp;

public:
    ElectricBus(std::string reg, double rate, double chargePct, Date due,
                int seats, double kwhCap, double kwhRate, bool ramp)
        : ElectricCar(std::move(reg), rate, chargePct, due, seats, kwhCap, kwhRate),
          wheelchairRamp(ramp) {}

    std::string category() const override { return "ElectricBus"; }

    double calculateCharge(int days, double kwhUsed) const override {
        // Fleet commercial surcharge of Rs 500/day + energy usage
        return ElectricCar::calculateCharge(days, kwhUsed) + days * 500.0;
    }

    void printDetails(std::ostream& os) const override {
        ElectricCar::printDetails(os);
        os << " | Ramp=" << (wheelchairRamp ? "Yes" : "No");
    }
};

int main() {
    std::vector<std::unique_ptr<Vehicle>> fleet;
    fleet.push_back(std::make_unique<Bicycle>("CYC-01", 150.0, true));
    fleet.push_back(std::make_unique<TwoWheeler>("TN-09-AB-1234", 500.0, 85.0, Date(15, 12, 2026), 102.0));
    fleet.push_back(std::make_unique<Car>("TN-01-CD-5678", 2200.0, 70.0, Date(20, 1, 2027), 5, 102.0));
    fleet.push_back(std::make_unique<ElectricCar>("TN-07-EV-9001", 2800.0, 92.0, Date(10, 2, 2027), 5, 60.0, 14.0));
    fleet.push_back(std::make_unique<ElectricBus>("TN-01-EB-7777", 6500.0, 88.0, Date(5, 3, 2027), 32, 250.0, 12.0, true));

    std::cout << "=== Task 6: Extensible Vehicle Rental Fleet ===\n";
    for (auto& v : fleet) {
        v->rentOut();
        v->printDetails(std::cout);
        std::cout << '\n';
    }

    std::cout << "\n--- Returning Vehicles (Including Late Fee & Extension Demo) ---\n";
    double usageSamples[5] = {0.0, 4.5, 18.0, 45.0, 140.0};
    int daysSamples[5]     = {2,   3,   2,    3,    2};
    int lateSamples[5]     = {0,   0,   1,    0,    1}; // Car and ElectricBus returned 1 day late

    for (std::size_t i = 0; i < fleet.size(); ++i) {
        double bill = fleet[i]->returnVehicle(daysSamples[i], lateSamples[i], usageSamples[i]);
        std::cout << fleet[i]->category() << " (" << fleet[i]->getRegNo() << ")"
                  << " | Days=" << daysSamples[i] << ", Late=" << lateSamples[i]
                  << " -> Total Bill = Rs " << std::fixed << std::setprecision(2) << bill << '\n';
    }

    return 0;
}
