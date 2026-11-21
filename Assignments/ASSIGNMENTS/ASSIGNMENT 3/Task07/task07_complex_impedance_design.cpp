#include <cmath>
#include <complex>
#include <iomanip>
#include <iostream>
#include <stdexcept>

class Complex {
private:
    double re;
    double im;

public:
    static constexpr double EPS = 1e-12;

    Complex() : re(0.0), im(0.0) {}
    Complex(double r) : re(r), im(0.0) {}
    Complex(double r, double i) : re(r), im(i) {}

    double real() const { return re; }
    double imag() const { return im; }
    double magnitude() const { return std::hypot(re, im); }
    double phaseDegrees() const { return std::atan2(im, re) * (180.0 / 3.14159265358979323846); }

    friend Complex operator+(const Complex& a, const Complex& b) { return Complex(a.re + b.re, a.im + b.im); }
    friend Complex operator-(const Complex& a, const Complex& b) { return Complex(a.re - b.re, a.im - b.im); }
    friend Complex operator*(const Complex& a, const Complex& b) {
        return Complex(a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re);
    }
    friend Complex operator/(const Complex& a, const Complex& b) {
        double denom = b.re * b.re + b.im * b.im;
        if (denom <= EPS) {
            throw std::domain_error("Division by zero impedance (short-circuit condition)");
        }
        return Complex((a.re * b.re + a.im * b.im) / denom,
                       (a.im * b.re - a.re * b.im) / denom);
    }
    friend bool operator==(const Complex& a, const Complex& b) {
        return std::fabs(a.re - b.re) <= EPS && std::fabs(a.im - b.im) <= EPS;
    }
    friend std::ostream& operator<<(std::ostream& os, const Complex& z) {
        os << std::fixed << std::setprecision(4) << z.re;
        if (z.im >= 0.0) os << " + j" << z.im;
        else os << " - j" << (-z.im);
        return os;
    }
};

int main() {
    std::cout << "=== Q7: AC Circuit Impedance Calculator ===\n";
    double omega = 1000.0;
    Complex Z_R(30.0);
    Complex Z_L(0.0, omega * 0.08);
    Complex Z_C(0.0, -1.0 / (omega * 25e-6));

    Complex Z_series = Z_R + Z_L + Z_C;
    std::cout << "Component Impedances:\n";
    std::cout << "  Z_R = " << Z_R << " ohm\n";
    std::cout << "  Z_L = " << Z_L << " ohm\n";
    std::cout << "  Z_C = " << Z_C << " ohm\n";
    std::cout << "\nSeries RLC Impedance (Z_R + Z_L + Z_C):\n";
    std::cout << "  Z_series = " << Z_series << " ohm | |Z| = " << Z_series.magnitude()
              << " ohm | Phase = " << Z_series.phaseDegrees() << " deg\n";
    std::cout << "  Matches hand calculation (30 + j40, |Z|=50)? "
              << std::boolalpha << (Z_series == Complex(30.0, 40.0)) << '\n';

    Complex Z1(30.0, 0.0), Z2(0.0, 40.0);
    Complex Z_parallel = (Z1 * Z2) / (Z1 + Z2);
    std::cout << "\nParallel RL Impedance ((30) || (j40)):\n";
    std::cout << "  Z_parallel = " << Z_parallel << " ohm | |Z| = " << Z_parallel.magnitude() << " ohm\n";
    std::cout << "  Matches hand calculation (19.2 + j14.4, |Z|=24)? "
              << (Z_parallel == Complex(19.2, 14.4)) << '\n';

    std::complex<double> sz1(30.0, 0.0), sz2(0.0, 40.0);
    std::complex<double> szp = (sz1 * sz2) / (sz1 + sz2);
    std::cout << "  std::complex<double> reference: " << szp.real() << " + j" << szp.imag() << '\n';

    std::cout << "\n--- Division by Zero Impedance Test ---\n";
    try {
        Complex Z_short;
        Complex Z_fault = Z1 / Z_short;
        std::cout << Z_fault << '\n';
    } catch (const std::domain_error& ex) {
        std::cout << "Caught expected fault: " << ex.what() << '\n';
    }
    return 0;
}
