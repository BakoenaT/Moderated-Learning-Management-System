#include "business_rules.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace lms {
namespace {

bool leapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int parseDigits(const std::string& value, std::size_t start, std::size_t count) {
    int result = 0;
    for (std::size_t i = start; i < start + count; ++i) {
        if (i >= value.size() || !std::isdigit(static_cast<unsigned char>(value[i]))) {
            throw std::invalid_argument("Date/time must use YYYY-MM-DD [HH:MM:SS] format.");
        }
        result = result * 10 + (value[i] - '0');
    }
    return result;
}

// Gregorian calendar serial-day calculation; independent of local timezone/DST.
std::int64_t civilDay(int year, unsigned month, unsigned day) {
    year -= month <= 2;
    const int era = (year >= 0 ? year : year - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(year - era * 400);
    const unsigned adjustedMonth = month > 2 ? month - 3 : month + 9;
    const unsigned doy = (153 * adjustedMonth + 2) / 5 + day - 1;
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return static_cast<std::int64_t>(era) * 146097 + static_cast<std::int64_t>(doe);
}

void validateIsoDateTime(const std::string& value) {
    if (value.size() != 10 && value.size() != 19) {
        throw std::invalid_argument("Date/time must be YYYY-MM-DD or YYYY-MM-DD HH:MM:SS.");
    }
    if (value[4] != '-' || value[7] != '-') {
        throw std::invalid_argument("Date/time must use YYYY-MM-DD format.");
    }
    const int year = parseDigits(value, 0, 4);
    const int month = parseDigits(value, 5, 2);
    const int day = parseDigits(value, 8, 2);
    static const int monthDays[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (year < 1 || month < 1 || month > 12) {
        throw std::invalid_argument("Invalid calendar date.");
    }
    int maximumDay = monthDays[month - 1];
    if (month == 2 && leapYear(year)) maximumDay = 29;
    if (day < 1 || day > maximumDay) throw std::invalid_argument("Invalid calendar date.");
    if (value.size() == 19) {
        if (value[10] != ' ' || value[13] != ':' || value[16] != ':') {
            throw std::invalid_argument("Date/time must use YYYY-MM-DD HH:MM:SS format.");
        }
        const int hour = parseDigits(value, 11, 2);
        const int minute = parseDigits(value, 14, 2);
        const int second = parseDigits(value, 17, 2);
        if (hour > 23 || minute > 59 || second > 59) {
            throw std::invalid_argument("Invalid time value.");
        }
    }
}

std::vector<int> enrolledStudents(sqlite3* db, int courseId) {
    sqlite3_stmt* statement = nullptr;
    const char* sql = "SELECT student_id FROM enrolment WHERE course_id = ? ORDER BY student_id";
    if (sqlite3_prepare_v2(db, sql, -1, &statement, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }
    sqlite3_bind_int(statement, 1, courseId);
    std::vector<int> students;
    int result = SQLITE_ROW;
    while ((result = sqlite3_step(statement)) == SQLITE_ROW) {
        students.push_back(sqlite3_column_int(statement, 0));
    }
    if (result != SQLITE_DONE) {
        const std::string error = sqlite3_errmsg(db);
        sqlite3_finalize(statement);
        throw std::runtime_error(error);
    }
    sqlite3_finalize(statement);
    return students;
}

} // namespace

int calendarDaysLate(const std::string& deadline, const std::string& submittedAt) {
    validateIsoDateTime(deadline);
    validateIsoDateTime(submittedAt);
    const auto deadlineDay = civilDay(parseDigits(deadline, 0, 4),
                                      static_cast<unsigned>(parseDigits(deadline, 5, 2)),
                                      static_cast<unsigned>(parseDigits(deadline, 8, 2)));
    const auto submittedDay = civilDay(parseDigits(submittedAt, 0, 4),
                                       static_cast<unsigned>(parseDigits(submittedAt, 5, 2)),
                                       static_cast<unsigned>(parseDigits(submittedAt, 8, 2)));
    return static_cast<int>(std::max<std::int64_t>(0, submittedDay - deadlineDay));
}

double lateAdjustedMaximum(double maximumMark, int lateDays) {
    if (maximumMark < 0.0) throw std::invalid_argument("Maximum mark cannot be negative.");
    if (lateDays < 0) throw std::invalid_argument("Late days cannot be negative.");
    if (lateDays > 3) return 0.0;
    return maximumMark * (1.0 - 0.10 * lateDays);
}

bool submissionTimeAllowed(const std::string& effectiveDeadline,
                           const std::string& submittedAt,
                           bool alreadySubmitted) {
    validateIsoDateTime(effectiveDeadline);
    validateIsoDateTime(submittedAt);
    if (!alreadySubmitted) return true;
    // Canonical ISO date/time strings sort chronologically as text.
    return submittedAt <= effectiveDeadline;
}

int grantWholeClassExtension(sqlite3* db, int assignmentId, int lecturerId,
                             const std::string& revisedDeadline, const std::string& reason) {
    if (db == nullptr) throw std::invalid_argument("Database connection is required.");
    if (assignmentId <= 0 || lecturerId <= 0) throw std::invalid_argument("Assignment and lecturer IDs must be positive.");
    validateIsoDateTime(revisedDeadline);
    if (reason.empty()) throw std::invalid_argument("A reason is required for an extension.");

    sqlite3_stmt* assignment = nullptr;
    const char* assignmentSql = "SELECT course_id, deadline FROM assignment WHERE id = ? AND lecturer_id = ?";
    if (sqlite3_prepare_v2(db, assignmentSql, -1, &assignment, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }
    sqlite3_bind_int(assignment, 1, assignmentId);
    sqlite3_bind_int(assignment, 2, lecturerId);
    const int assignmentResult = sqlite3_step(assignment);
    if (assignmentResult != SQLITE_ROW) {
        sqlite3_finalize(assignment);
        throw std::runtime_error("Assignment not found or lecturer is not assigned to it.");
    }
    const int courseId = sqlite3_column_int(assignment, 0);
    const auto* originalDeadlineText = sqlite3_column_text(assignment, 1);
    const std::string originalDeadline = originalDeadlineText
        ? reinterpret_cast<const char*>(originalDeadlineText) : "";
    sqlite3_finalize(assignment);
    validateIsoDateTime(originalDeadline);
    if (revisedDeadline <= originalDeadline) {
        throw std::invalid_argument("A class extension must be later than the original assignment deadline.");
    }

    sqlite3_stmt* previousClassGrant = nullptr;
    const char* previousClassSql =
        "SELECT MAX(deadline) FROM extension_grant WHERE assignment_id = ? AND extension_scope = 'CLASS'";
    if (sqlite3_prepare_v2(db, previousClassSql, -1, &previousClassGrant, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }
    sqlite3_bind_int(previousClassGrant, 1, assignmentId);
    const int previousResult = sqlite3_step(previousClassGrant);
    if (previousResult != SQLITE_ROW) {
        const std::string message = sqlite3_errmsg(db);
        sqlite3_finalize(previousClassGrant);
        throw std::runtime_error(message);
    }
    const auto* previousDeadlineText = sqlite3_column_text(previousClassGrant, 0);
    const std::string previousClassDeadline = previousDeadlineText
        ? reinterpret_cast<const char*>(previousDeadlineText) : "";
    sqlite3_finalize(previousClassGrant);
    if (!previousClassDeadline.empty() && revisedDeadline <= previousClassDeadline) {
        throw std::invalid_argument("A new class extension cannot shorten an existing class-wide deadline.");
    }

    const auto students = enrolledStudents(db, courseId);
    if (students.empty()) throw std::runtime_error("There are no enrolled students in this class.");

    char* error = nullptr;
    if (sqlite3_exec(db, "BEGIN IMMEDIATE", nullptr, nullptr, &error) != SQLITE_OK) {
        std::string message = error ? error : sqlite3_errmsg(db);
        sqlite3_free(error);
        throw std::runtime_error(message);
    }

    int affected = 0;
    try {
        const char* upsertSql =
            "INSERT INTO extension_grant(assignment_id, student_id, granted_by, deadline, reason, extension_scope) "
            "VALUES(?, ?, ?, ?, ?, 'CLASS') "
            "ON CONFLICT(assignment_id, student_id) DO UPDATE SET "
            "granted_by=excluded.granted_by, deadline=excluded.deadline, reason=excluded.reason, "
            "extension_scope='CLASS' "
            "WHERE extension_grant.extension_scope='CLASS' AND extension_grant.deadline < excluded.deadline";
        sqlite3_stmt* upsert = nullptr;
        if (sqlite3_prepare_v2(db, upsertSql, -1, &upsert, nullptr) != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(db));
        }
        for (const int studentId : students) {
            sqlite3_reset(upsert);
            sqlite3_clear_bindings(upsert);
            sqlite3_bind_int(upsert, 1, assignmentId);
            sqlite3_bind_int(upsert, 2, studentId);
            sqlite3_bind_int(upsert, 3, lecturerId);
            sqlite3_bind_text(upsert, 4, revisedDeadline.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(upsert, 5, reason.c_str(), -1, SQLITE_TRANSIENT);
            if (sqlite3_step(upsert) != SQLITE_DONE) {
                const std::string message = sqlite3_errmsg(db);
                sqlite3_finalize(upsert);
                throw std::runtime_error(message);
            }
            affected += sqlite3_changes(db);
        }
        sqlite3_finalize(upsert);
        if (sqlite3_exec(db, "COMMIT", nullptr, nullptr, &error) != SQLITE_OK) {
            std::string message = error ? error : sqlite3_errmsg(db);
            sqlite3_free(error);
            throw std::runtime_error(message);
        }
    } catch (...) {
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr);
        throw;
    }
    return affected;
}

std::string extensionLabel(const std::string& scope) {
    if (scope == "CLASS") return "Class extension";
    if (scope == "INDIVIDUAL") return "Individual extension";
    return "No extension";
}

} // namespace lms
