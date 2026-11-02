# Moderated LMS v1.0 — Initial Deployment Release Notes

**Release type:** Initial baseline release (Phase 7).  
**Tag:** `v1.0` (create on the accepted commit; record the actual tag/commit SHA in the report).

## Included
- Offline C++17 console application with four roles: Administrator, Lecturer, HoD and Student.
- Local SQLite persistence through the `Db` data-access component.
- Salted PBKDF2-HMAC-SHA256 password hashes using OpenSSL.
- Department/course management, lecturer assignment and student enrolment.
- Assignment creation, individual extensions, file submission and resubmission history.
- Late penalty, lecturer marking/feedback, HoD moderation and explicit release to students.
- Audit records for mark creation/changes, moderation and release.

## Known limitations at v1.0
- No browser/mobile interface, online payment, video conferencing, automated grading or plagiarism detection (out of scope).
- No password-change/reset menu; the seeded default admin credential must be replaced through a secure provisioning improvement before real learner data is used.
- Console password input is visible while typed; use only in a controlled environment until hidden password input is implemented.
- Upload extension/size allow-list is not yet enforced.
- Backup/restore is a documented technician process and requires a timed restore exercise.
- The current deployment is intended for one designated local workstation; do not place the SQLite database on a shared network filesystem.

## Upgrade and data
Back up `moderated_lms.db` and `submissions/` together before replacing the executable. Keep the database and submitted-file directory from the same backup set. Confirm the application starts and verify sample account/course/submission records after the upgrade.
