#include <chrono>
#include <cstring>
#include <iostream>
#include <string>

class Text {
private:
    std::size_t len;
    char* buf;

    Text(std::size_t length, char* ownedBuffer) : len(length), buf(ownedBuffer) {}

public:
    static inline int copyCtorCount = 0;

    Text(const char* s = "") {
        if (s == nullptr) s = "";
        len = std::strlen(s);
        buf = new char[len + 1];
        std::memcpy(buf, s, len + 1);
    }

    Text(const Text& other) : len(other.len), buf(new char[other.len + 1]) {
        ++copyCtorCount;
        std::memcpy(buf, other.buf, len + 1);
    }

    Text& operator=(const Text& other) {
        if (this == &other) return *this;
        char* newBuf = new char[other.len + 1];
        std::memcpy(newBuf, other.buf, other.len + 1);
        delete[] buf;
        buf = newBuf;
        len = other.len;
        return *this;
    }

    ~Text() { delete[] buf; }

    std::size_t length() const { return len; }

    friend Text operator+(const Text& lhs, const Text& rhs) {
        std::size_t totalLen = lhs.len + rhs.len;
        char* combined = new char[totalLen + 1];
        std::memcpy(combined, lhs.buf, lhs.len);
        std::memcpy(combined + lhs.len, rhs.buf, rhs.len + 1);
        return Text(totalLen, combined);
    }

    friend bool operator==(const Text& lhs, const Text& rhs) {
        return lhs.len == rhs.len && std::strcmp(lhs.buf, rhs.buf) == 0;
    }

    friend bool operator!=(const Text& lhs, const Text& rhs) { return !(lhs == rhs); }

    friend std::ostream& operator<<(std::ostream& os, const Text& t) { return os << t.buf; }
};

int main() {
    std::cout << "=== Q6: SMS Gateway Custom Text Class ===\n";
    Text emptyMsg("");
    Text a("OTP: ");
    Text b("482910");
    Text c(" (Valid for 5 mins)");

    Text::copyCtorCount = 0;
    Text sms = a + b + c;
    std::cout << "Concatenated SMS: \"" << sms << "\" (len=" << sms.length() << ")\n";
    std::cout << "Copy constructor calls in (a + b + c): " << Text::copyCtorCount
              << " (C++17 guaranteed copy elision / NRVO)\n";

    Text& ref = sms;
    sms = ref;
    std::cout << "After self-assignment: \"" << sms << "\"\n";

    Text combinedEmpty = emptyMsg + emptyMsg;
    std::cout << "Empty == combinedEmpty? " << std::boolalpha << (emptyMsg == combinedEmpty)
              << " | sms != emptyMsg? " << (sms != emptyMsg) << '\n';

    std::string longRaw(500, 'X');
    Text longMsg(longRaw.c_str());
    std::cout << "Long message length: " << longMsg.length() << " chars\n";

    constexpr int ITERS = 100000;
    auto t1Start = std::chrono::high_resolution_clock::now();
    Text tAcc(""), tChunk("SMS");
    for (int i = 0; i < ITERS; ++i) tAcc = tAcc + tChunk;
    auto t1End = std::chrono::high_resolution_clock::now();

    auto t2Start = std::chrono::high_resolution_clock::now();
    std::string sAcc, sChunk("SMS");
    for (int i = 0; i < ITERS; ++i) sAcc += sChunk;
    auto t2End = std::chrono::high_resolution_clock::now();

    std::cout << "\n--- 100,000 Concatenations Benchmark ---\n";
    std::cout << "Custom Text (a = a + chunk, O(N^2))       : "
              << std::chrono::duration<double, std::milli>(t1End - t1Start).count() << " ms (len=" << tAcc.length() << ")\n";
    std::cout << "std::string (s += chunk, amortized O(N))  : "
              << std::chrono::duration<double, std::milli>(t2End - t2Start).count() << " ms (len=" << sAcc.length() << ")\n";

    return 0;
}
