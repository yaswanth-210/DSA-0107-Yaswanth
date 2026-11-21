#include <iostream>
#include <string>

class AuditoriumEvent {
private:
    std::string eventName;
    int rows;
    int cols;
    char** seatMap;

public:
    AuditoriumEvent(const std::string& name, int r, int c)
        : eventName(name), rows(r > 0 ? r : 0), cols(c > 0 ? c : 0), seatMap(nullptr) {
        if (rows > 0 && cols > 0) {
            seatMap = new char*[rows];
            for (int i = 0; i < rows; ++i) {
                seatMap[i] = new char[cols];
                for (int j = 0; j < cols; ++j) {
                    seatMap[i][j] = 'O';
                }
            }
        }
        std::cout << "[Constructed] Event '" << eventName << "' (" << rows << "x" << cols
                  << ", bytes=" << allocatedBytes() << ")\n";
    }

    AuditoriumEvent(const AuditoriumEvent&) = delete;
    AuditoriumEvent& operator=(const AuditoriumEvent&) = delete;

    ~AuditoriumEvent() {
        int bookedCount = countBooked();
        if (seatMap != nullptr) {
            for (int i = 0; i < rows; ++i) {
                delete[] seatMap[i];
            }
            delete[] seatMap;
        }
        std::cout << "[Destroyed]   Event '" << eventName << "' closed | Booked="
                  << bookedCount << "/" << (rows * cols) << " | All heap memory freed\n";
    }

    bool bookSeat(int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= cols || seatMap == nullptr || seatMap[r][c] == 'B') return false;
        seatMap[r][c] = 'B';
        return true;
    }

    bool cancelSeat(int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= cols || seatMap == nullptr || seatMap[r][c] == 'O') return false;
        seatMap[r][c] = 'O';
        return true;
    }

    int countBooked() const {
        if (seatMap == nullptr) return 0;
        int total = 0;
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                if (seatMap[i][j] == 'B') ++total;
        return total;
    }

    std::size_t allocatedBytes() const {
        if (rows <= 0 || cols <= 0) return 0;
        return static_cast<std::size_t>(rows) * sizeof(char*) +
               static_cast<std::size_t>(rows) * static_cast<std::size_t>(cols) * sizeof(char);
    }
};

struct Logger {
    std::string n;
    Logger(const std::string& s) : n(s) { std::cout << '+' << n << ' '; }
    Logger(const Logger& o) : n(o.n + "c") { std::cout << "copy" << n << ' '; }
    ~Logger() { std::cout << '-' << n << ' '; }
};

void use(Logger) { std::cout << "use "; }

int main() {
    std::cout << "=== Q3 Part A: Predict the Output Verification ===\n";
    {
        Logger a("A");
        {
            Logger b("B");
        }
        use(a);
        Logger* p = new Logger("P");
        delete p;
        std::cout << "end ";
    }
    std::cout << "\n\n=== Q3 Part B: Auditorium Dynamic Seat Map Lifecycle ===\n";

    AuditoriumEvent stackEvent("Annual Tech Fest", 5, 10);
    stackEvent.bookSeat(0, 1);
    stackEvent.bookSeat(2, 4);
    stackEvent.cancelSeat(0, 1);

    {
        AuditoriumEvent blockEvent("Guest Lecture", 4, 8);
        blockEvent.bookSeat(1, 2);
    }

    AuditoriumEvent* heapEvent = new AuditoriumEvent("Symphony Night", 6, 12);
    heapEvent->bookSeat(3, 5);
    delete heapEvent;

    AuditoriumEvent emptyEvent("Cancelled Show", 0, 10);

    std::cout << "\n--- Memory Footprint Measurements (64-bit pointers) ---\n";
    int dims[3][2] = {{10, 20}, {100, 200}, {1000, 2000}};
    for (auto& d : dims) {
        AuditoriumEvent m("Bench", d[0], d[1]);
        std::cout << "  " << d[0] << " x " << d[1] << " -> " << m.allocatedBytes() << " bytes\n";
    }

    std::cout << "\n--- Exiting main() (stackEvent destroyed next) ---\n";
    return 0;
}
