# Phase 5–8 Result Status Summary

**Date:** 1 November 2026.

| Phase | Results obtained | Still outstanding |
|---|---|---|
| 5 Verification and Validation | Production build; DB initialization; schema parity; legacy migration; 8 groups of business-rule assertions; CR1 application workflow; preliminary 84.18% business-rule line execution. | Catch2/CTest and CI report; complete system/acceptance cases; target performance, offline, usability and timed restore. |
| 6 Configuration and QA | Git strategy and workflow definition prepared; GCC build warning review passed; Clang Static Analyzer passed; physical LOC measured. | Push Phase 5–8 files/workflow; GitHub Actions evidence; clang-tidy; Lizard metrics; student review sign-off and exact commit SHA. |
| 7 Deployment | UML diagram, installation guide, four-role manual and v1.0 release notes prepared; local Linux build and DB initialization passed. | Windows target build, school workstation installation, user acceptance, backup/restore and v1.0 release tag. |
| 8 Maintenance | CR1 implemented in package; class and individual deadline behaviour verified across restart; legacy migration, authorization, empty class, extension update and transaction rollback tested. | Full Catch2/CI and regression suite; marking/moderation/audit regression; target Windows tests; v1.1 tag and supervisor policy confirmation. |

**Interpretation:** A PASS in this report applies only to the exact scope and environment stated for that test. Partial or Not Run tests are not evidence of target acceptance. The Catch2 tests exist as source code but were not executed locally because the environment could not resolve GitHub to download Catch2.
