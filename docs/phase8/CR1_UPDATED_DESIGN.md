# CR1 Updated Design Artefacts

CR1 extends the existing extension workflow. Updated UML sources are:

- `cr1_use_case_update.puml` — adds Grant whole-class extension and exposes effective deadline in the student's assignment-list use case.
- `cr1_activity.puml` — shows ownership validation, roster iteration, individual-exception handling and transaction success/failure.
- `cr1_sequence.puml` — shows the lecturer-to-business-rule-to-SQLite interaction and the student query for effective deadline.

## Updated class responsibilities

- `App::lecturerMenu` presents the whole-class action and sends input to the business-rule operation.
- `lms::grantWholeClassExtension` validates assignment ownership and deadline, reads current enrolments, and applies all eligible extension writes in one SQLite transaction.
- `lms::calendarDaysLate`, `lms::lateAdjustedMaximum` and `lms::submissionTimeAllowed` are shared rules used by the application and Catch2 tests.
- The `extension_grant` data record retains the original assignment/student key and adds `extension_scope` (`INDIVIDUAL` or `CLASS`).
- The student's assignment list joins the extension record and displays `effective_deadline` and `extension_type`.

## Updated data dictionary entry

| Table | Attribute | Type | Null | Key / rule | Purpose |
|---|---|---|---|---|---|
| `extension_grant` | `extension_scope` | `TEXT` | No | Default `INDIVIDUAL`; CHECK in `INDIVIDUAL`, `CLASS` | Records whether the current effective extension is a student-specific grant or a whole-class grant. |

## Migration behaviour

For a clean database, `database/schema.sql` creates `extension_scope` with default `INDIVIDUAL`. At application startup, the `Db` constructor checks `PRAGMA table_info(extension_grant)` and adds the column to a pre-CR1 database if it is absent. The migration is additive: old individual extension rows receive the `INDIVIDUAL` default. Back up the database before deploying v1.1.

## Effective deadline rule

`extension_grant.deadline` is the student's current effective deadline when a row exists; otherwise `assignment.deadline` applies. An individual row is a specific exception and is not overwritten by a class grant. Existing class grants may be moved later by another class grant but not shortened. The lecturer's assignment ownership is revalidated by the business rule before any database write.
