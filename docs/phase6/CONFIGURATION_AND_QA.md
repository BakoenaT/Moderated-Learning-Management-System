# Phase 6 — Configuration Management and Quality Assurance

## Repository baseline
Repository: https://github.com/BakoenaT/Moderated-Learning-Management-System
Default branch: `main`. The live repository link is supplied, but the Phase 5–8 package and CI workflow must be committed to the repository before CI evidence can be claimed. Record the final branch, commit SHA, PR URL and tag after upload.

## Branching strategy
Use GitHub Flow: keep `main` as reviewed baseline; create focused feature/maintenance branches; use small imperative commits; open pull requests; merge only after review and successful CI. Proposed branches: `feature/phase5-tests`, `feature/phase6-ci`, `feature/phase7-deployment-docs`, and `maintenance/cr1-class-extension`.

## CI pipeline
`.github/workflows/ci.yml` is prepared to build C++17 with CMake, execute Catch2/CTest, generate gcovr business-logic line coverage and enforce a 70% threshold, upload test/coverage evidence and run clang-tidy. The workflow has not yet been confirmed committed and executed on the live GitHub repository. Therefore **no GitHub Actions pass is claimed**.

## Results recorded locally
- GCC 14.2.0 production build: PASS; no compiler warnings in captured output.
- Clang 17 Static Analyzer on `main.cpp` and `business_rules.cpp`: PASS; no diagnostics.
- `clang-tidy`: PENDING (not installed locally).
- Catch2/CTest: BLOCKED locally because CMake FetchContent could not resolve github.com.
- Lizard complexity: PENDING (not installed); manual estimates are labelled as such in `METRICS_AND_STATIC_ANALYSIS.md`.

## Release/configuration status
The branching strategy and commands are documented. Tags `v1.0` and `v1.1` are proposed but must not be created until review, the applicable CI run, acceptance tests and regression tests pass. Attach GitHub release/tag links and workflow evidence to the final submission.
