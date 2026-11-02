# Moderated Learning Management System (Moderated LMS)

**CS4433: Software Engineering — National University of Lesotho**  
**Project:** Moderated Learning Management System for a secondary school  
**Implementation:** C++17 console application, SQLite, CMake, OpenSSL  
**Repository owner:** BakoenaT  

This repository contains the project deliverables from Phase 1 through Phase 8. The concise five-page final report is the main report; the phase reports, diagrams, test logs, manuals, SQL scripts and other supporting files are linked separately below so that the report remains readable and supporting artefacts remain available as independent files.

## Start here

- [Five-page Final Project Report (DOCX)](docs/reports/Final_Project_Report.docx)
- [Five-page Final Project Report (PDF)](docs/reports/Final_Project_Report.pdf)
- [Complete project contents by phase](PROJECT_CONTENTS.md)
- [Phase 5–8 deliverables index](PHASE5_6_7_8_DELIVERABLE_INDEX.md)
- [Phase 5–8 detailed technical report (DOCX)](docs/reports/Phases5_6_7_8_Detailed_Report.docx) and [PDF](docs/reports/Phases5_6_7_8_Detailed_Report.pdf)
- [Assignment brief](docs/assignment-reference/CS4433_Final_Project_Individual_Assignment.pdf)
- [Demonstration video folder](recordings/README.md) and [recording script](recordings/VIDEO_SCRIPT.md) — upload the final MP4 after recording; no recording has been supplied yet.
- [GitHub upload instructions](GITHUB_UPLOAD_INSTRUCTIONS.md).

## Project phases and deliverables

### Phase 1 — Project planning and management

- [Project Management Plan](docs/phases/phase1/Phase1_Project_Management_Plan.docx)
- [Seven-week Gantt Chart](docs/phases/phase1/Phase1_Gantt_Chart.xlsx)

### Phase 2 — Requirements engineering

- [Requirements Engineering Report / SRS](docs/phases/phase2/Phase2_Requirements_Engineering.docx)
- [Use Case Diagram — editable draw.io source](docs/diagrams/phase2/Use_Case_Diagram.drawio)
- [Use Case Diagram — PNG preview](docs/diagrams/phase2/Use_Case_Diagram.png)

### Phase 3 — System design

- [Revised System Design Report (DOCX)](docs/phases/phase3/Phase3_System_Design_Revised.docx)
- [Revised System Design Report (PDF)](docs/phases/phase3/Phase3_System_Design_Revised.pdf)
- [Corrected MySQL 8 schema (design artefact)](docs/phases/phase3/07_Moderated_LMS_MySQL_Schema.sql)
- [Logical LMS ERD preview](docs/diagrams/phase3/07_Moderated_LMS_ERD.png)
- Editable and preview diagrams are in [`docs/diagrams/phase3/`](docs/diagrams/phase3/).

**Database distinction:** MySQL is retained as the Phase 3 relational-design/ERD artefact. The Phase 4–8 application uses SQLite at runtime, as required by the assignment. These are intentionally separate schemas for separate purposes.

**Workbench evidence caution:** the supplied MySQL Workbench screenshot in `docs/evidence/phase3/` displays a timetable database, not the Moderated LMS. It is retained unchanged as environment evidence only, not labelled as the LMS ERD. Generate a genuine Workbench EER model from the corrected MySQL DDL if required by the supervisor.

### Phase 4 — Implementation

- [Current C++17 application source](src/main.cpp)
- [Business-rule module](src/business_rules.cpp) and [header](src/business_rules.hpp)
- [SQLite runtime schema](database/schema.sql)
- [CMake build configuration](CMakeLists.txt)
- [Implementation notes](IMPLEMENTATION_NOTES.md)
- The original Phase 4 baseline source snapshot is preserved under [`releases/v1.0/source/`](releases/v1.0/source/).

### Phase 5 — Verification and validation

- [V-Model test strategy](docs/phase5/TEST_STRATEGY.md)
- [Catch2 automated tests](tests/test_business_rules.cpp)
- [Manual business-rule smoke harness](tests/manual_business_rules_smoke.cpp)
- [System / acceptance cases](docs/phase5/SYSTEM_ACCEPTANCE_TESTS.csv)
- [Executed test results](docs/phase5/TEST_EXECUTION_RESULTS.md)
- [Local build, migration, schema-parity and coverage evidence](docs/phase5/LOCAL_TEST_AND_COVERAGE_RESULTS.md)
- [Defect log](docs/phase5/DEFECT_LOG.csv)
- [800-student seed data](tests/seed_800_students.sql)

### Phase 6 — Configuration management and quality assurance

- [GitHub Actions CI workflow](.github/workflows/ci.yml)
- [Branching, commits and release process](docs/phase6/CONFIGURATION_AND_QA.md)
- [Static analysis and code metrics](docs/phase6/METRICS_AND_STATIC_ANALYSIS.md)
- [Static analysis result files](docs/phase6/)
- [Self code-review record](docs/phase6/CODE_REVIEW_RECORD.md)

### Phase 7 — Deployment

- [Deployment diagram PNG](docs/phase7/deployment_diagram.png)
- Editable diagram sources: [PlantUML](docs/phase7/deployment_diagram.puml), [DOT](docs/phase7/deployment_diagram.dot), and [SVG](docs/phase7/deployment_diagram.svg)
- [Technician installation guide](docs/phase7/INSTALLATION_GUIDE.md)
- [Four-role user manual](docs/phase7/USER_MANUAL.md)
- [v1.0 release notes](docs/phase7/RELEASE_NOTES_v1.0.md)
- [Deployment verification](docs/phase7/DEPLOYMENT_VERIFICATION.md)

### Phase 8 — Maintenance (CR1)

- [CR1 change request and impact analysis](docs/phase8/CR1_CHANGE_ANALYSIS.md)
- [Updated design](docs/phase8/CR1_UPDATED_DESIGN.md)
- [CR1 regression cases](docs/phase8/REGRESSION_TESTS.csv)
- [Maintenance verification and actual outcomes](docs/phase8/MAINTENANCE_VERIFICATION.md)
- [v1.1 release notes](docs/phase8/RELEASE_NOTES_v1.1.md)
- [v1.1 user manual addendum](docs/phase8/USER_MANUAL_ADDENDUM_v1.1.md)
- [Updated CR1 activity, sequence and use-case diagrams](docs/phase8/)

## Build and run

### Dependencies

- C++17 compiler (MSVC 2022, GCC or Clang)
- CMake 3.20 or newer
- SQLite development library
- OpenSSL development library
- Git (for version control)

### Windows (Visual Studio 2022 Build Tools)

Open **Developer PowerShell for VS 2022** from the Windows Start menu, change directory to the repository root, and run:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DBUILD_TESTING=OFF
cmake --build build --config Release
```

This builds the application without downloading Catch2. Make sure SQLite and OpenSSL development packages are installed and discoverable by CMake (use `CMAKE_PREFIX_PATH` if required). To build automated tests, set `BUILD_TESTING=ON`; CMake FetchContent downloads Catch2 v3.7.1 and therefore requires network access during configuration unless Catch2 is already available locally.

Run the executable from the build output directory. The program creates `moderated_lms.db` in its working directory. First-run credentials are `admin` / `ChangeMe123!`; change the default password before any real use. Use synthetic data for demonstrations.

### Linux

Install a C++17 compiler, CMake, SQLite and OpenSSL development packages, then run:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
cmake --build build --parallel
./build/moderated_lms
```

To run Catch2 tests in a network-enabled environment:

```bash
cmake -S . -B build-tests -DBUILD_TESTING=ON -DENABLE_COVERAGE=ON
cmake --build build-tests --parallel
ctest --test-dir build-tests --output-on-failure
```

## Evidence status — read before submission

The recorded 1 November 2026 Linux development-container evidence covers a production build, database initialization, ten-table schema parity, legacy `extension_scope` migration, business-rule smoke assertions and a CR1 console workflow across an application restart. The standalone smoke harness reported 84.18% line execution and 78.91% branches executed for `src/business_rules.cpp`. This is **preliminary manual-harness coverage**, not the final Catch2/CI coverage result.

Catch2/CTest and GitHub Actions evidence remain pending because Catch2 could not be fetched in the earlier development environment. Windows target-machine acceptance, 800-student performance, offline workflow, representative-user usability, and timed backup/restore tests also remain pending. Do not report those as passed until executed and captured.

The v1.1 CR1 code is present in the root source tree, but v1.1 is a release candidate, not yet a tagged/accepted release. The supervisor should confirm how class extensions interact with individual extensions and whether students enrolled after a class grant inherit it.

## Demonstration recording and privacy

Place the student-recorded MP4 in `recordings/` or add a stable course-approved view-only link there. Use synthetic accounts/data. Do not publish real learner records, passwords, student IDs, marks, email addresses or identifiable audio/video without permission. For videos larger than GitHub's standard browser-upload limit, use Git LFS or a course-approved video platform.

## Folder map

- `src/`, `database/`, `tests/`, `.github/` — working implementation, runtime schema, automated/manual tests and CI.
- `docs/phases/` — Phase 1–4 reports and design submissions.
- `docs/diagrams/` — editable draw.io sources and exported previews.
- `docs/phase5/`–`docs/phase8/` — test evidence, QA, deployment and maintenance artefacts.
- `docs/reports/` — concise final report and detailed supplementary report.
- `docs/evidence/` — supporting tool evidence and notes.
- `releases/` — archived v1.0 baseline and v1.1 candidate notes.
- `recordings/` — final demonstration video placeholder.
