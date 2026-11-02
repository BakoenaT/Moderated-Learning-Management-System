#include "business_rules.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <sqlite3.h>
#include <memory>
#include <string>
#include <stdexcept>

namespace {
struct InMemoryDb {
    sqlite3* db = nullptr;
    InMemoryDb() {
        REQUIRE(sqlite3_open(":memory:", &db) == SQLITE_OK);
        const char* schema = R"SQL(
CREATE TABLE assignment(id INTEGER PRIMARY KEY, course_id INTEGER NOT NULL, lecturer_id INTEGER NOT NULL, deadline TEXT NOT NULL);
CREATE TABLE enrolment(student_id INTEGER NOT NULL, course_id INTEGER NOT NULL, PRIMARY KEY(student_id,course_id));
CREATE TABLE extension_grant(
 id INTEGER PRIMARY KEY AUTOINCREMENT,
 assignment_id INTEGER NOT NULL,
 student_id INTEGER NOT NULL,
 granted_by INTEGER NOT NULL,
 deadline TEXT NOT NULL,
 reason TEXT NOT NULL,
 extension_scope TEXT NOT NULL DEFAULT 'INDIVIDUAL' CHECK(extension_scope IN('INDIVIDUAL','CLASS')),
 UNIQUE(assignment_id,student_id));
INSERT INTO assignment(id,course_id,lecturer_id,deadline) VALUES(10,100,7,'2026-11-01 12:00:00'),(20,200,8,'2026-11-01 12:00:00');
INSERT INTO enrolment(student_id,course_id) VALUES(1,100),(2,100),(3,100),(4,200);
INSERT INTO extension_grant(assignment_id,student_id,granted_by,deadline,reason,extension_scope)
VALUES(10,1,7,'2026-11-10 10:00:00','Approved personal case','INDIVIDUAL');
)SQL";
        char* error = nullptr;
        const int rc = sqlite3_exec(db, schema, nullptr, nullptr, &error);
        if (rc != SQLITE_OK) {
            const std::string message = error ? error : sqlite3_errmsg(db);
            sqlite3_free(error);
            throw std::runtime_error(message);
        }
    }
    ~InMemoryDb() { if (db) sqlite3_close(db); }
};

int countRows(sqlite3* db, const char* sql) {
    sqlite3_stmt* st = nullptr;
    REQUIRE(sqlite3_prepare_v2(db, sql, -1, &st, nullptr) == SQLITE_OK);
    REQUIRE(sqlite3_step(st) == SQLITE_ROW);
    const int count = sqlite3_column_int(st, 0);
    sqlite3_finalize(st);
    return count;
}
std::string textValue(sqlite3* db, const char* sql, int assignmentId, int studentId) {
    sqlite3_stmt* st = nullptr;
    REQUIRE(sqlite3_prepare_v2(db, sql, -1, &st, nullptr) == SQLITE_OK);
    sqlite3_bind_int(st, 1, assignmentId);
    sqlite3_bind_int(st, 2, studentId);
    REQUIRE(sqlite3_step(st) == SQLITE_ROW);
    const auto* value = sqlite3_column_text(st, 0);
    std::string result = value ? reinterpret_cast<const char*>(value) : "";
    sqlite3_finalize(st);
    return result;
}
}

TEST_CASE("Calendar-day late calculation observes boundary partitions", "[unit][late]") {
    SECTION("on deadline") { REQUIRE(lms::calendarDaysLate("2026-10-01 23:59:59", "2026-10-01 00:00:00") == 0); }
    SECTION("one day late") { REQUIRE(lms::calendarDaysLate("2026-10-01 12:00:00", "2026-10-02 00:00:01") == 1); }
    SECTION("two days late") { REQUIRE(lms::calendarDaysLate("2026-10-01", "2026-10-03") == 2); }
    SECTION("three days late") { REQUIRE(lms::calendarDaysLate("2026-10-01", "2026-10-04") == 3); }
    SECTION("more than three days late") { REQUIRE(lms::calendarDaysLate("2026-10-01", "2026-10-05") == 4); }
    SECTION("early submission") { REQUIRE(lms::calendarDaysLate("2026-10-05", "2026-10-04") == 0); }
}

TEST_CASE("Calendar date parser handles leap years and invalid dates", "[unit][date]") {
    REQUIRE(lms::calendarDaysLate("2024-02-28", "2024-03-01") == 2);
    REQUIRE_THROWS_AS(lms::calendarDaysLate("2025-02-29", "2025-03-01"), std::invalid_argument);
    REQUIRE_THROWS_AS(lms::calendarDaysLate("2026-13-01", "2026-13-02"), std::invalid_argument);
    REQUIRE_THROWS_AS(lms::calendarDaysLate("2026-10-01 25:00:00", "2026-10-02 10:00:00"), std::invalid_argument);
}

TEST_CASE("Late penalty maximum uses the correct 10 percent boundaries", "[unit][penalty]") {
    REQUIRE(lms::lateAdjustedMaximum(100.0, 0) == Catch::Approx(100.0));
    REQUIRE(lms::lateAdjustedMaximum(100.0, 1) == Catch::Approx(90.0));
    REQUIRE(lms::lateAdjustedMaximum(100.0, 2) == Catch::Approx(80.0));
    REQUIRE(lms::lateAdjustedMaximum(100.0, 3) == Catch::Approx(70.0));
    REQUIRE(lms::lateAdjustedMaximum(100.0, 4) == Catch::Approx(0.0));
    REQUIRE_THROWS_AS(lms::lateAdjustedMaximum(-1.0, 0), std::invalid_argument);
    REQUIRE_THROWS_AS(lms::lateAdjustedMaximum(100.0, -1), std::invalid_argument);
}

TEST_CASE("Resubmission is allowed only at or before the effective deadline", "[unit][submission]") {
    REQUIRE(lms::submissionTimeAllowed("2026-11-01 12:00:00", "2026-11-01 12:00:00", true));
    REQUIRE(lms::submissionTimeAllowed("2026-11-01 12:00:00", "2026-11-01 11:59:59", true));
    REQUIRE_FALSE(lms::submissionTimeAllowed("2026-11-01 12:00:00", "2026-11-01 12:00:01", true));
    // Initial work is accepted after the deadline; the late rule may reduce the mark to zero.
    REQUIRE(lms::submissionTimeAllowed("2026-11-01 12:00:00", "2026-11-06 12:00:00", false));
}

TEST_CASE("Whole-class extension updates enrolled students and preserves individual overrides", "[integration][cr1]") {
    InMemoryDb fixture;
    const int affected = lms::grantWholeClassExtension(fixture.db, 10, 7,
        "2026-11-15 23:59:00", "Class fieldwork disruption");
    REQUIRE(affected == 2); // students 2 and 3; student 1 has an individual override.
    REQUIRE(countRows(fixture.db, "SELECT COUNT(*) FROM extension_grant WHERE assignment_id=10") == 3);
    REQUIRE(textValue(fixture.db,
        "SELECT extension_scope FROM extension_grant WHERE assignment_id=? AND student_id=?",10,1) == "INDIVIDUAL");
    REQUIRE(textValue(fixture.db,
        "SELECT deadline FROM extension_grant WHERE assignment_id=? AND student_id=?",10,2) == "2026-11-15 23:59:00");
    REQUIRE(textValue(fixture.db,
        "SELECT extension_scope FROM extension_grant WHERE assignment_id=? AND student_id=?",10,2) == "CLASS");
    REQUIRE(countRows(fixture.db, "SELECT COUNT(*) FROM extension_grant WHERE assignment_id=20") == 0);
    // A later operation cannot shorten any existing class-wide deadline.
    REQUIRE_THROWS(lms::grantWholeClassExtension(fixture.db, 10, 7,
        "2026-11-12 23:59:00", "Earlier than current class grant"));
    REQUIRE(textValue(fixture.db,
        "SELECT deadline FROM extension_grant WHERE assignment_id=? AND student_id=?",10,2) == "2026-11-15 23:59:00");
    // A later class-wide extension updates CLASS rows but does not alter an individual exception.
    REQUIRE(lms::grantWholeClassExtension(fixture.db, 10, 7,
        "2026-11-16 23:59:00", "Later class-wide extension") == 2);
    REQUIRE(textValue(fixture.db,
        "SELECT deadline FROM extension_grant WHERE assignment_id=? AND student_id=?",10,2) == "2026-11-16 23:59:00");
    REQUIRE(textValue(fixture.db,
        "SELECT deadline FROM extension_grant WHERE assignment_id=? AND student_id=?",10,1) == "2026-11-10 10:00:00");
    REQUIRE_THROWS(lms::grantWholeClassExtension(fixture.db, 10, 7,
        "2026-10-30 10:00:00", "Earlier than original deadline"));
}

TEST_CASE("Whole-class extension rejects unauthorised lecturer and empty class", "[integration][cr1][security]") {
    InMemoryDb fixture;
    REQUIRE_THROWS(lms::grantWholeClassExtension(nullptr, 10, 7,
        "2026-11-15 23:59:00", "Null database"));
    REQUIRE_THROWS(lms::grantWholeClassExtension(fixture.db, 0, 7,
        "2026-11-15 23:59:00", "Invalid ID"));
    REQUIRE_THROWS(lms::grantWholeClassExtension(fixture.db, 10, 99,
        "2026-11-15 23:59:00", "Unauthorised"));
    REQUIRE_THROWS(lms::grantWholeClassExtension(fixture.db, 10, 7,
        "2026-11-15 25:00:00", "Invalid deadline"));
    REQUIRE_THROWS(lms::grantWholeClassExtension(fixture.db, 10, 7,
        "2026-11-15 23:59:00", ""));
    char* error = nullptr;
    REQUIRE(sqlite3_exec(fixture.db, "DELETE FROM enrolment WHERE course_id=100", nullptr, nullptr, &error) == SQLITE_OK);
    REQUIRE_THROWS(lms::grantWholeClassExtension(fixture.db, 10, 7,
        "2026-11-15 23:59:00", "No students"));
}

TEST_CASE("Extension label distinguishes class and individual extension", "[unit][cr1]") {
    REQUIRE(lms::extensionLabel("CLASS") == "Class extension");
    REQUIRE(lms::extensionLabel("INDIVIDUAL") == "Individual extension");
    REQUIRE(lms::extensionLabel("") == "No extension");
}

TEST_CASE("Whole-class extension is atomic when a student row fails", "[integration][cr1][transaction]") {
    InMemoryDb fixture;
    char* error = nullptr;
    const int rc = sqlite3_exec(fixture.db, R"SQL(
CREATE TRIGGER reject_student_three
BEFORE INSERT ON extension_grant
WHEN NEW.assignment_id = 10 AND NEW.student_id = 3
BEGIN
  SELECT RAISE(ABORT, 'simulated row failure');
END;
)SQL", nullptr, nullptr, &error);
    if (rc != SQLITE_OK) {
        const std::string message = error ? error : sqlite3_errmsg(fixture.db);
        sqlite3_free(error);
        FAIL(message);
    }
    REQUIRE_THROWS(lms::grantWholeClassExtension(fixture.db, 10, 7,
        "2026-11-15 23:59:00", "Atomicity test"));
    REQUIRE(countRows(fixture.db,
        "SELECT COUNT(*) FROM extension_grant WHERE assignment_id=10 AND extension_scope='CLASS'") == 0);
    REQUIRE(countRows(fixture.db,
        "SELECT COUNT(*) FROM extension_grant WHERE assignment_id=10 AND extension_scope='INDIVIDUAL'") == 1);
}
