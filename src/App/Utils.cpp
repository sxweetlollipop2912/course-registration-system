#include <random>

#include "Utils.h"

using std::chrono::system_clock, std::mt19937_64, std::random_device, std::uniform_int_distribution;

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

tm Utils::mksession(const int weekday, const int hour, const int minute) {
    tm time{};

    time.tm_hour = hour;
    time.tm_min = minute;
    time.tm_sec = 0;
    time.tm_mday = weekday == 1 ? 7 : weekday - 1;
    time.tm_mon = 0;
    time.tm_year = 0;
    auto mk = mktime(&time);

    return *localtime(&mk);
}

string Utils::tmToStr(const tm &time) {
    tm tmp = time;
    mktime(&tmp);
    string s = asctime(&tmp); // Mon Jan  1 00:00:00 1900\n

    auto pos = s.find("  ");
    if (pos != string::npos) s.erase(pos, 1);

    s.pop_back();

    // Mon Jan 1 00:00:00 1900
    return s;
}

string Utils::sessionToStr(const tm &session) {
    tm tmp = session;
    mktime(&tmp);
    string s = asctime(&tmp); // Mon Jan  1 00:00:00 1900\n

    auto pos = s.find("Jan");
    if (pos != string::npos) s.erase(pos, 7);

    pos = s.find(":00 1900");
    if (pos != string::npos) s.erase(pos, 8);
    else {
        pos = s.find("1900");
        if (pos != string::npos) s.erase(pos, 4);
    }

    s.pop_back();

    // Mon 00:00
    return s;
}

int Utils::random(const int min, const int max) {
    auto rng = mt19937_64(random_device()());
    auto dis = uniform_int_distribution<int>(min, max);

    return dis(rng);
}