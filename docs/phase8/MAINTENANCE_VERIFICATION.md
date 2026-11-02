# Phase 8 — CR1 Maintenance and Regression Verification

## Change request
CR1 asks that an assigned Lecturer can extend the deadline for a whole class and that each Student sees the effective revised deadline on their assignment list. The change is classified as **perfective maintenance / functional enhancement**, because it extends the existing extension workflow without replacing the system architecture.

## Implementation verification
- Added `extension_scope` (`INDIVIDUAL` or `CLASS`) to `extension_grant`.
- Existing databases receive an additive migration; prior rows default to `INDIVIDUAL`.
- `grantWholeClassExtension` validates lecturer ownership, date/reason and class roster, and writes in one SQLite transaction.
- Existing individual extensions remain exceptions; class grants apply to other currently enrolled students.
- Later class deadlines may extend existing class rows, while a shorter class deadline is rejected.
- Student assignment list displays the effective deadline and source label.

## Actual test outcomes (1 November 2026, Linux development container)

| Regression ID | Actual result | Status |
|---|---|---|
| REG-01 | Whole-class extension applied to all eligible currently enrolled students in a two-student console workflow and a three-student rule fixture. | PASS |
| REG-02 | Existing individual exception was preserved when class extension was granted. | PASS |
| REG-03 | Lecturer who did not own the assignment was rejected by the business-rule integration function; no extension row written. | PASS (service scope) |
| REG-04 | Empty class was rejected with no extension change. | PASS (service scope) |
| REG-05 | Student assignment list displayed revised deadline and Class extension label after application restart. | PASS |
| REG-06 | Later class extension updated CLASS rows; shorter extension was rejected; INDIVIDUAL row was unchanged. | PASS (business-rule harness) |
| REG-07 | Individual extension granted after class extension became Student A's effective deadline and label. | PASS |
| REG-08 | Late-day and penalty boundary rules passed in business-rule harness; full persisted file submission sequence remains pending. | PARTIAL |
| REG-09 | Resubmission deadline predicate passed exact/before/after checks; full console resubmission path remains pending. | PARTIAL |
| REG-10 | Marking, moderation, release and audit regression were not executed in this CR1 run. | NOT RUN |
| REG-11 | Broad cross-role/cross-department regression remains pending; only class-extension lecturer ownership was exercised. | NOT RUN / PARTIAL |
| REG-12 | Extension rows survived process restart and were displayed correctly to both students. | PASS |
| REG-13 | Offline workflow was not explicitly executed on the target machine. | NOT RUN |
| REG-14 | Forced SQLite trigger failure caused rollback with no partial class-extension rows. | PASS |

## Release status
The code change, updated UML sources, database migration, manual/Catch2 test code, user-manual addendum and v1.1 release notes are prepared. **v1.1 is not yet a released/tagged version.** Create the tag only after Catch2/CI, complete regression, target workstation acceptance and supervisor approval. Confirm the policy for students enrolled after a class extension and the precedence of overlapping class/individual extensions.
