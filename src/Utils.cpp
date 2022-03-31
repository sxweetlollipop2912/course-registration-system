#include "Utils.h"

using std::chrono::system_clock;

tm Utils::now() {
    auto now = system_clock::to_time_t(system_clock::now());
    return *localtime(&now);
}

tm Utils::mktm(const int day, const int month, const int year, const int hour, const int minute, const int second) {
    tm time{};
    time.tm_mday = day;
    time.tm_mon = month - 1;
    time.tm_year = year - 1900;
    time.tm_hour = hour;
    time.tm_min = minute;
    time.tm_sec = second;
    auto mk = mktime(&time);

    return *localtime(&mk);
}