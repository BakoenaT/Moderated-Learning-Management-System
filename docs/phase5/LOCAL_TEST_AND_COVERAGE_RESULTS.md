# Local Build, Integration, Smoke Test and Coverage Evidence

**Execution date:** 1 November 2026 (development container; Linux x86-64, not the student's Windows target workstation).

| Activity | Environment / tool | Result | Evidence / limitation |
|---|---|---|---|
| Production configure and build | CMake 3.31.6; GCC 14.2.0; SQLite 3.46.1; OpenSSL discovered by CMake | PASS | `BUILD_OUTPUT_LINUX.txt`; BUILD_TESTING=OFF because Catch2 could not be fetched in this environment. |
| Application database initialization | Built `moderated_lms --init-only` | PASS | Created ten application tables and added extension_scope. |
| Runtime/schema parity | Python sqlite3; runtime SQL extracted from main.cpp compared with database/schema.sql | PASS | Same ten tables, columns and foreign-key definitions; no foreign-key check violations. See `SCHEMA_PARITY_RESULTS.txt`. |
| Legacy database migration | Existing extension_grant table lacked extension_scope | PASS | Startup added the column, preserved old deadline/reason and assigned `INDIVIDUAL`; see `MIGRATION_TEST_RESULTS.txt`. |
| Manual business-rule harness | GCC 14.2; SQLite; gcov | PASS | Output: `PASS: business rule smoke assertions`; includes calendar boundaries, invalid dates, penalty, resubmission, CR1 roster/exception, later deadline update, validation and rollback. |
| CR1 console end-to-end workflow | Built application; temporary persistent SQLite database; two application processes | PASS | Administrator provisioning, lecturer class extension and individual exception; after restart each student saw their own effective deadline and extension label. See `docs/phase8/CR1_END_TO_END_TEST_OUTPUT.txt`. |
| Business-logic coverage | gcov on `src/business_rules.cpp` | 84.18% line execution (158 executable lines); 78.91% branches executed; 49.32% branches taken at least once | Preliminary manual-harness result only. Not the Catch2/CI coverage artifact. |
| Clang Static Analyzer | Clang 17.0.0; `clang++ --analyze` on main.cpp and business_rules.cpp | PASS | No diagnostics emitted; output files in `docs/phase6/`. |
| Catch2 / CTest | Catch2 v3.7.1 via CMake FetchContent | BLOCKED / PENDING | Git clone failed because the container could not resolve github.com. The suite code exists but was not run. |
| clang-tidy / Lizard | Tools not installed in local container | PENDING | Run in CI/network-enabled environment; exact analyzer/complexity results not yet available. |
| Target Windows acceptance | Student's Windows workstation | NOT RUN | MSVC compiler setup, system acceptance, offline, performance, usability and backup/restore evidence must be recorded on the target machine. |

Do not represent the local smoke-harness coverage as the required Catch2/CI coverage artifact. Do not mark the target-machine acceptance tests as passed until executed and supported by retained evidence.
