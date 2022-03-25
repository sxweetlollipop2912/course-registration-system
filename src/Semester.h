#pragma once

#include <ctime>

#include "Enums.h"
#include "Database.h"
#include "List.h"

using std::tm, std::string;

class Semester : public Data {
public:
    int no;
    tm start, end;
    List<DataIter> courses;

    Semester() = default;
    Semester(const int no, const tm &start, const tm &end) : no{no}, start{start}, end{end} {}

    /// Returns empty DataIter if no course is found.
    DataIter getCourseByID(const string course_id);
    /// False if a course with the same course_id is already added, otherwise true.
    bool addCourse(const DataIter &course);
    /// False if no such course is found, otherwise true.
    bool removeCourse(const DataIter &course);
    /// False if no couse with such couse_id is found, otherwise true.
    bool removeCourse(const string course_id);
    void sortCourse();
};