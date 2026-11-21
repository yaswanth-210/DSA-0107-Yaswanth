#include <chrono>
#include <iostream>
#include <string>

class Prescription {
    int n;
    std::string* medicines;         // dynamically allocated array

public:
    static inline int copyCount = 0;

    explicit Prescription(int count)
        : n(count), medicines(count > 0 ? new std::string[count] : nullptr) {}

    // TODO 1: Copy constructor (deep copy)
    Prescription(const Prescription& other)
        : n(other.n), medicines(other.n > 0 ? new std::string[other.n] : nullptr) {
        ++copyCount;
        for (int i = 0; i < n; ++i) {
            medicines[i] = other.medicines[i];
        }
    }

    // TODO 2: Copy assignment with self-assignment guard
    Prescription& operator=(const Prescription& other) {
        if (this == &other) {
            return *this;
        }
        std::string* newArr = other.n > 0 ? new std::string[other.n] : nullptr;
        for (int i = 0; i < other.n; ++i) {
            newArr[i] = other.medicines[i];
        }
        delete[] medicines;
        medicines = newArr;
        n = other.n;
        return *this;
    }

    ~Prescription() {
        // TODO 3: Release dynamically allocated array
        delete[] medicines;
    }

    void set(int i, const std::string& m) { medicines[i] = m; }
    std::string get(int i) const { return medicines[i]; }
    int size() const { return n; }
};

void inspectByValue(Prescription copyRx) {
    (void)copyRx.size();
}

void inspectByConstRef(const Prescription& refRx) {
    (void)refRx.size();
}

double measureCopyTimeMs(int medCount, int iterations) {
    Prescription src(medCount);
    for (int i = 0; i < medCount; ++i) {
        src.set(i, "Medicine_" + std::to_string(i));
    }
    auto start = std::chrono::high_resolution_clock::now();
    for (int it = 0; it < iterations; ++it) {
        Prescription dup = src;
        dup.set(0, "Updated");
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count() / iterations;
}

int main() {
    Prescription p(2);
    p.set(0, "Paracetamol");
    p.set(1, "ORS");
    Prescription q = p;                 // copy constructor
    q.set(1, "Cetirizine");
    Prescription r(1);
    r = p;                              // copy assignment
    p.set(0, "Ibuprofen");
    std::cout << p.get(0) << ' ' << p.get(1) << " | " << q.get(0) << ' ' << q.get(1)
              << " | " << r.get(0) << ' ' << r.get(1) << '\n';

    Prescription& alias = p;
    p = alias;
    std::cout << "After self-assignment (p = p): " << p.get(0) << ' ' << p.get(1) << '\n';

    Prescription::copyCount = 0;
    inspectByValue(p);
    int byValCopies = Prescription::copyCount;

    Prescription::copyCount = 0;
    inspectByConstRef(p);
    int byRefCopies = Prescription::copyCount;
    std::cout << "Copies made -> pass-by-value: " << byValCopies
              << ", pass-by-const-ref: " << byRefCopies << '\n';

    std::cout << "Avg deep-copy time (10 medicines)     : " << measureCopyTimeMs(10, 2000) << " ms\n";
    std::cout << "Avg deep-copy time (1,000 medicines)  : " << measureCopyTimeMs(1000, 500) << " ms\n";
    std::cout << "Avg deep-copy time (100,000 medicines): " << measureCopyTimeMs(100000, 20) << " ms\n";

    return 0;
}
