# Phase 8 — Maintenance: Change Request CR1

## 1. Change request

**CR1:** “A lecturer should be able to extend the deadline for a whole class, not just an individual student, and extensions should be shown on the student's assignment list.”

## 2. Classification

CR1 is **perfective (enhancement) maintenance**. It adds a new capability and improves the usefulness/visibility of an existing extension workflow; it is not primarily corrective because the individual-extension feature was already operating as designed. The change is backward-compatible with individual extensions.

## 3. Policy decision for overlapping extensions

The assignment brief does not prescribe how a whole-class extension interacts with an existing individual extension. The implemented rule is: a class grant is recorded for each currently enrolled student who has no individual extension or whose existing extension is already class-scoped. A pre-existing individual extension is preserved as a student-specific exception. The lecturer can subsequently replace that student's individual extension using the existing individual-extension function. This policy prevents a general class grant from unintentionally shortening or removing a separately approved individual arrangement.

A class extension applies to students enrolled in the course at the time it is granted. Students enrolled later do not automatically inherit a historical grant; the lecturer can issue a new class grant if required. Confirm these two policy details with the supervisor/stakeholders before final acceptance.

## 4. Impact analysis

| Artefact | Before CR1 | Required change |
|---|---|---|
| SRS FR-08 | Lecturer grants an individual student's extension | Extend FR-08 to include individual and whole-class operations; clarify that the revised deadline appears in the student assignment list |
| BR-02 / lecturer scope | Lecturer acts only on assigned courses | Retain; class grant must verify the assignment belongs to the acting lecturer |
| BR-06 / deadline | Individual extension replaces the original deadline for a student | A class extension provides an effective deadline for currently enrolled students without an individual override; individual extension remains an exception |
| Use case model | Grant individual extension | Add/extend use case “Grant class extension” with course roster selection implicit from assignment course |
| Fully dressed use case | Individual extension flow | Add actor, preconditions, transaction, main/exception flows and postconditions for whole class |
| Behavioural/activity model | One student receives an extension | Add loop/bulk operation over enrolments and success/exception branches |
| Class design | ExtensionService/extension grant workflow | Add `grantWholeClassExtension`; keep policy decision separate and testable |
| SQLite `extension_grant` table | One current row per assignment/student | Add `extension_scope` (`INDIVIDUAL`/`CLASS`); preserve unique assignment/student key; migration adds the field to existing databases |
| Student assignment query | Displays original assignment deadline | Join the student's extension row; show effective deadline and label `Class extension` or `Individual extension` |
| `src/main.cpp` | Individual extension menu only | Add lecturer whole-class extension option, call validated transaction helper, show updated student count |
| `src/business_rules.cpp/.hpp` | No isolated CR1 operation | Add authorised bulk grant, strict deadline validation and extension-label helper |
| Tests | Individual extension only | Add tests for class roster coverage, lecturer authorisation, individual override preservation, empty class and displayed extension source |
| Deployment/user manual | v1.0 only | Document v1.1 class extension workflow and release notes |

## 5. Data design and transaction behaviour

The existing `extension_grant` table already stores one current extension per assignment/student. CR1 reuses it rather than introducing a second competing deadline table. A new `extension_scope` column identifies `INDIVIDUAL` or `CLASS`. The existing unique key `(assignment_id, student_id)` remains. A bulk operation reads the enrolled students, validates that the assignment is owned by the acting lecturer, and applies all eligible rows in one `BEGIN IMMEDIATE` transaction. On any database error, the operation rolls back all class-extension updates.

For an enrolled student, the effective deadline is `extension_grant.deadline` when a grant exists; otherwise it is `assignment.deadline`. The student assignment-list output includes the effective date and extension type. No mark, submission, moderation or release is changed solely by granting an extension.

## 6. Implementation record

Code changes are in:
- `src/business_rules.hpp` and `src/business_rules.cpp` — class-wide extension transaction, strict ISO date/time validation and shared late-rule functions.
- `src/main.cpp` — migration for `extension_scope`, lecturer menu action, individual scope writes, student assignment-list effective deadline and resubmission boundary enforcement.
- `database/schema.sql` — new `extension_scope` column for clean database creation.
- `tests/test_business_rules.cpp` — Catch2 unit/integration tests.

The helper deliberately leaves pre-existing `INDIVIDUAL` rows untouched, treating them as a more specific exception. Existing databases receive an additive `ALTER TABLE` migration at startup if the column is missing; new databases create it directly.

## 7. Release and regression

The change is the basis of **v1.1**. Do not tag `v1.1` until the CI build/test/coverage workflow passes, system regression tests have run, and the school has accepted the policy for overlapping extensions. See `RELEASE_NOTES_v1.1.md` and `REGRESSION_TESTS.csv`.
