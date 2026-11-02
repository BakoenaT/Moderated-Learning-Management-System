# Phase 7 — Deployment Verification Record

**Deployment baseline described:** Moderated LMS v1.0, offline C++17 console application, local SQLite database, controlled `submissions/` folder and OpenSSL password hashing.

| Deployment item | Evidence status | Verification note |
|---|---|---|
| UML deployment diagram | Prepared and visually reviewed | `deployment_diagram.puml`, `.dot`, `.svg` and `.png` are included. Diagram shows a single offline school workstation and controlled backup location. |
| Technician installation guide | Prepared | Windows/MSVC + vcpkg and Ubuntu/Debian build steps are documented. |
| Four role user manuals | Prepared | Administrator, Lecturer, HoD and Student workflows are documented. |
| v1.0 release notes | Prepared | Release notes are included; a Git tag/release has not yet been created. |
| Linux development build | PASS | CMake/GCC production build completed and application DB initialized in the development container. |
| SQLite first-run initialization | PASS | Ten tables initialized; schema parity and extension_scope migration tested. |
| Windows target installation | NOT RUN | Student's Windows environment still needs MSVC, SQLite/OpenSSL dependency setup and a clean build. |
| School workstation deployment / user acceptance | NOT RUN | No live school installation or representative-user sign-off is claimed. |
| Backup/restore operation at target | NOT RUN | Technician must run and time restore; requirement target is 15 minutes. |

**Release gate:** Create `v1.0` only after target-machine build, security setup (including replacement of default bootstrap password), system/acceptance tests, backup/restore verification and supervisor review. The prepared release notes are not evidence that deployment has occurred.
