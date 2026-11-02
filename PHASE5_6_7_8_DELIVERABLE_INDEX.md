# CS4433 — Phase 5, 6, 7 and 8 Deliverables

| Phase | Deliverables in this package | Marks |
|---|---|---:|
| 5 Verification and validation | `docs/phase5/TEST_STRATEGY.md`; Catch2 `tests/test_business_rules.cpp`; `SYSTEM_ACCEPTANCE_TESTS.csv`; `DEFECT_LOG.csv`; local build/smoke/coverage evidence (`tests/manual_business_rules_smoke.cpp`); `tests/seed_800_students.sql` | 12 |
| 6 Configuration management and QA | `.github/workflows/ci.yml`; `.clang-tidy`; branch strategy, static analysis and metrics in `docs/phase6/`; self-review record | 6 |
| 7 Deployment | UML deployment source `docs/phase7/deployment_diagram.puml` plus editable DOT source and PNG/SVG render; technician installation guide; role manual; v1.0 release notes | 4 |
| 8 Maintenance | CR1 classification/impact analysis; updated UML sources; SQLite migration; implemented class-wide extension; Catch2 integration tests; v1.1 user-manual addendum and release notes; regression sheet | 6 |

Cross-phase requirement mapping: `docs/TRACEABILITY_MATRIX.md`.

## Remaining student actions before final submission

1. Commit these updated files to GitHub using a feature branch and pull request.
2. Allow GitHub Actions to run; fix any Catch2, build, test, coverage or clang-tidy failure. Save the workflow URL, commit SHA and artifacts.
3. Run the manual system/acceptance tests on the target Windows/school computer and fill actual results/evidence columns.
4. Measure NFR-01 with 800 synthetic students on the target machine; measure NFR-07 with representative users; run and time NFR-09 backup/restore.
5. Confirm CR1 class-vs-individual extension precedence with the supervisor.
6. Tag `v1.0` and `v1.1` only after the corresponding release has been reviewed and accepted.
7. Fill the student name, review date, tester names, commit SHAs, CI run URL and release tag details in the final report.

Do not submit pending checks as completed test evidence. The local test-results file distinguishes the checks run in the development container from those still pending in GitHub and on the target workstation.
