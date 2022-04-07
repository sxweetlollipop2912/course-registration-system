#include "Account.h"

using std::dynamic_pointer_cast, std::make_shared;


shared_ptr<CourseScore> Student::addScore(const CourseScore &score) {
    if (getScore(score.course.uid()))
        return {};

    scores.push_back(make_shared<CourseScore>(score));

    return scores.back();
}

bool Student::removeScore(const Data::UID &course_uid) {
    auto it = scores.find_if([&](const shared_ptr<CourseScore> &score) {
        return score->course.uid() == course_uid;
    });

    if (it != scores.end()) {
        scores.remove(it);

        return true;
    }

    return false;
}

bool Student::addCourse(const DataIter &course) {
    /// If course is already enrolled.
    if (courses.find(course) != courses.end()) return false;

    /// TODO: Check for overlapping course sessions.

    courses.push_back(course);
    scores.push_back(make_shared<CourseScore>(course));

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

shared_ptr<CourseScore> Student::replaceScore(const CourseScore &score) {
    removeScore(score.course.uid());
    scores.push_back(make_shared<CourseScore>(score));

    return scores.back();
}

shared_ptr<CourseScore> Student::getScore(const Data::UID &course_uid) {
    /// Check if score of that course already exists.
    auto it = scores.find_if([&](const shared_ptr<CourseScore> &score) {
        return course_uid == score->course.uid();
    });

    if (it == scores.end()) return {};

    return *it;
}

shared_ptr<const CourseScore> Student::getScore(const Data::UID &course_uid) const {
    /// Check if score of that course already exists.
    auto it = scores.find_if([&](const shared_ptr<CourseScore> &score) {
        return course_uid == score->course.uid();
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