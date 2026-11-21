# DSA01-192511172-Assignment5 — Reliable Software with Runtime Polymorphism and Exceptions (CO5)

- **Student Name:** Samrakshini G
- **Register Number:** 192511172
- **Course:** DSA0107 — Object Oriented Programming with C++ (CO5)
- **Compiler & Flags:** g++ 13 (`g++ -std=c++17 -Wall -Wextra`, plus `-fsanitize=address -g` and `-O2` benchmarks)

## Task Folders & Results
1. `TaskA1_Polymorphic_Hierarchy/` — `Bill` abstract hierarchy (`OutPatient`, `InPatient`, `DayCare`, `Emergency`), UML diagram, SIMATS pseudocode, and switch/Big-O analysis.
2. `TaskA2_Runtime_Binding/` — Early vs. late binding, `vtable` layouts, virtual destructor rationale, `dynamic_cast` analysis, and `Bill::show()` prediction output.
3. `TaskA3_Abstract_Payers/` — `Payer` interface with `SelfPay`, `Insurance`, and `GovernmentScheme`, extension path, and function-object comparison.
4. `TaskA4_Exception_Strategy/` — Domain exception hierarchy (`BillingError`, `ClaimError`), `noexcept` rules, and return-code vs. exception analysis.
5. `TaskA5_Recovery_Rethrow/` — Catch/log/rethrow strategy, RAII stack unwinding, `Guard` prediction output, and manual `delete` risk analysis.
6. `TaskB1_Polymorphic_Billing/` — Completed `TODO 1–3` code, verified totals, virtual removal & ASan evidence, and `std::variant` / template comparison.
7. `TaskB2_Error_Conditions/` — `validate()` prediction output, completed `ClaimError` `TODO 1–2`, error stress tests, `std::bad_alloc`, and `std::optional` comparison.
8. `TaskB3_Cost_Comparison/` — 10-bill correctness check, 1k / 1M / 10M dispatch benchmarks (`-O2`), 0% / 1% / 50% exception overhead, and `std::visit` comparison.
9. `TaskB4_Final_Decision/` — Added `DayCareSurgeryBill` and `CorporateTieUp` without modifying existing classes, clean ASan run, and production decision.
