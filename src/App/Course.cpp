#include <iostream>

#include "Course.h"
#include "Account.h"

using std::shared_ptr, std::min, std::exception, std::stod, std::cerr;


DataIter Course::getStudent(const string &student_id) {
    auto it = students.find_if([&](const DataIter &ref) {
        auto studentOnList = ref.ptr<Student>();

        if (student_id == studentOnList->student_id) return true;

        return false;
    });

    if (it != students.end()) return *it;
    return {};
}

void Course::sortStudentsByID() {
    students.sort([](const DataIter &s1, const DataIter &s2) {
        return s1.ptr<Student>()->student_id < s2.ptr<Student>()->student_id;
    });
}

bool Course::addStudent(const DataIter &student) {
    /// Check by student_id if this student is not already added.
    auto student_id = student.ptr<Student>()->student_id;
    if (getStudent(student_id).empty()) {
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

bool Course::Session::inRange(const tm &time) const {
    tm tmp = time, tmp_st = start, tmp_en = end;
    return mktime(&tmp_st) < mktime(&tmp) && mktime(&tmp_en) > mktime(&tmp);
}

bool Course::Session::operator==(const Session &s) const {
    tm s_st = s.start, s_en = s.end, tmp_st = start, tmp_en = end;
    return mktime(&s_st) == mktime(&tmp_st) && mktime(&s_en) == mktime(&tmp_en);
}

void Course::load(Database &database) {
    semester = database.get(semester.uid());

    for(auto &e : students)
        e = database.get(e.uid());
}
