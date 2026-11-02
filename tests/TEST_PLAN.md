# Phase 5 test entry point

Automated unit and SQLite integration tests are in `tests/test_business_rules.cpp` and are executed with Catch2 v3 by CTest. The V-Model test strategy and test design are in `docs/phase5/TEST_STRATEGY.md`. System/acceptance execution sheet: `docs/phase5/SYSTEM_ACCEPTANCE_TESTS.csv`. Defect log: `docs/phase5/DEFECT_LOG.csv`.

Run: `cmake -S . -B build -DBUILD_TESTING=ON`; `cmake --build build`; `ctest --test-dir build --output-on-failure`. Coverage (GCC): configure with `-DENABLE_COVERAGE=ON`, then generate using gcovr as described in CI. Fill Actual result/Status/Evidence columns only after each test is executed.
