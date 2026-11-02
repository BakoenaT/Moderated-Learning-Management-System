# Phase 6 — Code Metrics and Static Analysis

**Measurement date:** 1 November 2026. **Source snapshot:** the Phase 5–8 package in this deliverable.

## Physical source lines

| File | Physical lines |
|---|---:|
| `src/main.cpp` | 171 |
| `src/business_rules.cpp` | 228 |
| `src/business_rules.hpp` | 32 |
| `tests/test_business_rules.cpp` | 173 |
| `tests/manual_business_rules_smoke.cpp` | 39 |
| **Total** | **643** |

Physical line counts were measured from the files in this package. The compact formatting of portions of `src/main.cpp` means physical lines understate logical statement count.

## Cyclomatic complexity

The values below are manual structural estimates, not Lizard measurements. Lizard was not installed in the local execution environment; do not present these estimates as automated-tool output.

| Function | Estimated complexity | Qualification |
|---|---:|---|
| `calendarDaysLate` | 1 | Strict date validation is delegated to helper functions. |
| `lateAdjustedMaximum` | 4 | Input guards and late-day cap. |
| `submissionTimeAllowed` | 2 | Distinguishes first attempt from resubmission deadline. |
| `grantWholeClassExtension` | 13 | Assignment validation, roster loop, conditional upsert and transaction error handling. |

## Static analysis and build diagnostics

- CMake 3.31.6 / GCC 14.2.0 production build completed with `-Wall -Wextra -Wpedantic`; no compiler warning appeared in captured output.
- Clang 17 Static Analyzer completed on `src/main.cpp` and `src/business_rules.cpp` with no diagnostic output.
- `clang-tidy` was not installed and remains pending.
- Lizard was not installed; complexity estimates above are explicitly manual.
- Catch2 and GitHub Actions jobs have not yet run; the CMake FetchContent attempt could not resolve `github.com` in this execution environment.
- Preliminary gcov smoke coverage for `src/business_rules.cpp`: 84.18% of 158 executable lines; 78.91% of 294 branches executed, with 49.32% taken at least once. The Catch2/CI report is still required.

## Self-review actions recorded

1. Extracted calendar-date, late-penalty, resubmission and class-extension rules into `src/business_rules.cpp/.hpp` for independent testing.
2. Added strict Gregorian date/time validation so invalid dates no longer become zero late days.
3. Added an effective-deadline resubmission check.
4. Added the `extension_scope` column and an additive migration for existing SQLite files.
5. Added transactional class-wide extension updates while preserving individual overrides.
6. Updated student assignment-list output to show the effective deadline and extension label.
7. Added manual business-rule assertions and Catch2 test source for boundaries, authorization, exceptions and rollback.

**Outstanding reviewer record:** The student must enter their own reviewer name/initials, review date and final repository commit SHA after reviewing the exact commit submitted. CI run URL and clang-tidy/Lizard outputs must be attached when available.
