# Self Code Review Record — Phase 6

**Review type:** Preliminary structured review of the Phase 5–8 source snapshot.
**Review date:** 1 November 2026.
**Student reviewer initials/sign-off:** To be completed by the student.
**Final Git commit SHA:** To be copied from the reviewed GitHub commit.

| Review area | Observation / action | Local retest / status |
|---|---|---|
| Requirements traceability | Linked requirement workflow and CR1 paths to test artefacts and source files. | Documentation prepared; final commit SHA mapping pending. |
| Role and assignment scope | Whole-class extension verifies the assigned lecturer owns the assignment. | Wrong lecturer rejected by in-memory SQLite rule test; broader cross-role/system matrix pending. |
| Date validation | Invalid Gregorian dates and invalid time fields must be rejected. | Manual assertion harness passed. |
| Resubmission | A resubmission after the effective deadline must be denied; first attempts follow late policy. | Manual boundary assertions passed; full upload UI path pending. |
| Data integrity | Class-wide grants must commit as one transaction. | Forced trigger failure rolled back all inserted class rows. |
| Individual exception | Class grant must not overwrite an existing individual extension. | Manual database test and console workflow passed. |
| Persistence | Extension records must survive application restart. | Console workflow restarted app and verified student views; passed. |
| Documentation | Updated manual, installation guide, release notes, tests and CR1 analysis. | Artefacts present in phase package; final GitHub upload pending. |

**Review conclusion:** The selected business-rule and CR1 scenarios passed local smoke/integration tests. This is not the final student self-review sign-off. The student must inspect the exact GitHub commit, record their initials and commit SHA, review all remaining system tests, and attach successful CI and target-platform evidence before submission.
