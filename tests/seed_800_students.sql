-- Run only against a disposable test database after the application has initialised it.
-- Inserts 800 synthetic student accounts and enrols them in one test course.
PRAGMA foreign_keys = ON;

INSERT OR IGNORE INTO department(id,name) VALUES(9900,'PERFORMANCE TEST DEPARTMENT');
INSERT OR IGNORE INTO user_account(id,department_id,username,password_hash,full_name,email,role,active)
VALUES(9900,9900,'perf_lecturer','NOT_FOR_LOGIN_TEST','Performance Test Lecturer','perf_lecturer@example.invalid','LECTURER',1);
INSERT OR IGNORE INTO course(id,department_id,code,title,description)
VALUES(9900,9900,'PERF800','Performance Test Course','Synthetic data for NFR-01 measurement only');
INSERT OR IGNORE INTO lecturer_course(lecturer_id,course_id) VALUES(9900,9900);

WITH RECURSIVE seq(n) AS (
  SELECT 1 UNION ALL SELECT n+1 FROM seq WHERE n < 800
)
INSERT OR IGNORE INTO user_account(department_id,username,password_hash,full_name,email,role,active)
SELECT 9900, 'perf_student_'||n, 'NOT_FOR_LOGIN_TEST', 'Performance Student '||n,
       'perf_student_'||n||'@example.invalid', 'STUDENT', 1
FROM seq;

INSERT OR IGNORE INTO enrolment(student_id,course_id)
SELECT id,9900 FROM user_account WHERE username LIKE 'perf_student_%';

INSERT OR IGNORE INTO assignment(course_id,lecturer_id,title,instructions,deadline,max_mark)
VALUES(9900,9900,'NFR-01 Performance Assignment','Synthetic test record','2026-12-01 23:59:00',100);

-- Confirm sample size. Expected result: 800.
SELECT COUNT(*) AS synthetic_student_count
FROM enrolment e JOIN course c ON c.id=e.course_id
WHERE c.id=9900;
