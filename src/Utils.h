#pragma once

#include <ctime>
#include <chrono>

using std::tm;

namespace Utils {
    /// Returns std::tm of current time.
    tm now();
    /// Returns std::tm of desired time.
    tm mktm(const int day, const int month, const int year, const int hour = 0, const int minute = 0, const int second = 0);
}