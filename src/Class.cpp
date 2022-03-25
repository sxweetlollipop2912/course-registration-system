#include "Class.h"
#include "Account.h"

DataIter Class::getStudentByID(const int student_id) {
    auto it = students.find_if([&](const DataIter &ref) {
        auto studentOnList = ref.ptr<Student>();

        if (student_id == studentOnList->student_id) return true;

        return false;
    });

    if (it == students.end()) return *it;
    return {};
}

bool Class::addStudent(const DataIter &student) {
    /// Check by student_id if this student is not already added.
    auto student_id = student.ptr<Student>()->student_id;
    if (getStudentByID(student_id).empty()) {
        students.push_back(student);

        return true;
    }
    return false;
}

bool Class::removeStudent(const DataIter &student) {
    auto it = students.find(student);
    if (it != students.end()) {
        students.remove(it);

        return true;
    }

    return false;
}

bool Class::removeStudent(const int student_id) {
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

void Class::sortStudent() {
    students.sort([](const DataIter& ref1, const DataIter& ref2) {
        auto id1 = ref1.ptr<Student>()->student_id;
        auto id2 = ref2.ptr<Student>()->student_id;
        return id1 < id2;
    });
}