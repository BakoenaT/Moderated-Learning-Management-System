# Moderated LMS — Phase 4 (C++17)

Offline console implementation for the CS4433 Moderated Learning Management System.

## Dependencies
- C++17 compiler (GCC/Clang or Visual Studio)
- CMake 3.16+
- SQLite development library
- OpenSSL development library (PBKDF2-HMAC-SHA256 password hashing)

## Build on Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake libsqlite3-dev libssl-dev
cmake -S . -B build
cmake --build build
./build/moderated_lms
```

## Build on Windows
Install Visual Studio C++ build tools, CMake, SQLite development files, and OpenSSL. Open the folder in Visual Studio/CMake or run `cmake -S . -B build` and `cmake --build build --config Release`. If packages are not detected, configure `CMAKE_PREFIX_PATH` to their installation directories.

## First login
The first run creates `moderated_lms.db` and an initial administrator:
- username: `admin`
- password: `ChangeMe123!`

Change the default password before using real data. The password is stored as a salted PBKDF2-HMAC-SHA256 hash. Database and submission files are local; the app works offline.

## Included workflows
- Administrator: list/create/deactivate accounts; create departments/courses; assign lecturers; enrol students.
- Lecturer: view assigned courses; create assignments; grant individual extensions; view submissions; mark submissions with feedback.
- Head of Department: view marks for own department; approve/return marks; release approved marks.
- Student: view enrolled courses and assignments; submit/resubmit a file; view only released marks and feedback.

## Design and security
- C++17 abstract `User` class with inheritance and polymorphism for Administrator, Lecturer, HoD and Student.
- SQLite persistence with prepared statements and foreign keys; database access is encapsulated in `Db`.
- PBKDF2-HMAC-SHA256 with a random salt via OpenSSL; no plaintext password storage.
- Mark updates and audit events are written in a transaction.
- Strategy-style late-penalty calculation is isolated in `penalty()`.
- Role, course, department and enrolment checks are enforced by application workflows.

## Policy assumptions and limitations
- The Phase 2 calendar-day assumption is used for lateness. Up to 3 late days are accepted with a 10% of maximum-mark deduction per day; more than 3 days may be submitted but the allowed mark is zero. Confirm the exact policy with the supervisor.
- Submissions are copied into a controlled `submissions/` directory; only the generated path is stored in SQLite.
- This is a Phase 4 implementation baseline. Execute the test plan and record actual evidence; do not claim tests passed unless run. Backup/restore automation, password-change/reset UI, upload allow-list/size limit, and a complete returned-mark correction loop are follow-up items.
- The assignment specifies SQLite for implementation. The MySQL schema from Phase 3 remains a separate design artefact.
