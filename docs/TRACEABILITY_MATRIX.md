# Moderated LMS Traceability Matrix — Phase 2 to Phase 8

| Requirement | Use case / workflow | Design component / data | Source file | Verification |
|---|---|---|---|---|
| FR-01 Authentication | UC Login | User hierarchy; `user_account.password_hash` | `src/main.cpp` | T02, SAT-01/02 |
| FR-02 Role access | Role-specific menu workflows | `User`, `Administrator`, `Lecturer`, `HoD`, `Student` | `src/main.cpp` | SAT-03/04/05; REG-11 |
| FR-03 User administration | Manage user accounts | `user_account` | `src/main.cpp` | SAT-06 |
| FR-04 Academic structure | Manage departments/courses | `department`, `course` | `src/main.cpp`, `database/schema.sql` | SAT-07 |
| FR-05 Lecturer/course assignment and student enrolment | Assign lecturer / enrol student | `lecturer_course`, `enrolment` | `src/main.cpp`, `database/schema.sql` | SAT-07 |
| FR-06 View enrolled course assignments | Student views course list | `enrolment`, `course`, `assignment` | `src/main.cpp` | SAT-03; SAT-19 |
| FR-07 Create assignment | Lecturer creates assignment | `assignment`, course-lecturer relation | `src/main.cpp` | SAT-07 |
| FR-08 Grant extensions | Individual and class extension | `Extension` behaviour in `extension_grant`; `extension_scope` | `src/main.cpp`, `src/business_rules.cpp` | SAT-08; SAT-19; REG-01–07 |
| FR-09 Submit assignment | Student submits file | `submission`, controlled `submissions/` directory | `src/main.cpp` | SAT-09; SAT-18 |
| FR-10 Resubmit before effective deadline | Student resubmits | `submission.version_no`; effective deadline | `src/main.cpp`, `src/business_rules.cpp` | SAT-10; REG-09 |
| FR-11 Late penalty | Mark submission | Penalty policy function | `src/business_rules.cpp` | Unit late-boundary tests; SAT-09 |
| FR-12 Mark/feedback | Lecturer marks | `mark_record` | `src/main.cpp` | SAT-11; SAT-14 |
| FR-13 Moderate marks | HoD approves/returns | `mark_record.moderation` | `src/main.cpp` | SAT-12; SAT-13 |
| FR-14 Release marks | HoD releases approved marks | `mark_record.released_by`, `submission.status` | `src/main.cpp` | SAT-13; SAT-14 |
| FR-15 Audit mark changes | Audit mark event | `audit_log` | `src/main.cpp` | SAT-14; SAT-18 |
| FR-16 Student views released results | Student result list | `mark_record`, `submission`, `user_account` | `src/main.cpp` | SAT-03; SAT-13 |
| FR-17 Submission states (Should) | Submission workflow | `submission.status`; moderation status | `src/main.cpp` | SAT-09–13 |
| FR-18 Staff queues (Should) | Lecturer/HoD queues | course and department-scoped queries | `src/main.cpp` | SAT-04/05; SAT-12 |
| NFR-01 Performance | 800-student read test | Indexed SQLite queries | `database/schema.sql` | SAT-16 (target-hardware measurement) |
| NFR-02 Offline | Core workflows | Local executable, SQLite and file store | `src/main.cpp` | SAT-15; REG-13 |
| NFR-03 Password protection | Login | PBKDF2-HMAC-SHA256 with random salt | `src/main.cpp` | T02; static database inspection |
| NFR-04 Access control | All protected use cases | Application service checks | `src/main.cpp`, `src/business_rules.cpp` | SAT-03/04/05; REG-03/11 |
| NFR-05 Integrity | Database transactions/validation | CHECK/FK/UNIQUE and transactions | `src/main.cpp`, `database/schema.sql`, `src/business_rules.cpp` | SAT-11/18; CR1 integration tests |
| NFR-06 Auditability | Mark changes/moderation/release | `audit_log` | `src/main.cpp` | SAT-14 |
| NFR-07 Usability | Role primary-task walkthrough | Console role guides | `docs/phase7/USER_MANUAL.md` | Observed acceptance task under five minutes |
| NFR-08 Compatibility | Clean build | CMake/C++17 | `CMakeLists.txt`, `.github/workflows/ci.yml` | CI build; SAT-19 is the CR1 acceptance test; NFR-08 clean target build must be recorded separately |
| NFR-09 Recoverability | Backup/restore | Database + submission backup | `docs/phase7/INSTALLATION_GUIDE.md` | SAT-17 |

**Traceability note:** The class-wide extension is an extension of FR-08, not a replacement for individual extensions. Exact UC identifiers should be reconciled with the final Phase 2 use-case diagram/specification if the supervisor requires the diagram's numbering to be copied verbatim.
