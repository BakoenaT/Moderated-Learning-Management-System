#pragma once

#include <sqlite3.h>
#include <string>

namespace lms {

// Accepts canonical YYYY-MM-DD or YYYY-MM-DD HH:MM:SS strings.
// Returns the number of calendar dates elapsed, never a negative value.
int calendarDaysLate(const std::string& deadline, const std::string& submittedAt);

// The assignment brief's late rule: 10% of the assignment maximum per late day,
// for days 1-3; work later than day 3 receives zero.
double lateAdjustedMaximum(double maximumMark, int lateDays);

// First attempts may be submitted late (including after day 3, where the mark is zero).
// A resubmission is permitted only up to and including the student's effective deadline.
bool submissionTimeAllowed(const std::string& effectiveDeadline,
                            const std::string& submittedAt,
                            bool alreadySubmitted);

// CR1: apply one lecturer-authorised deadline to currently enrolled students in the
// assignment's course. Existing individual extensions remain as student-specific overrides.
// Requires extension_grant.extension_scope ('INDIVIDUAL' or 'CLASS') to exist.
// Returns the number of student records created/updated by the class grant.
int grantWholeClassExtension(sqlite3* db, int assignmentId, int lecturerId,
                             const std::string& revisedDeadline,
                             const std::string& reason);

std::string extensionLabel(const std::string& scope);

} // namespace lms
