# Technician Installation Guide — Moderated LMS v1.0

## 1. Scope and deployment assumptions

The system is a C++17 offline console application using a local SQLite database and a controlled local submission folder. The deployment model in `deployment_diagram.puml` assumes one designated school workstation is the authoritative host. Do not place the SQLite database on a general network share or allow multiple independent database copies to be edited: SQLite is intended here as a local file database. If several staff need access, use the designated workstation under the school's approved local access arrangements. The application does not provide a browser/mobile client or remote multi-user service.

## 2. Hardware and software prerequisites

- Windows 10/11 64-bit or a supported Linux desktop, with adequate local storage and a backup device.
- C++17 runtime/build environment for compiling: Visual Studio C++ Build Tools/MSVC or GCC/Clang; CMake 3.20+.
- SQLite development library and OpenSSL development library for the build.
- For production build, set `BUILD_TESTING=OFF`; Catch2 is only needed for developer/CI tests.
- Do not use real learner information in development or test databases.

## 3. Build from a clean checkout

### Windows (Visual Studio Build Tools)

1. Install Visual Studio Build Tools with **Desktop development with C++**, MSVC x64/x86 tools and a Windows SDK.
2. Install CMake 3.20+.
3. Install vcpkg and the required libraries (example):

```powershell
vcpkg install sqlite3:x64-windows openssl:x64-windows
```

4. Open **Developer PowerShell for Visual Studio** and change directory to the repository root.
5. Configure, build, and verify the production executable:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake" `
  -DBUILD_TESTING=OFF
cmake --build build --config Release
```

6. Copy the executable and required OpenSSL runtime DLLs (if dynamically linked) into the approved application folder. Confirm SQLite is linked/available. Run from a writable data folder so the database and `submissions/` folder are created with restricted permissions.

### Ubuntu/Debian Linux

```bash
sudo apt update
sudo apt install -y build-essential cmake libsqlite3-dev libssl-dev
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
cmake --build build --parallel
./build/moderated_lms --init-only
```

## 4. First-run setup

1. Create a dedicated OS account for the school application and restrict folder permissions to authorised staff.
2. Run the application from the approved data directory. It creates `moderated_lms.db` and the `submissions/` folder when needed.
3. The current Phase 4 seed creates `admin` with the initial password `ChangeMe123!`. This is a development bootstrap credential. **Do not use the system with real learner data while this shared default credential remains active.** The current console application does not yet include a password-change screen; arrange a secure administrator credential provisioning change before live deployment.
4. Sign in with the bootstrap account only in a dummy-data environment. Create a department, accounts, courses, assign lecturers, and enrol students for demonstration/testing.
5. Verify that users have the correct roles and department values. Do not assign HoD access outside the person's department.

## 5. Data locations and access control

- Database: `moderated_lms.db` in the application working directory.
- Submission files: `submissions/` beneath the working directory.
- Restrict read/write access to the school application account and authorised administrator/IT staff. Ordinary students should not browse the raw database or submission directory.
- Never email or place the database in a public cloud folder. The database contains learner identifiers, account details, assessment marks, feedback and audit information.

## 6. Backup and restore

1. Ask users to log out and close the LMS cleanly.
2. Copy `moderated_lms.db` and the complete `submissions/` folder together to encrypted, access-controlled removable media. Keep a dated backup log.
3. Do not back up only the database if submission files are needed for restore; the database stores paths to those files.
4. To restore, close the application, move the damaged database/submissions directory aside, copy the database and submissions folder from the same backup set, restore folder permissions, and start the app.
5. Verify a known course, assignment, submission record, file, mark and audit entry. Record elapsed restore time. The Phase 2 target is completion within 15 minutes.
6. Test restore periodically on a separate copy; do not test by overwriting the live database.

## 7. Acceptance and handover checklist

- [ ] Clean build from repository succeeds on target workstation.
- [ ] Login with test accounts for all four roles.
- [ ] Role and department restrictions demonstrated.
- [ ] Offline workflow demonstrated.
- [ ] Database and submission directory access permissions checked.
- [ ] Backup and restore completed and timed.
- [ ] Default password provisioning risk resolved before real data use.
- [ ] Technician receives repository link, build steps, backup instructions and known limitations.
