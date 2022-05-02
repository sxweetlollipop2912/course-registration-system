#include "Class.h"
#include "Account.h"

DataIter Class::getStudent(const string &student_id) {
    auto it = students.find_if([&](const DataIter &ref) {
        auto studentOnList = ref.ptr<Student>();

        if (student_id == studentOnList->student_id) return true;

        return false;
    });

    if (it != students.end()) return *it;
    return {};
}

bool Class::addStudent(const DataIter &student) {
    /// Check by student_id if this student is not already added.
    auto student_id = student.ptr<Student>()->student_id;
    if (getStudent(student_id).empty()) {
        students.push_back(student);

        return true;
    }
    return false;
}

bool Class::removeStudent(const Data::UID &student_uid) {
    auto it = students.find_if([&](const DataIter &iter) {
        return iter.uid() == student_uid;
    });
    if (it != students.end()) {
        students.remove(it);

        return true;
    }

    return false;
}

bool Class::removeStudent(const string &student_id) {
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
    students.sort([](const DataIter &ref1, const DataIter &ref2) {
        auto id1 = ref1.ptr<Student>()->student_id;
        auto id2 = ref2.ptr<Student>()->student_id;
        return id1 < id2;
    });
}

void Class::load(Database &database) {
    for (auto &e: students)
        e = database.get(e.uid());
}