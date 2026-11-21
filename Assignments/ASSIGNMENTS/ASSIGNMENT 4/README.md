# DSA01-192511172-Assignment4 — Designing Reusable Class Hierarchies (CO4)

- **Student Name:** Samrakshini G
- **Register Number:** 192511172
- **Course:** DSA0107 — Object Oriented Programming with C++ (CO4)
- **Compiler & Version:** g++ 13 (`g++ -std=c++17 -Wall -Wextra`)

## Repository Structure
- `Task01/` — College Payroll Hierarchy (`Staff` -> `SalariedStaff` -> `Professor`, `LabAssistant`; `GuestFaculty`)
- `Task02/` — University People Management (Virtual Inheritance Diamond Resolution: `Person`, `Student`, `Staff`, `TeachingAssistant`, `ResearchScholar`)
- `Task03/` — Constructors in Derived & Member Classes (`Address`, `Course`, construction/destruction trace, virtual vs. non-virtual `sizeof`)
- `Task04/` — Pointers to Derived Classes Forensics (`Account` / `SeniorAccount`, `virtual` dispatch, `virtual` destructor, AddressSanitizer & `std::unique_ptr`)
- `Task05/` — Pointers to Objects & `this` Pointer (`Cart` method chaining, 4 access modes, pointer-arithmetic highest-total search)
- `Task06/` — Vehicle Rental Fleet Design Challenge (Abstract `Vehicle`, `Bicycle`, `MotorVehicle`, `TwoWheeler`, `Car`, `ElectricCar`, and `ElectricBus` extension)

## How to Compile and Run
```bash
g++ -std=c++17 -Wall -Wextra Task01/task01_payroll_hierarchy.cpp -o task01 && ./task01
g++ -std=c++17 -Wall -Wextra Task02/task02_university_hierarchy.cpp -o task02 && ./task02
g++ -std=c++17 -Wall -Wextra Task03/task03_constructors_member_classes.cpp -o task03 && ./task03
g++ -std=c++17 -Wall -Wextra -fsanitize=address -g Task04/task04_pointers_forensics.cpp -o task04 && ./task04
g++ -std=c++17 -Wall -Wextra Task05/task05_pointers_and_this.cpp -o task05 && ./task05
g++ -std=c++17 -Wall -Wextra Task06/task06_rental_fleet_design.cpp -o task06 && ./task06
```
