#include "SchoolYear.h"
#include "Semester.h"
#include "Class.h"


DataIter SchoolYear::getSemesterByNo(const int no) {
    auto it = semesters.find_if([&](const DataIter &ref) {
        auto semesterOnList = ref.ptr<Semester>();

        if (no == semesterOnList->no) return true;

        return false;
    });

    if (it != semesters.end()) return *it;
    return {};
}

bool SchoolYear::addSemester(const DataIter &semester) {
    /// Check by semester no. if semester is already added.
    auto no = semester.ptr<Semester>()->no;
    if (getSemesterByNo(no).empty()) {
        semesters.push_back(semester);

        return true;
    }

    return false;
}

bool SchoolYear::removeSemester(const DataIter &semester) {
    auto it = semesters.find(semester);
    if (it != semesters.end()) {
        semesters.remove(it);

        return true;
    }

    return false;
}


bool SchoolYear::removeSemester(const int no) {
    auto it = semesters.find_if([&](const DataIter &ref) {
        auto semesterOnList = ref.ptr<Semester>();

        if (no == semesterOnList->no) return true;

        return false;
    });
    if (it != semesters.end()) {
        semesters.remove(it);

        return true;
    }

    return false;
}


DataIter SchoolYear::getClassByName(const string &name) {
    auto it = classes.find_if([&](const DataIter &ref) {
        auto classOnList = ref.ptr<Class>();

        if (name == classOnList->name) return true;

        return false;
    });

    if (it != classes.end()) return *it;
    return {};
}

bool SchoolYear::addClass(const DataIter &classroom) {
    /// Check by semester no. if semester is already added.
    auto name = classroom.ptr<Class>()->name;
    if (getClassByName(name).empty()) {
        classes.push_back(classroom);

        return true;
    }

    return false;
}

bool SchoolYear::removeClass(const DataIter &classroom) {
    auto it = classes.find(classroom);
    if (it != classes.end()) {
        classes.remove(it);

        return true;
    }

    return false;
}


bool SchoolYear::removeClass(const string &name) {
    auto it = classes.find_if([&](const DataIter &ref) {
        auto classOnList = ref.ptr<Class>();

        if (name == classOnList->name) return true;

        return false;
    });
    if (it != classes.end()) {
        classes.remove(it);

        return true;
    }

    return false;
}
