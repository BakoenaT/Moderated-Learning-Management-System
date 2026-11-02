# Upload this complete project package to GitHub

This package is structured for the existing repository:

`https://github.com/BakoenaT/Moderated-Learning-Management-System`

## Recommended safe upload

1. Download `ModeratedLMS_Complete_GitHub_Repository_GitHub_Upload.zip` and extract it on your computer.
2. Open the extracted repository folder. The root should contain `README.md`, `CMakeLists.txt`, `.github/`, `src/`, `database/`, `tests/`, `docs/`, `releases/` and `recordings/`.
3. In GitHub, create a branch such as `feature/complete-project-documentation` from `main`.
4. Upload the **contents of the extracted folder**, not the ZIP itself and not the enclosing directory as one nested folder. Preserve the folder structure.
5. Existing files such as `README.md`, `CMakeLists.txt`, `src/main.cpp` and `database/schema.sql` are updated to the consolidated versions in this package. Review the diff before committing.
6. Use the commit message `Add complete CS4433 phases 1-8 project artefacts`.
7. Open a pull request to `main`, review the changed files, and inspect the GitHub Actions workflow result.

## Before committing

- Do not upload a generated `moderated_lms.db`, real learner data or the `submissions/` folder.
- Do not upload build output (`build/`, object files or executables); `.gitignore` excludes these.
- The demonstration video is not present yet. Record it with synthetic accounts and add `recordings/Moderated_LMS_Demonstration.mp4` (or a course-approved view-only link) separately.
- `docs/evidence/phase3/` identifies the old Workbench screenshot as an unrelated timetable schema; it is not the LMS ERD.
- The Phase 3 MySQL schema is a design artefact. The C++ Phase 4–8 application uses SQLite at runtime.
- Catch2/CTest, successful GitHub Actions, Windows acceptance and final v1.0/v1.1 tags are still to be evidenced. Do not describe pending checks as passed.

## Report and linked files

The main report is `docs/reports/Final_Project_Report.docx` and its PDF. It links to supporting files using GitHub paths. Those links resolve once this directory structure has been committed to the repository's `main` branch. Keep diagrams and images as separate files; do not paste them into the short report unless the lecturer specifically requests it.
