#include "Account.h"
#include "Course.h"

using std::dynamic_pointer_cast, std::make_shared;


bool Student::addCourse(const DataIter &course) {
    /// If course is already enrolled.
    if (courses.find(course) != courses.end()) return false;

    /// Check for overlapping course sessions.
    /// here

    courses.push_back(course);
    return true;
}

bool Student::removeCourse(const Data::UID &course_uid) {
    auto it = courses.find_if([&](const DataIter &iter) {
        return iter.uid() == course_uid;
    });
    if (it != courses.end()) {
        courses.remove(it);

        return true;
    }

    return false;
}

bool Student::addScore(const DataIter &score) {
    /// Check if score of that course already exists.
    if (scores.any_of([&](const DataIter& ref) {
        auto scoreToSet = score.ptr<CourseScore>();
        auto scoreAlreadySet = score.ptr<CourseScore>();

        if (scoreToSet->course == scoreAlreadySet->course) return true;

        return false;
    }))
        return false;

    scores.push_back(score);
    return true;
}

bool Student::removeScore(const Data::UID &score_uid) {
    auto it = scores.find_if([&](const DataIter &iter) {
        return iter.uid() == score_uid;
    });
    if (it != scores.end()) {
        scores.remove(it);

        return true;
    }

    return false;
}

DataIter Student::getScore(const Data::UID &course_uid) const {
    /// Check if score of that course already exists.
    auto it = scores.find_if([&](const DataIter& ref) {
        auto score = ref.ptr<CourseScore>();

        if (course_uid == score->course.uid()) return true;

        return false;
    });

    if (it == scores.end()) return {};

    return *it;
}

bool Account::checkPassword(const string &input) const {
    return input == password;
}

UserType Account::getUserType() const {
    return user_type;
}