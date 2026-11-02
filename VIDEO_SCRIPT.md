# Moderated LMS demonstration video script

**Suggested duration:** 8–10 minutes (follow any different duration set by the lecturer).  
**Recording software:** OBS Studio, Windows Game Bar, or another screen recorder.  
**Output:** `Moderated_LMS_Demonstration.mp4` in this folder.

## Before recording

- Build and launch the application successfully on the demonstration computer.
- Prepare synthetic demo accounts for Administrator, Lecturer, HoD and Student.
- Prepare a demo department, course, assignment, enrolment and a harmless sample file.
- Do not display real learner information, passwords, personal email addresses or marks.
- Close unrelated windows/notifications and check microphone audio.
- Run one short test recording and verify picture and sound.

## Suggested demonstration sequence

| Time | Demonstration | What to show |
|---|---|---|
| 00:00–00:40 | Introduction | Project name, problem, four roles, C++17/SQLite/offline context |
| 00:40–01:25 | Repository and design | GitHub README, CMake project structure, key diagrams and runtime SQLite schema |
| 01:25–02:25 | Administrator | Log in; create a department/course/user; assign lecturer; enrol student |
| 02:25–03:55 | Lecturer | View assigned course; create assignment; grant individual and whole-class extension |
| 03:55–05:05 | Student | View enrolled assignments and effective deadline; submit/resubmit a synthetic file |
| 05:05–06:20 | Marking and moderation | Lecturer records mark/feedback; HoD reviews, returns or approves, then releases |
| 06:20–07:10 | Security and persistence | Show released-mark visibility, audit rows, hashed password field (not actual password), and persistence after restart |
| 07:10–08:10 | Tests and quality | Show actual executed test evidence and GitHub Actions status; distinguish pending checks |
| 08:10–09:00 | Conclusion | Summarise features, known limitations and repository location |

## Recording and upload

1. Record the demonstration with a clear spoken explanation.
2. Edit out pauses, accidental exposure of personal data and unrelated desktop activity.
3. Export MP4 (H.264 video; 720p or 1080p is suitable).
4. Save as `Moderated_LMS_Demonstration.mp4` in this folder.
5. If the file is too large for a normal GitHub browser upload, use Git LFS or a course-approved video service and place the view-only URL in `recordings/README.md` and the final report.
6. Check the uploaded recording plays and that no private/real learner data is exposed.

## Evidence integrity

Show only features that really run in the submitted build. Do not show planned tests as passed, mock-ups as working software, or a prepared CI workflow as a successful CI run. Where a test is pending, state that it remains pending.
