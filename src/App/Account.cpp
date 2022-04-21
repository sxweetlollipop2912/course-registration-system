#include <algorithm>

#include "Account.h"
#include "Utils.h"

using std::dynamic_pointer_cast, std::make_shared, std::min, std::stoi, std::exception;


Student Student::tryParse(const List<string> &headers, const List<string> &row) {
    string student_id = {}, social_id = {};
    FullName name = {};
    Gender gender = Gender::Unknown;
    tm birth = {};

    try {
        for (int i = 0; i < min(headers.size(), row.size()); i++) {
            auto header = headers[i];
            auto data = row[i];

            if (header.find("student") != string::npos && header.find("id") != string::npos) {
                student_id = data;
            }

            else if (header.find("social") != string::npos) {
                social_id = data;
            }

            else if (header.find("name") != string::npos && header.find("last") != string::npos) {
                name.last = data;
            }

            else if (header.find("name") != string::npos && header.find("first") != string::npos) {
                name.first = data;
            }

            else if (header.find("gender") != string::npos) {
                Utils::toLowerStr(data);
                if (data == "male")
                    gender = Gender::Male;
                else if (data == "female")
                    gender = Gender::Female;
                else if (data == "other")
                    gender = Gender::Other;
            }

            else if (header.find("birth") != string::npos) {
                for(int pos = (int)header.find(' '); pos != string::npos; pos = (int)header.find(' '))
                    header.erase(pos, 1);

                char sep[] = { '\\','-','.'};
                for(const auto &c : sep) {
                    int s1 = (int)header.find(c), s2 = (int)header.find(c, s1 + 1);
                    if (s1 == string::npos || s2 == string::npos)
                        continue;

                    try {
                        int day = stoi(header.substr(0, s1));
                        int month = stoi(header.substr(s1 + 1, s2 - s1 - 1));
                        int year = stoi(header.substr(s2 + 1, header.size() - s2 - 1));
                        birth = Utils::mktm(day, month, year);

                        break;
                    }
                    catch (exception &e) {}
                }
            }
        }
    }
    catch (exception &e) {
        return {};
    }

    return { student_id, social_id, name, gender, birth };
}


shared_ptr<Score> Student::addScore(const Score &score) {
    if (getScore(score.course.uid()))
        return nullptr;

    scores.push_back(make_shared<Score>(score));

    return scores.back();
}

bool Student::removeScore(const Data::UID &course_uid) {
    auto it = scores.find_if([&](const shared_ptr<Score> &score) {
        return score->course.uid() == course_uid;
    });

    if (it != scores.end()) {
        scores.remove(it);

        return true;
    }

    return false;
}

List<DataIter> Student::overlappingCourses(const List<Course::Session> &sessions) const {
    return courses.filter([&](const DataIter &iter) {
        auto course_ptr = iter.ptr<Course>();

        return course_ptr->sessions.any_of([&](const Course::Session &s1) {
            return sessions.any_of([&](const Course::Session &s2) {
                if (s1 == s2)
                    return true;

                return s1.inRange(s2.start) || s1.inRange(s2.end) || s2.inRange(s1.start) || s2.inRange(s1.end);
            });
        });
    });
}

bool Student::addCourse(const DataIter &course) {
    /// If course is already enrolled.
    if (courses.find(course) != courses.end()) return false;

    courses.push_back(course);
    scores.push_back(make_shared<Score>(course));

    return true;
}

bool Student::removeCourse(const Data::UID &course_uid) {
    auto it = courses.find_if([&](const DataIter &iter) {
        return iter.uid() == course_uid;
    });

    if (it != courses.end()) {
        removeScore(course_uid);
        courses.remove(it);

        return true;
    }

    return false;
}

shared_ptr<Score> Student::replaceScore(const Score &score) {
    removeScore(score.course.uid());
    scores.push_back(make_shared<Score>(score));

    return scores.back();
}

shared_ptr<Score> Student::getScore(const Data::UID &course_uid) {
    /// Check if score of that course already exists.
    auto it = scores.find_if([&](const shared_ptr<Score> &score) {
        return course_uid == score->course.uid();
    });

    if (it == scores.end()) return nullptr;

    return *it;
}

shared_ptr<Score> Student::getScore(const string &course_id) {
    /// Check if score of that course already exists.
    auto it = scores.find_if([&](const shared_ptr<Score> &score) {
        return course_id == score->course.ptr<Course>()->id;
    });

    if (it == scores.end()) return nullptr;

    return *it;
}

shared_ptr<const Score> Student::getScore(const Data::UID &course_uid) const {
    /// Check if score of that course already exists.
    auto it = scores.find_if([&](const shared_ptr<Score> &score) {
        return course_uid == score->course.uid();
    });

    if (it == scores.end()) return nullptr;

    return *it;
}

shared_ptr<const Score> Student::getScore(const string &course_id) const {
    /// Check if score of that course already exists.
    auto it = scores.find_if([&](const shared_ptr<Score> &score) {
        return course_id == score->course.ptr<Course>()->id;
    });

    if (it == scores.end()) return nullptr;

    return *it;
}

bool Account::checkPassword(const string &input) const {
    return input == password;
}

UserType Account::getUserType() const {
    return user_type;
}