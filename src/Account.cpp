#include "Account.h"
#include "Course.h"

using std::dynamic_pointer_cast, std::make_shared;


bool Student::enroll(const DataIter &course) {
    /// If course is already enrolled.
    if (courses.find(course) != courses.end()) return false;

    /// Check for overlapping course sessions.
    /// here

    courses.push_back(course);
    return true;
}

bool Student::disenroll(const DataIter& course) {
    auto it = courses.find(course);
    if (it != courses.end()) {
        courses.remove(it);

        return true;
    }

    return false;
}

bool Student::setScore(const DataIter &score) {
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

bool Student::removeScore(const DataIter &score) {
    auto it = scores.find(score);
    if (it != scores.end()) {
        scores.remove(it);

        return true;
    }

    return false;
}

DataIter Student::getScore(const DataIter &course) const {
    /// Check if score of that course already exists.
    auto it = scores.find_if([&](const DataIter& ref) {
        auto score = ref.ptr<CourseScore>();

        if (course == score->course) return true;

        return false;
    });

    if (it == scores.end()) return {};

    return *it;
}

bool Account::checkPassword(const string &input) const {
    return input == password;
}
