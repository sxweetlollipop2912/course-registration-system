#pragma once

#include <ctime>

#include "Enums.h"
#include "Database.h"

using std::tm;

class RegistrationSession {
public:
    /// Marks the first day of the session.
    tm start;
    /// Marks the day after the session ends.
    tm end;

    RegistrationSession() = default;
    RegistrationSession(const tm &start, const tm &end) : start{start}, end{end} {}

    /// True if session is ongoing given current date, otherwise false.
    bool isOngoing();
};