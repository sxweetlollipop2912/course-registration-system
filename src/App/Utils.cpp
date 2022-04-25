#include <random>
#include <sstream>
#include <iostream>
#include <exception>

#include "Utils.h"

#pragma warning(disable:4996)

using std::chrono::system_clock, std::mt19937_64, std::random_device, std::uniform_int_distribution, std::to_string;

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
	mktime(&time);

	return time;
}

tm Utils::mksession(const int weekday, const int hour, const int minute) {
	tm time{};

	time.tm_hour = hour;
	time.tm_min = minute;
	time.tm_sec = 0;
	time.tm_mday = weekday == 1 ? 7 : weekday - 1;
	time.tm_mon = 0;
	time.tm_year = 0;
	mktime(&time);

	return time;
}

tm Utils::strToDate(const string &s) {
    // 01/01/1900
    tm time{};

    char sep[] = { '/','-','.',',','\\' };
    for(auto c : sep) {
        try {
            int date[] = {0, 0, 0};
            auto s1 = s + c;
            for (int pos = (int)s1.find(' '); pos != string::npos; pos = (int)s1.find(' '))
                s1.erase(pos, 1);

            for (int pos = (int) s1.find(c), i = 0; pos != string::npos && i < 3; pos = (int) s1.find(c), i++) {
                auto num = s1.substr(0, pos);
                s1.erase(0, pos + 1);
                date[i] = std::stoi(num);
            }
            time.tm_mday = date[0];
            time.tm_mon = date[1];
            time.tm_year = date[2];

            time.tm_mon -= 1;
            time.tm_year -= 1900;
            mktime(&time);

            break;
        }
        catch (std::exception &e) {}
    }

    return time;
}

tm Utils::strToSession(const string &s) {
    // Mon 00:00
    tm time{};

    try {
        std::stringstream ss(s);
        string wday;
        ss >> wday;

        string clock;
        ss >> clock;

        string wday_s[] = {"mon", "tue", "wed", "thu", "fri", "sat", "sun" };
        Utils::toLowerStr(wday);
        for (int i = 0; i < 7; i++)
            if (wday == wday_s[i]) {
                time.tm_mday = i + 1;
                break;
            }

        int pos = (int)clock.find(':');
        if (pos != string::npos) time.tm_hour = std::stoi(clock.substr(0, pos));
        int pos2 = (int)clock.size();
        time.tm_min = std::stoi(clock.substr(pos + 1, pos2 - pos - 1));

        time.tm_sec = 0;
        mktime(&time);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return time;
}

string Utils::tmToStr(const tm &time) {
	tm tmp = time;
	mktime(&tmp);
	string s = asctime(&tmp); // Mon Jan  1 00:00:00 1900\n
    std::cout << "to str " << s << '\n';

	Utils::trimStr(s);
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

string Utils::dateToStr(const tm &date) {
	tm tmp = date;
	mktime(&tmp);

	string s;
	s += (tmp.tm_mday < 10 ? "0" : "") + to_string(tmp.tm_mday) + (char)'/';
	s += (tmp.tm_mon + 1 < 10 ? "0" : "") + to_string(tmp.tm_mon + 1) + (char)'/';
	s += to_string(tmp.tm_year + 1900);

	// 01/01/1900
	return s;
}

void Utils::toLowerStr(string &s) {
	std::transform(s.begin(), s.end(), s.begin(),
		[](char c) -> char {
		if (c <= 'Z' && c >= 'A')
			return (char)(c - ('Z' - 'z'));
		return c;
	});
}

void Utils::trimStr(string &s) {
	int st = 0;
	while (st < (int)s.size() && s[st] == ' ') ++st;
	s.erase(0, st);

	int en = (int)s.size() - 1;
	while (en >= 0 && s[en] == ' ') --en;
	s.erase(en + 1, s.size());

	for (int pos = (int)s.find("  "); pos != string::npos; pos = (int)s.find("  "))
		s.erase(pos, 1);
}

tm Utils::strToTm(const string &s) {
	// Mon Jan 1 00:00:00 1900
	tm time{};

    try {
        std::stringstream ss(s);
        string wday;
        ss >> wday;
        string tm_mon;
        ss >> tm_mon;

        ss >> time.tm_mday;

        string clock;
        ss >> clock;

        ss >> time.tm_year;

        string mon[] = {"jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec"};
        Utils::toLowerStr(tm_mon);
        for (int i = 0; i < 12; i++)
            if (tm_mon == mon[i]) {
                time.tm_mon = i;
                break;
            }

        int pos = (int)clock.find(':');
        if (pos != string::npos) time.tm_hour = std::stoi(clock.substr(0, pos));
        int pos2 = (int)clock.find(':', pos + 1);
        if (pos2 != string::npos) time.tm_min = std::stoi(clock.substr(pos + 1, pos2 - pos - 1));
        int pos3 = (int)clock.size();
        time.tm_sec = std::stoi(clock.substr(pos2 + 1, pos3 - pos2 - 1));

        time.tm_year -= 1900;
        mktime(&time);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

	return time;
}

int Utils::random(const int min, const int max) {
	auto rng = mt19937_64(random_device()());
	auto dis = uniform_int_distribution<int>(min, max);

	return dis(rng);
}

bool Utils::getline(std::istream &is, string &s) {
	while (std::getline(is, s) && s.empty());

	return !s.empty();
}