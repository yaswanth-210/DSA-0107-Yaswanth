#include <iostream>
#include <string>

struct LargeStudentRecord {
    std::string regNo = "192511172";
    std::string fullName = "Samrakshini G";
    std::string hostelBlock = "Block-C Emerald";
    long long grades[64]{};
    long long tuitionFeePaise = 4500000;
};

void applyConcessionByValue(long long feePaise, int percent) {
    feePaise -= (feePaise * percent) / 100;
    std::cout << "  [Inside ByValue]     local fee = Rs " << (feePaise / 100) << '\n';
}

void applyConcessionByRef(long long& feePaise, int percent) {
    feePaise -= (feePaise * percent) / 100;
}

void applyConcessionByPtr(long long* feePaise, int percent) {
    if (feePaise != nullptr) {
        *feePaise -= (*feePaise * percent) / 100;
    }
}

long long computeConcessionFee(long long feePaise, int percent) {
    return feePaise - (feePaise * percent) / 100;
}

void printReceipt(const LargeStudentRecord& student) {
    std::cout << "Receipt -> " << student.regNo << " (" << student.fullName
              << ") | Payable: Rs " << (student.tuitionFeePaise / 100) << '\n';
}

int main() {
    long long fee = 4500000; // Rs 45,000.00 in paise
    std::cout << "Initial Hostel Fee: Rs " << (fee / 100) << "\n\n";

    applyConcessionByValue(fee, 10); // Merit 10%
    std::cout << "After ByValue(10%)   -> Stored Fee: Rs " << (fee / 100) << "\n\n";

    applyConcessionByRef(fee, 15);   // Sports 15%
    std::cout << "After ByRef(15%)     -> Stored Fee: Rs " << (fee / 100) << "\n\n";

    applyConcessionByPtr(&fee, 25);  // Single-Parent 25%
    std::cout << "After ByPtr(25%)     -> Stored Fee: Rs " << (fee / 100) << "\n\n";

    long long baseFee = 4500000;
    long long finalFee = computeConcessionFee(computeConcessionFee(baseFee, 10), 15);
    std::cout << "Chained Functional (10% then 15% on Rs 45000): Rs " << (finalFee / 100) << "\n\n";

    LargeStudentRecord rec;
    rec.tuitionFeePaise = finalFee;
    printReceipt(rec);
    std::cout << "sizeof(long long) = " << sizeof(long long)
              << " bytes | sizeof(LargeStudentRecord) = " << sizeof(LargeStudentRecord) << " bytes\n";
    return 0;
}
