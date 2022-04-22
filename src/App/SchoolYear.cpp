#include "SchoolYear.h"
#include "Semester.h"
#include "Class.h"


DataIter SchoolYear::getSemester(const int no) {
    auto it = semesters.find_if([&](const DataIter &iter) {
        auto semesterOnList = iter.ptr<Semester>();

        if (no == semesterOnList->no) return true;

        return false;
    });

    if (it != semesters.end()) return *it;
    return {};
}

DataIter SchoolYear::getSemester(const Data::UID &uid) {
    auto it = semesters.find_if([&](const DataIter &iter) {
        return uid == iter.uid();
    });

    if (it != semesters.end()) return *it;
    return {};
}

bool SchoolYear::addSemester(const DataIter &semester) {
    /// Check by semester no. if semester is already added.
    auto no = semester.ptr<Semester>()->no;
    if (getSemester(no).empty()) {
        semesters.push_back(semester);

        return true;
    }

    return false;
}

bool SchoolYear::removeSemester(const Data::UID &semester_uid) {
    auto it = semesters.find_if([&](const DataIter &iter) {
        return iter.uid() == semester_uid;
    });
    if (it != semesters.end()) {
        semesters.remove(it);

        return true;
    }

    return false;
}


bool SchoolYear::removeSemester(const int no) {
    auto it = semesters.find_if([&](const DataIter &iter) {
        auto semesterOnList = iter.ptr<Semester>();

        return no == semesterOnList->no;
    });
    if (it != semesters.end()) {
        semesters.remove(it);

        return true;
    }

    return false;
}
