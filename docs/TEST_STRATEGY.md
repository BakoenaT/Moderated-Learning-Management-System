# Phase 5 — Verification and Validation

## 1. Purpose and test basis

This strategy verifies the Moderated LMS against the Phase 2 SRS (FR-01–FR-18, NFR-01–NFR-09 and BR-01–BR-08), the Phase 3 design, and the Phase 4 C++17/SQLite implementation. Testing follows the V-Model: each development artefact has a corresponding verification level. Tests are evidence-producing activities; a test is not marked passed until it has actually been executed and its output retained.

## 2. V-Model mapping, entry and exit criteria

| V-Model level | Development artefact verified | Main test focus | Entry criteria | Exit criteria |
|---|---|---|---|---|
| Unit | Detailed class specifications, late-penalty strategy, submission/extension rules and validation routines | Date parsing, late-day boundaries, adjusted mark maximum, resubmission deadline, class extension policy | C++17 business-rule module compiles; test data and expected results reviewed | All planned unit cases executed; all critical defects fixed or formally accepted; test summary and coverage report saved |
| Integration | Architecture, class-to-table mapping, repositories/data-access layer and SQLite schema | Prepared statements, FK/unique constraints, transaction boundaries, class-extension writes, audit persistence and file metadata | Unit tests pass; clean test database available; schema migrations applied | All interface/DB tests executed; no partial writes after failed transaction; referential integrity checks pass |
| System | Use-case model and behavioural diagrams (login, assignment, submission, marking, moderation and release flows) | Complete end-to-end role workflows, exceptions, offline execution, access boundaries and recoverability | Integrated executable built; role accounts and representative records seeded | Every Must use case has execution evidence; exception paths tested; high/critical defects closed; system test report completed |
| Acceptance | Fully dressed use cases and stakeholder needs | Principal/administrator, lecturer, HoD, student and IT officer acceptance scenarios; usability and performance targets | System tests passed; user manuals available; representative users and target machine prepared | Stakeholder acceptance criteria evaluated; deviations and sign-off recorded; no unresolved release-blocking defect |

## 3. Test design techniques

### Equivalence partitions

- Authentication: valid active account; wrong password; unknown username; inactive account.
- Roles: permitted role/action; authenticated but wrong role; correct role but wrong course/department/student scope.
- Mark values: below zero; valid inclusive range 0..adjusted maximum; above adjusted maximum; non-numeric input.
- Submission file: existing regular file; missing path; directory instead of file; inaccessible file. The current UI does not yet enforce an extension/size allow-list, which remains a release limitation.
- Extension: assigned lecturer for a class with enrolments; unassigned lecturer; assignment from another lecturer; no enrolled students; invalid/empty reason; valid revised deadline.

### Boundary-value analysis — late penalty

The current SRS assumption is calendar days. The penalty is 10% of the assignment maximum per late day, up to day 3; after day 3 the mark is zero. For a maximum of 100 marks:

| Late-day input | Expected maximum award | Boundary interpretation |
|---:|---:|---|
| -1 / earlier date | 100 | Not late; negative lateness is clamped to zero |
| 0 | 100 | Submitted on deadline date |
| 1 | 90 | First late day |
| 2 | 80 | Second late day |
| 3 | 70 | Final penalised day |
| 4 | 0 | Beyond permitted penalty window |

The business-rule tests also include leap-day and invalid-date inputs. The implementation uses calendar-date arithmetic rather than elapsed 24-hour periods so daylight-saving clock changes do not alter the calendar-day penalty.

### Resubmission boundary

A first attempt may still be recorded after the deadline and is subject to late penalties (including zero after day 3). Once a student has an existing attempt, a resubmission must be made on or before the effective deadline. The latest permitted attempt becomes active while earlier versions remain stored.

## 4. Automated unit and integration tests

Catch2 v3 tests are in `tests/test_business_rules.cpp`. They exercise:
- Calendar-day calculations and date validation.
- Penalty boundaries at 0, 1, 2, 3 and more than 3 days.
- Resubmission exactly at, before and after the effective deadline.
- CR1 class extension persistence against an in-memory SQLite database, including preservation of existing individual extensions and denial for an unassigned lecturer.

The test target is built and discovered by CTest. GitHub Actions runs the build and tests on every push and pull request. The CI job generates a gcovr line-coverage report for `src/business_rules.cpp` and enforces the 70% target. A preliminary local smoke-harness result is recorded in `LOCAL_TEST_AND_COVERAGE_RESULTS.md` (84.18% of 158 executable lines, with 78.91% of 294 branches executed). The Catch2/CI report must be regenerated from the committed test suite and retained as the formal phase evidence; the local smoke-harness figure is not a substitute for the final CI artifact.

## 5. System and acceptance test set

See `SYSTEM_ACCEPTANCE_TESTS.csv`. It includes use-case flows and exception cases for all four roles. Run on a clean database and retain screenshots/terminal output, database inspection extracts (with dummy data only), and tester/date details.

## 6. Defect management

Use `DEFECT_LOG.csv`. Severity definitions: Critical = confidentiality/integrity compromise or app cannot start; High = Must requirement or role security broken; Medium = Should requirement or important workflow incorrect; Low = cosmetic/usability issue. A defect is closed only after a repeatable retest passes. Initial inspection observations are marked Open/Not Run until reproduced and retested.

## 7. Test environment and limitations

- Application language: C++17.
- Persistence: SQLite, local/offline.
- Password hash: OpenSSL PBKDF2-HMAC-SHA256.
- Build: CMake.
- Automated framework: Catch2 v3.
- CI: GitHub Actions on Ubuntu latest.
- Coverage: gcovr/gcov; denominator is business-logic source only, as required by the 70% target for business logic.
- Target performance: NFR-01 requires 95% of common login/course/assignment-list operations within 2 seconds with 800 student records on the target school computer. Use `tests/seed_800_students.sql` on a disposable test database to prepare synthetic data; this must be measured on the target machine; CI timing is not a substitute.
- Usability: NFR-07 requires a representative first-time user per role to complete login and a primary task within five minutes; record observation results separately.
