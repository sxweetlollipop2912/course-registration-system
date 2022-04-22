#pragma once

#include <ctime>

#include "../Enums.h"
#include "Database.h"
#include "List.h"
#include "RegistrationSession.h"

using std::tm, std::string;

class Semester : public Data {
public:
    DataIter school_year;
    RegistrationSession reg_session{};
    int no{};
    tm start{}, end{};
    List<DataIter> courses;

    Semester() = default;
    Semester(const int no, const tm &start, const tm &end) : no{no}, start{start}, end{end} {}

    /// Returns empty DataIter if no course is found.
    DataIter getCourse(const string &course_id);
    DataIter getCourse(const Data::UID &uid);
    /// False if a course with the same course_id is already added, otherwise true.
    bool addCourse(const DataIter &course);
    /// False if no course with such UID is found, otherwise true.
    bool removeCourse(const Data::UID &course_uid);
    /// False if no couse with such couse_id is found, otherwise true.
    bool removeCourse(const string &course_id);
    void sortCourse();
};