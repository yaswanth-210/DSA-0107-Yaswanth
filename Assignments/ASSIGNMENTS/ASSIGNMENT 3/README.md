# DSA01-192511172-Assignment3 — Constructors, Destructors & Operator Overloading (CO3)

- **Student Name:** Samrakshini G
- **Register Number:** 192511172
- **Course:** DSA0107 — Object Oriented Programming with C++ (CO3)
- **Compiler & Version:** g++ 13 (`g++ -std=c++17 -Wall -Wextra`)

## Repository Structure
- `Task01/` — Library Book Records (`Book` default, parameterised & delegating constructors, `explicit`, `-Wreorder`, negative copy validation)
- `Task02/` — Hospital Prescription Records (`Prescription` deep-copy constructor, copy assignment, destructor, pass-by-value vs. const-ref, AddressSanitizer)
- `Task03/` — Auditorium Seat Booking (`AuditoriumEvent` dynamic 2D constructor/destructor, `Logger` lifecycle order, memory footprint scaling)
- `Task04/` — Money Billing Counter (`Money` `+`, `-`, `+=`, `==`, `<`, `<<`, `>>`, paise carry, `1,000,000` additions vs. `double`)
- `Task05/` — Music Playlist & Track Timer (`Time` prefix/postfix `++`, `==`, `<`, `Playlist` const/non-const `[]`, `10,000,000` benchmark)
- `Task06/` — SMS Gateway `Text` Class (`Text` Rule of Three, `+`, `==`, `!=`, `<<`, NRVO copy count, `100,000` concatenations vs. `std::string`)
- `Task07/` — AC Circuit Impedance Calculator (`Complex` first-principles design, series/parallel RLC verification, division-by-zero safety)

## How to Compile and Run
```bash
g++ -std=c++17 -Wall -Wextra Task01/task01_constructors_overloading.cpp -o task01 && ./task01
g++ -std=c++17 -Wall -Wextra -fsanitize=address -g Task02/task02_copy_constructors.cpp -o task02 && ./task02
g++ -std=c++17 -Wall -Wextra -fsanitize=address -g Task03/task03_dynamic_constructors_destructors.cpp -o task03 && ./task03
g++ -std=c++17 -Wall -Wextra Task04/task04_arithmetic_stream_operators.cpp -o task04 && ./task04
g++ -std=c++17 -Wall -Wextra -O2 Task05/task05_increment_comparison_subscript.cpp -o task05 && ./task05
g++ -std=c++17 -Wall -Wextra -O2 Task06/task06_operator_overloading_rules.cpp -o task06 && ./task06
g++ -std=c++17 -Wall -Wextra Task07/task07_complex_impedance_design.cpp -o task07 && ./task07
```
