#include "Semester.h"
#include "Course.h"

DataIter Semester::getCourseByID(const string &course_id) {
    auto it = courses.find_if([&](const DataIter &ref) {
        auto courseOnList = ref.ptr<Course>();

        if (course_id == courseOnList->id) return true;

        return false;
    });

    if (it != courses.end()) return *it;
    return {};
}

bool Semester::addCourse(const DataIter &course) {
    /// Check by course_id if this course is not already added.
    auto course_id = course.ptr<Course>()->id;
    if (getCourseByID(course_id).empty()) {
        courses.push_back(course);

        return true;
    }

    return false;
}

bool Semester::removeCourse(const DataIter &course) {
    auto it = courses.find(course);
    if (it != courses.end()) {
        courses.remove(it);

        return true;
    }

    return false;
}

bool Semester::removeCourse(const string &course_id) {
    auto it = courses.find_if([&](const DataIter &ref) {
        auto courseOnList = ref.ptr<Course>();

        if (course_id == courseOnList->id) return true;

        return false;
    });
    if (it != courses.end()) {
        courses.remove(it);

        return true;
    }

    return false;
}

void Semester::sortCourse() {
    courses.sort([](const DataIter& ref1, const DataIter& ref2) {
        auto id1 = ref1.ptr<Course>()->id;
        auto id2 = ref2.ptr<Course>()->id;
        return id1 < id2;
    });
}