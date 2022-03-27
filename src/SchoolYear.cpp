#include "SchoolYear.h"
#include "Semester.h"
#include "Class.h"


DataIter SchoolYear::getSemesterByNo(const int no) {
    auto it = semesters.find_if([&](const DataIter &iter) {
        auto semesterOnList = iter.ptr<Semester>();

        if (no == semesterOnList->no) return true;

        return false;
    });

    if (it != semesters.end()) return *it;
    return {};
}

DataIter SchoolYear::getSemesterByUID(const Data::UID &uid) {
    auto it = semesters.find_if([&](const DataIter &iter) {
        return uid == iter.uid();
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


DataIter SchoolYear::getClassByName(const string &name) {
    auto it = classes.find_if([&](const DataIter &iter) {
        auto classOnList = iter.ptr<Class>();

        return name == classOnList->name;
    });

    if (it != classes.end()) return *it;
    return {};
}

DataIter SchoolYear::getClassByUID(const Data::UID &uid) {
    auto it = classes.find_if([&](const DataIter &iter) {
        return uid == iter.uid();
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

bool SchoolYear::removeClass(const Data::UID &class_uid) {
    auto it = classes.find_if([&](const DataIter &iter) {
        return iter.uid() == class_uid;
    });
    if (it != classes.end()) {
        classes.remove(it);

        return true;
    }

    return false;
}


bool SchoolYear::removeClass(const string &name) {
    auto it = classes.find_if([&](const DataIter &iter) {
        auto classOnList = iter.ptr<Class>();

        return name == classOnList->name;
    });
    if (it != classes.end()) {
        classes.remove(it);

        return true;
    }

    return false;
}
