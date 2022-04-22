#pragma once

#include <ctime>
#include <chrono>
#include <string>

using std::tm, std::string;

namespace Utils {
    /// Returns std::tm of current time.
    tm now();
    /// Returns std::tm of desired time.
    tm mktm(const int day, const int month, const int year, const int hour = 0, const int minute = 0, const int second = 0);
    /// Returns std::tm of desired time.\n
    /// NOTE: Sunday to Saturday is 1 to 7.
    tm mksession(const int weekday, const int hour, const int minute);
    /// Convert std::tm to std::string for output.\n
    /// Ex: Mon Jan 1 00:00:00 1900
    string tmToStr(const tm &time);
    /// Convert std::tm to std::string for output (session syntax).\n
    /// Ex: Mon 00:00
    string sessionToStr(const tm &session);
    /// Convert std::tm to std::string for output.\n
    /// Ex: Jan 1 1900
    string dateToStr(const tm &date);
    /// Returns a random integer from [min, max].
    int random(const int min, const int max);
    void toLowerStr(string &s);
    /// Trims spaces at the beginning and end.\n
    /// Reduces consecutive spaces into one.
    void trimStr(string &s);
}