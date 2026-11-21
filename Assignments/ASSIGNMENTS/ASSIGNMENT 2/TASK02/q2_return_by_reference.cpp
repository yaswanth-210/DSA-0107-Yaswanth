#include <iostream>
#include <vector>
#include <cstddef>

int& highestMarkRef(int marks[], int n) {
    int maxIdx = 0;
    for (int i = 1; i < n; ++i) {
        if (marks[i] > marks[maxIdx]) maxIdx = i;
    }
    return marks[maxIdx];
}

const int& highestMarkConstRef(const int marks[], int n) {
    int maxIdx = 0;
    for (int i = 1; i < n; ++i) {
        if (marks[i] > marks[maxIdx]) maxIdx = i;
    }
    return marks[maxIdx];
}

int& danglingLocalRef() {
    int localTemp = 99;
    return localTemp;
}

std::size_t highestMarkIndex(const std::vector<int>& marks) {
    std::size_t maxIdx = 0;
    for (std::size_t i = 1; i < marks.size(); ++i) {
        if (marks[i] > marks[maxIdx]) maxIdx = i;
    }
    return maxIdx;
}

int main() {
    int examMarks[5] = {74, 88, 92, 81, 85};
    std::cout << "Initial marks: ";
    for (int m : examMarks) std::cout << m << ' ';
    std::cout << '\n';

    highestMarkRef(examMarks, 5) += 2;
    std::cout << "After +2 moderation via reference: ";
    for (int m : examMarks) std::cout << m << ' ';
    std::cout << '\n';

    highestMarkRef(examMarks, 5) = 100;
    std::cout << "After L-value assignment (= 100): ";
    for (int m : examMarks) std::cout << m << ' ';
    std::cout << '\n';

    const int& topConstRef = highestMarkConstRef(examMarks, 5);
    int topCopy = highestMarkConstRef(examMarks, 5);
    topCopy -= 10;
    std::cout << "topConstRef = " << topConstRef << " | modified topCopy = " << topCopy << "\n\n";

    std::vector<int> vecMarks;
    vecMarks.reserve(2);
    vecMarks.push_back(70);
    vecMarks.push_back(91);

    const int* oldAddr = &vecMarks[1];
    std::size_t safeIdx = highestMarkIndex(vecMarks);
    std::cout << "Before vector growth: highest = " << vecMarks[safeIdx]
              << " at address " << oldAddr << " (capacity=" << vecMarks.capacity() << ")\n";

    vecMarks.push_back(84);
    vecMarks.push_back(79);
    const int* newAddr = &vecMarks[safeIdx];
    std::cout << "After vector growth : highest = " << vecMarks[safeIdx]
              << " at address " << newAddr << " (capacity=" << vecMarks.capacity() << ")\n";
    std::cout << "Pointer/reference invalidated? " << std::boolalpha << (oldAddr != newAddr) << '\n';

    (void)&danglingLocalRef;
    return 0;
}
