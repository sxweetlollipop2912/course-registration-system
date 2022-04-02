#include "Course.h"
#include "Account.h"

using std::shared_ptr;


DataIter Course::getStudentByID(const string &student_id) {
    auto it = students.find_if([&](const DataIter &ref) {
        auto studentOnList = ref.ptr<Student>();

        if (student_id == studentOnList->student_id) return true;

        return false;
    });

    if (it != students.end()) return *it;
    return {};
}

bool Course::addStudent(const DataIter &student) {
    /// Check by student_id if this student is not already added.
    auto student_id = student.ptr<Student>()->student_id;
    if (getStudentByID(student_id).empty()) {
        students.push_back(student);

        return true;
    }

    return false;
}

bool Course::removeStudent(const Data::UID& student_uid) {
    auto it = students.find_if([&](const DataIter &iter) {
        return iter.uid() == student_uid;
    });
    if (it != students.end()) {
        students.remove(it);

        return true;
    }

    return false;
}

bool Course::removeStudent(const string &student_id) {
    auto it = students.find_if([&](const DataIter &ref) {
        auto studentOnList = ref.ptr<Student>();

        if (student_id == studentOnList->student_id) return true;

        return false;
    });
    if (it != students.end()) {
        students.remove(it);

        return true;
    }

    return false;
}

