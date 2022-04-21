#include "Account.h"

using std::dynamic_pointer_cast, std::make_shared;


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

bool Student::addCourse(const DataIter &course) {
    /// If course is already enrolled.
    if (courses.find(course) != courses.end()) return false;

    /// TODO: Check for overlapping course sessions.

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