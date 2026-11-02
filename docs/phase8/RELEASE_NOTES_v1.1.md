# Moderated LMS v1.1 — CR1 Maintenance Release

**Release type:** Perfective maintenance / enhancement.  
**Tag:** `v1.1` (create only after PR review, CI pass and regression acceptance).

## Added
- Lecturer can grant a revised deadline to the whole class associated with an assignment.
- Class extension is applied to currently enrolled students in one database transaction.
- The student assignment list displays the effective deadline and whether the extension is class-wide or individual.
- `extension_scope` distinguishes individual and class grants.
- Existing individual extensions are preserved as student-specific exceptions during a class grant.
- Date/time parsing and late-day calculations are strict and timezone/DST-independent.
- A resubmission is rejected when made after the effective deadline; first attempts remain subject to the accepted late policy.

## Database migration
On startup, the application checks `extension_grant` for `extension_scope`. Existing database files receive an additive column with default `INDIVIDUAL`; new databases create the column directly. Back up `moderated_lms.db` and `submissions/` before upgrading.

## Regression focus
Run all Catch2 tests and all cases in `docs/phase8/REGRESSION_TESTS.csv`. Record actual outcomes and attach CI coverage/test reports. Do not tag v1.1 if any Critical/High defect remains open or the supervisor has not accepted the overlapping-extension policy.
