# Phase 4 — Implementation summary

The Moderated LMS is implemented as a C++17 offline console application. Runtime persistence uses SQLite, with database access encapsulated in the `Db` data-access wrapper and user-provided values passed through prepared statements. Passwords are represented by PBKDF2-HMAC-SHA256 hashes generated with OpenSSL and a per-password random salt.

The abstract `User` base class has four role-specific subclasses: `Administrator`, `Lecturer`, `HoD`, and `Student`. Role-specific workflows enforce account role and scope checks in the application. The administrator maintains departments, courses, users, lecturer-course assignments and student enrolments. Lecturers create assignments, grant individual or class-wide extensions, inspect submissions and mark work. HoDs moderate departmental results and release approved marks. Students view their enrolments and assignments, submit/resubmit work and view only released feedback/results.

The SQLite schema is in `database/schema.sql`. The application also creates/migrates the required tables at startup. Marking, moderation, release and audit changes are grouped in database transactions. The original Phase 4 baseline is preserved under `releases/v1.0/source/`; the root implementation includes the CR1 class-wide extension maintenance update for the v1.1 candidate.

The MySQL schema in `docs/phases/phase3/` is a separate Phase 3 design artefact and is not used by the Phase 4–8 runtime.
