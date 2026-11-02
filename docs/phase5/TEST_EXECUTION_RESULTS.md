# Phase 5 — Executed Test Results and Evidence

**Execution date:** 1 November 2026.
**Executed environment:** Linux x86-64 development container; CMake 3.31.6; GCC 14.2.0; SQLite 3.46.1; Clang 17.0.0. These are development-container results, not the designated Windows/school workstation results.

## Result summary

The production C++17 application configured and built successfully. The application database initialized with ten tables. Runtime and external SQLite schema definitions matched across all ten tables and foreign-key relationships. The additive migration from a legacy extension table without `extension_scope` preserved the existing row and assigned it the default scope `INDIVIDUAL`. The standalone business-rule assertion harness passed. A scripted console workflow successfully exercised user provisioning, assignment creation, a class extension, an individual exception, application shutdown/restart and student deadline display.

The Catch2 v3 suite was not executed: CMake FetchContent attempted to clone Catch2 from GitHub, but this container could not resolve `github.com`. The Catch2 tests are present in `tests/test_business_rules.cpp`; run them through GitHub Actions or a network-enabled environment. The target Windows performance, usability, offline and backup/restore tests remain pending.

## Executed rule-level cases

| Case | Actual outcome | Status |
|---|---|---|
| Calendar-day calculation: same date, before deadline, 1, 2, 3 and 4 days late | Returned 0, 0, 1, 2, 3 and 4 late days respectively; leap-day interval was correct. | PASS |
| Date/time validation | Rejected 2025-02-29, month 13 and hour 25. | PASS |
| Late penalty with maximum 100 | Calculated 100, 90, 80, 70 and 0 for 0–4 late days; rejected negative maximum and late-day values. | PASS |
| Resubmission boundary | Allowed before and exactly at effective deadline; rejected one second after; a first attempt remained eligible for late handling. | PASS |
| Class extension roster | Two of three enrolled students received a CLASS grant; an existing INDIVIDUAL grant was preserved; unrelated assignment unchanged. | PASS |
| Extension authorisation and validation | Rejected null DB, invalid IDs, lecturer who does not own assignment, malformed deadline, empty reason, deadline earlier than original, and empty class. | PASS |
| Repeated class extension | Later class grant updated CLASS rows; a shorter class deadline was rejected; individual exception was unchanged. | PASS |
| Extension labels | Returned Class extension, Individual extension and No extension for the respective scope values. | PASS |
| Transaction atomicity | A forced SQLite trigger failure rolled back all rows from the class grant; existing individual row remained. | PASS |

## Executed application workflow

The application was initialized in a temporary test folder. The Administrator created a department, Lecturer and two Student accounts, a course, lecturer assignment and two enrolments. The Lecturer created an assignment and granted a class extension, then granted an individual extension to Student A. The application process was closed and restarted. Student A saw the individual deadline and `Individual extension` label; Student B saw the class-wide deadline and `Class extension` label. The database rows were inspected after restart. Result: PASS for this CR1 workflow on the Linux development container. The evidence transcript is `docs/phase8/CR1_END_TO_END_TEST_OUTPUT.txt`.

## Coverage result and qualification

The manual assertion harness was compiled with GCC coverage instrumentation and executed against `src/business_rules.cpp`. gcov reported **84.18% line execution (158 executable lines)** and **78.91% of 294 branches executed**; **49.32% of branches were taken at least once**. This is a preliminary business-rule smoke-harness measurement, not the final Catch2/CI coverage report. The assignment's CI coverage gate remains 70% line coverage for the business-logic file and must be evidenced by a successful GitHub Actions artifact.

Reproduction command (from repository root, with GCC and SQLite headers/library):

```bash
g++ -std=c++17 -O0 -g --coverage -Isrc src/business_rules.cpp tests/manual_business_rules_smoke.cpp -lsqlite3 -o manual_business_rules_smoke
./manual_business_rules_smoke
gcov -b -c manual_business_rules_smoke-business_rules.gcno
```

## Pending test execution

- Catch2/CTest automated suite and CI coverage artefact.
- All unexecuted entries in `SYSTEM_ACCEPTANCE_TESTS.csv`, including negative-password tests, role cross-access, HoD moderation/release, mark audit, complete file submission paths and exception recovery.
- NFR-01 performance with 800 student records on the target school computer.
- NFR-02 full offline workflow on the target machine.
- NFR-07 representative-user usability observation for all four roles.
- NFR-09 timed backup and restore test (within 15 minutes).
