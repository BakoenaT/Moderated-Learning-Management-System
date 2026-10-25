# Phase 4 test plan
Record actual pass/fail, date, tester and evidence after executing each test. Do not claim tests passed before running them.

| ID | Test | Expected result |
|---|---|---|
| T01 | First launch | DB/schema created and default admin created once |
| T02 | Correct/incorrect login | Correct credentials succeed; invalid or inactive account is denied |
| T03 | Duplicate username | SQLite unique constraint rejects duplicate |
| T04 | Create department/course | Records persist after restart |
| T05 | Assign lecturer/enrol student | Valid relationship is persisted |
| T06 | Lecturer attempts unassigned course | Denied |
| T07 | Assignment maximum <= 0 | Rejected |
| T08 | Student course list | Only enrolled courses are listed |
| T09 | Submit file | File copied to controlled submissions folder; metadata stored |
| T10 | Resubmit | New version is stored and previous version remains |
| T11 | Late submission 1/2/3 days | Adjusted maximum is 90%/80%/70% of original maximum |
| T12 | More than 3 days late | Submission is retained; maximum permitted mark is zero |
| T13 | Mark outside range | Rejected |
| T14 | HoD cross-department moderation | Denied |
| T15 | Unreleased mark visibility | Student cannot see it |
| T16 | Release approved mark | Student sees own released mark and feedback |
| T17 | Mark change | Audit entry stores actor, timestamp, old/new mark and reason |
| T18 | Restart | Data persists |
| T19 | Clean CMake build | Build succeeds from clean checkout |
| T20 | 800-student representative dataset | Measure common reads; Phase 2 target is 95% within 2 seconds |

Follow-up before final submission: password-change/reset UI; upload extension/size allow-list; backup/restore command and timed restore test; improved returned-mark correction workflow; automated unit/integration tests; review deadline timestamp semantics and the more-than-three-days rule with supervisor.
