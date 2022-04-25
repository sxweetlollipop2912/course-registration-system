#pragma once

#include <ctime>

#include "../Enums.h"
#include "Database.h"
#include "Utils.h"

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

    friend std::ostream& operator<<(std::ostream &os, const RegistrationSession &obj) {
        os << Utils::tmToStr(obj.start) << '\n';
        os << Utils::tmToStr(obj.end) << '\n';

        return os;
    }

    friend std::istream& operator>>(std::istream &is, RegistrationSession &obj) {
        string s;
        Utils::getline(is, s);
        obj.start = Utils::strToTm(s);
        Utils::getline(is, s);
        obj.end = Utils::strToTm(s);

        return is;
    }
};