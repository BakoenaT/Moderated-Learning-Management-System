PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS department (
  id INTEGER PRIMARY KEY,
  name TEXT NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS user_account (
  id INTEGER PRIMARY KEY,
  department_id INTEGER REFERENCES department(id),
  username TEXT NOT NULL UNIQUE,
  password_hash TEXT NOT NULL,
  full_name TEXT NOT NULL,
  email TEXT UNIQUE,
  role TEXT NOT NULL CHECK(role IN ('ADMINISTRATOR','LECTURER','HOD','STUDENT')),
  active INTEGER NOT NULL DEFAULT 1 CHECK(active IN (0,1)),
  created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS course (
  id INTEGER PRIMARY KEY,
  department_id INTEGER NOT NULL REFERENCES department(id),
  code TEXT NOT NULL UNIQUE,
  title TEXT NOT NULL,
  description TEXT
);

CREATE TABLE IF NOT EXISTS lecturer_course (
  lecturer_id INTEGER NOT NULL REFERENCES user_account(id),
  course_id INTEGER NOT NULL REFERENCES course(id),
  PRIMARY KEY(lecturer_id,course_id)
);

CREATE TABLE IF NOT EXISTS enrolment (
  id INTEGER PRIMARY KEY,
  student_id INTEGER NOT NULL REFERENCES user_account(id),
  course_id INTEGER NOT NULL REFERENCES course(id),
  UNIQUE(student_id,course_id)
);

CREATE TABLE IF NOT EXISTS assignment (
  id INTEGER PRIMARY KEY,
  course_id INTEGER NOT NULL REFERENCES course(id),
  lecturer_id INTEGER NOT NULL,
  title TEXT NOT NULL,
  instructions TEXT,
  deadline TEXT NOT NULL,
  max_mark REAL NOT NULL CHECK(max_mark > 0),
  FOREIGN KEY(lecturer_id,course_id) REFERENCES lecturer_course(lecturer_id,course_id)
);

CREATE TABLE IF NOT EXISTS extension_grant (
  id INTEGER PRIMARY KEY,
  assignment_id INTEGER NOT NULL REFERENCES assignment(id) ON DELETE CASCADE,
  student_id INTEGER NOT NULL REFERENCES user_account(id),
  granted_by INTEGER NOT NULL REFERENCES user_account(id),
  deadline TEXT NOT NULL,
  reason TEXT NOT NULL,
  extension_scope TEXT NOT NULL DEFAULT 'INDIVIDUAL' CHECK(extension_scope IN ('INDIVIDUAL','CLASS')),
  UNIQUE(assignment_id,student_id)
);

CREATE TABLE IF NOT EXISTS submission (
  id INTEGER PRIMARY KEY,
  assignment_id INTEGER NOT NULL REFERENCES assignment(id),
  student_id INTEGER NOT NULL REFERENCES user_account(id),
  submitted_at TEXT NOT NULL,
  file_name TEXT NOT NULL,
  file_path TEXT NOT NULL,
  status TEXT NOT NULL CHECK(status IN ('SUBMITTED','LATE','MARKED','MODERATED','RELEASED')),
  version_no INTEGER NOT NULL CHECK(version_no >= 1),
  UNIQUE(assignment_id,student_id,version_no)
);

CREATE TABLE IF NOT EXISTS mark_record (
  id INTEGER PRIMARY KEY,
  submission_id INTEGER NOT NULL UNIQUE REFERENCES submission(id),
  lecturer_id INTEGER NOT NULL REFERENCES user_account(id),
  mark REAL NOT NULL CHECK(mark >= 0),
  feedback TEXT NOT NULL,
  moderation TEXT NOT NULL DEFAULT 'PENDING' CHECK(moderation IN ('PENDING','RETURNED','APPROVED','RELEASED')) ,
  marked_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
  moderated_by INTEGER REFERENCES user_account(id),
  released_by INTEGER REFERENCES user_account(id)
);

CREATE TABLE IF NOT EXISTS audit_log (
  id INTEGER PRIMARY KEY,
  mark_id INTEGER REFERENCES mark_record(id),
  actor_id INTEGER NOT NULL REFERENCES user_account(id),
  action TEXT NOT NULL,
  changed_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
  old_mark REAL,
  new_mark REAL,
  reason TEXT NOT NULL,
  details TEXT
);

CREATE INDEX IF NOT EXISTS idx_assignment_course_deadline ON assignment(course_id,deadline);
CREATE INDEX IF NOT EXISTS idx_submission_student_status ON submission(student_id,status);
CREATE INDEX IF NOT EXISTS idx_mark_moderation ON mark_record(moderation);
