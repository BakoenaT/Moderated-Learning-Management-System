-- CS4433 Phase 3: Moderated LMS database schema
-- Target: MySQL 8.0.16+ with InnoDB and utf8mb4 (CHECK constraints are enforced).
-- Run in a development database first. Then use Database > Reverse Engineer
-- in MySQL Workbench to generate and save the EER model.
CREATE DATABASE IF NOT EXISTS moderated_lms
  CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci;
USE moderated_lms;

CREATE TABLE department (
  department_id INT AUTO_INCREMENT PRIMARY KEY,
  name VARCHAR(100) NOT NULL UNIQUE
) ENGINE=InnoDB;

CREATE TABLE user_account (
  user_id INT AUTO_INCREMENT PRIMARY KEY,
  department_id INT NULL,
  username VARCHAR(80) NOT NULL UNIQUE,
  password_hash VARCHAR(255) NOT NULL,
  full_name VARCHAR(160) NOT NULL,
  email VARCHAR(160) NULL UNIQUE,
  role ENUM('ADMINISTRATOR','LECTURER','HOD','STUDENT') NOT NULL,
  is_active BOOLEAN NOT NULL DEFAULT TRUE,
  created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  CONSTRAINT fk_user_department FOREIGN KEY (department_id)
    REFERENCES department(department_id) ON UPDATE CASCADE ON DELETE SET NULL
) ENGINE=InnoDB;

CREATE TABLE course (
  course_id INT AUTO_INCREMENT PRIMARY KEY,
  department_id INT NOT NULL,
  course_code VARCHAR(30) NOT NULL UNIQUE,
  title VARCHAR(160) NOT NULL,
  description TEXT NULL,
  is_active BOOLEAN NOT NULL DEFAULT TRUE,
  CONSTRAINT fk_course_department FOREIGN KEY (department_id)
    REFERENCES department(department_id) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB;

CREATE TABLE lecturer_course (
  lecturer_id INT NOT NULL,
  course_id INT NOT NULL,
  assigned_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (lecturer_id, course_id),
  CONSTRAINT fk_lc_lecturer FOREIGN KEY (lecturer_id)
    REFERENCES user_account(user_id) ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_lc_course FOREIGN KEY (course_id)
    REFERENCES course(course_id) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB;

CREATE TABLE enrolment (
  enrolment_id INT AUTO_INCREMENT PRIMARY KEY,
  student_id INT NOT NULL,
  course_id INT NOT NULL,
  enrolled_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  UNIQUE KEY uq_student_course (student_id, course_id),
  CONSTRAINT fk_enrolment_student FOREIGN KEY (student_id)
    REFERENCES user_account(user_id) ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_enrolment_course FOREIGN KEY (course_id)
    REFERENCES course(course_id) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB;

CREATE TABLE assignment (
  assignment_id INT AUTO_INCREMENT PRIMARY KEY,
  course_id INT NOT NULL,
  lecturer_id INT NOT NULL,
  title VARCHAR(180) NOT NULL,
  instructions TEXT NULL,
  deadline DATETIME NOT NULL,
  max_mark DECIMAL(7,2) NOT NULL,
  created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  CONSTRAINT chk_assignment_max_mark CHECK (max_mark > 0),
  CONSTRAINT fk_assignment_course FOREIGN KEY (course_id)
    REFERENCES course(course_id) ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_assignment_lecturer_course
    FOREIGN KEY (lecturer_id, course_id)
    REFERENCES lecturer_course(lecturer_id, course_id)
    ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB;

CREATE TABLE extension_grant (
  extension_id INT AUTO_INCREMENT PRIMARY KEY,
  assignment_id INT NOT NULL,
  student_id INT NOT NULL,
  granted_by INT NOT NULL,
  revised_deadline DATETIME NOT NULL,
  reason VARCHAR(500) NOT NULL,
  granted_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  UNIQUE KEY uq_extension_assignment_student (assignment_id, student_id),
  CONSTRAINT fk_extension_assignment FOREIGN KEY (assignment_id)
    REFERENCES assignment(assignment_id) ON UPDATE CASCADE ON DELETE CASCADE,
  CONSTRAINT fk_extension_student FOREIGN KEY (student_id)
    REFERENCES user_account(user_id) ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_extension_granted_by FOREIGN KEY (granted_by)
    REFERENCES user_account(user_id) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB;

CREATE TABLE submission (
  submission_id INT AUTO_INCREMENT PRIMARY KEY,
  assignment_id INT NOT NULL,
  student_id INT NOT NULL,
  submitted_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  file_name VARCHAR(255) NOT NULL,
  file_location VARCHAR(500) NOT NULL,
  status ENUM('SUBMITTED','LATE','MARKED','MODERATED','RELEASED') NOT NULL DEFAULT 'SUBMITTED',
  version_no INT NOT NULL DEFAULT 1,
  CONSTRAINT chk_submission_version CHECK (version_no >= 1),
  UNIQUE KEY uq_submission_version (assignment_id, student_id, version_no),
  KEY ix_submission_assignment_student (assignment_id, student_id),
  CONSTRAINT fk_submission_assignment FOREIGN KEY (assignment_id)
    REFERENCES assignment(assignment_id) ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_submission_student FOREIGN KEY (student_id)
    REFERENCES user_account(user_id) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB;

CREATE TABLE mark_record (
  mark_id INT AUTO_INCREMENT PRIMARY KEY,
  submission_id INT NOT NULL UNIQUE,
  lecturer_id INT NOT NULL,
  mark_value DECIMAL(7,2) NOT NULL,
  feedback TEXT NOT NULL,
  moderation_status ENUM('PENDING','RETURNED','APPROVED','RELEASED') NOT NULL DEFAULT 'PENDING',
  marked_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  moderated_by INT NULL,
  moderated_at DATETIME NULL,
  released_by INT NULL,
  released_at DATETIME NULL,
  CONSTRAINT chk_mark_value_nonnegative CHECK (mark_value >= 0),
  CONSTRAINT fk_mark_submission FOREIGN KEY (submission_id)
    REFERENCES submission(submission_id) ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_mark_lecturer FOREIGN KEY (lecturer_id)
    REFERENCES user_account(user_id) ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_mark_moderator FOREIGN KEY (moderated_by)
    REFERENCES user_account(user_id) ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_mark_releaser FOREIGN KEY (released_by)
    REFERENCES user_account(user_id) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB;

CREATE TABLE audit_log (
  audit_id BIGINT AUTO_INCREMENT PRIMARY KEY,
  mark_id INT NULL,
  actor_id INT NOT NULL,
  action_type VARCHAR(40) NOT NULL,
  changed_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  old_value DECIMAL(7,2) NULL,
  new_value DECIMAL(7,2) NULL,
  reason VARCHAR(500) NULL,
  details JSON NULL,
  CONSTRAINT fk_audit_mark FOREIGN KEY (mark_id)
    REFERENCES mark_record(mark_id) ON UPDATE CASCADE ON DELETE SET NULL,
  CONSTRAINT fk_audit_actor FOREIGN KEY (actor_id)
    REFERENCES user_account(user_id) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB;

CREATE INDEX ix_assignment_course_deadline ON assignment(course_id, deadline);
CREATE INDEX ix_submission_student_status ON submission(student_id, status);
CREATE INDEX ix_mark_moderation_status ON mark_record(moderation_status);

-- Cross-table business rules (e.g., mark_value <= assignment.max_mark,
-- correct role for actor, enrolled student, permitted late window) must be
-- validated by application services within transactions. They cannot be
-- expressed by ordinary single-table CHECK constraints.
