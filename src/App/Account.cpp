#include <algorithm>
#include <stdexcept>
#include <iostream>

#include "Account.h"
#include "Utils.h"

using std::dynamic_pointer_cast, std::make_shared, std::min, std::stoi, std::exception, std::cerr;


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

List<DataIter> Student::getOverlappingCourses(const List<Course::Session> &sessions) const {
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

List<DataIter> Student::getCoursesInSemester(const Data::UID &semester_uid) {
    return courses.filter([&](const DataIter &iter) {
        auto course_ptr = iter.ptr<Course>();

        return course_ptr->semester.uid() == semester_uid;
    });
}

DataIter Student::getCourse(const string &course_id) {
    auto it = courses.find_if([&](const DataIter &ref) {
        return course_id == ref.ptr<Course>()->id;
    });

    if (it != courses.end()) return *it;
    return {};
}

DataIter Student::getCourse(const Data::UID &uid) {
    auto it = courses.find_if([&](const DataIter &ref) {
        return uid == ref.uid();
    });

    if (it != courses.end()) return *it;
    return {};
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

bool Student::valid() const {
    return !student_id.empty() && !social_id.empty();
}

bool Account::checkPassword(const string &input) const {
    return input == password;
}

UserType Account::getUserType() const {
    return user_type;
}