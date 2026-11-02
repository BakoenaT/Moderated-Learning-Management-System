# Moderated LMS User Manual — Version 1.0

## Getting started (all users)

1. Start the `moderated_lms` application from the designated school workstation.
2. At the opening menu choose `1 Login`.
3. Enter the username and password supplied by the administrator. Password input is currently visible in the console; use the application only in a private, controlled environment and do not disclose credentials.
4. Choose the menu item for your role. Enter `0` to log out of the role menu; choose `0` again to exit the application.
5. Do not access the SQLite database or submission folder directly.

## Administrator

The Administrator creates and maintains academic structure and user accounts.

1. Choose `1 List users` to inspect registered accounts.
2. Choose `2 Create user`; select Administrator, Lecturer, HoD or Student; enter username, full name, optional email, initial password (at least 10 characters), and department ID where appropriate.
3. Choose `3 Activate/deactivate user` to disable or re-enable an account. Confirm the user ID carefully.
4. Choose `4 Create department`; record the returned department ID.
5. Choose `5 Create course`; enter department ID, course code, title and description. Record the course ID.
6. Choose `6 Assign lecturer`; enter the Lecturer user ID and course ID.
7. Choose `7 Enrol student`; enter the Student user ID and course ID.
8. Choose `8 List courses` to verify the academic structure.

Administrator safeguards: use unique accounts; select the correct role and department; deactivate leavers promptly; do not share administrator credentials.

## Lecturer

1. Choose `1 My courses` to view courses assigned to your account.
2. Choose `2 Create assignment`; select one of your course IDs, then enter title, instructions, deadline (`YYYY-MM-DD HH:MM:SS`) and a positive maximum mark.
3. Choose `3 Grant individual extension`; enter assignment ID, enrolled student ID, revised deadline and a reason. Only the lecturer assigned to the assignment may grant it.
4. Choose `4 View submissions` to inspect submission IDs, student IDs, versions and status.
5. Choose `5 Mark submission`; enter a submission ID, mark within the late-adjusted maximum, written feedback and reason for the mark/change. The mark remains hidden from the student until HoD moderation and release.

Version 1.0 supports individual extensions only. Whole-class extensions are introduced in maintenance release v1.1.

## Head of Department

1. Choose `1 Pending marks in my department` to inspect marks awaiting moderation or returned for correction.
2. Choose `2 Approve/return mark`; enter mark ID, choose `A` to approve or `R` to return, and provide a moderation reason.
3. Choose `3 Release approved mark` and enter an approved mark ID. Only approved marks in your department can be released.
4. Ensure every moderation/release decision is justified. Students must not be shown a mark before the explicit release operation.

## Student

1. Choose `1 Courses/assignments` to view only courses in which you are enrolled and their assignments/deadlines.
2. Choose `2 Submit or resubmit file`; enter assignment ID and the full path to the file on the school workstation. Keep the original file until the application confirms the saved version.
3. A first attempt may be accepted late under the configured late policy. Resubmissions are permitted only on or before the effective deadline. Earlier attempts remain in the submission history.
4. Choose `3 Released results` to view your own released marks and written feedback. Unreleased marks are not displayed.

## Support and safe use

Contact the school administrator/IT officer if login fails, a file cannot be submitted, or an assignment/course is missing. Do not attempt to fix data by editing the database. Report suspected incorrect marks or unauthorised access immediately. The v1.0 console does not yet provide password self-service or an accessibility-tested graphical interface; the school should provide assisted access where needed.
