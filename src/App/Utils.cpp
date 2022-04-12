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

int Utils::random(const int min, const int max) {
    auto rng = mt19937_64(random_device()());
    auto dis = uniform_int_distribution<int>(min, max);

    return dis(rng);
}