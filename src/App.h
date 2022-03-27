#pragma once

#include "List.h"
#include "Database.h"
#include "SchoolYear.h"
#include "Semester.h"
#include "Course.h"
#include "Class.h"
#include "Account.h"

using std::shared_ptr, std::string;

class App {
private:
    /// For loading purposes, since year and semester pointers cannot be saved.
    Data::UID default_year_uid, default_semester_uid;
public:
    Database database;
    /// default year
    DataIter default_year;
    /// default semester
    DataIter default_semester;

    App() {
        default_year = database.getByUID(default_year_uid);
        default_semester = database.getByUID(default_semester_uid);
    }

    shared_ptr<SchoolYear> year() {return default_year.ptr<SchoolYear>();}
    shared_ptr<Semester> semester() {return default_semester.ptr<Semester>();}

    void load();
    void save();

    /// All methods below work on default_year and default_semester,
    /// unless year or semester is replaced by method's parameters.

    /// False if there is no existing schoolyear with such UID in database.
    bool setDefaultSchoolYear(const Data::UID &year_uid);
    /// False if there is no existing schoolyear with such start_year and end_year in database.
    bool setDefaultSchoolYear(const int start_year, const int end_year);
    /// False if there is no existing schoolyear with such UID in default schoolyear.
    bool setDefaultSemester(const Data::UID &semester_uid);
    /// False if there is no existing semester with such no. in default schoolyear.
    bool setDefaultSemester(const int no);

    /// False if another schoolyear with identical start_year and end_year is already added to database.
    bool addDefaultSchoolYear(const shared_ptr<SchoolYear> &year);
    /// False if another semester with the same no. is already added in default schoolyear.
    bool addDefaultSemester(const shared_ptr<Semester> &semester);
    /// Methods with `DataIter` returning types:
    /// If unsuccessful: returns empty DataIter. Otherwise, returns DataIter pointing to newly added data.
    DataIter addClass(const shared_ptr<Class> &classroom);
    DataIter addCourse(const shared_ptr<Course> &course);
    DataIter addStudent(const shared_ptr<Student> &student, const string &class_name);
    DataIter addStudent(const shared_ptr<Student> &student, const Data::UID &class_uid);

    /// Note: This method removes student entirely, even from previous schoolyear.
    bool removeStudent(const DataIter &student);
    bool removeCourse(const Data::UID &course_uid);
    /// Note: Removing a class also removes all students in that class.
    bool removeClassFromYear(const Data::UID &class_uid);
    /// Note: Removing a semester also removes all courses in that semester.
    bool removeSemester(const Data::UID &semester_uid);
    /// Removes default schoolyear.
    /// Note: Removing a schoolyear also removes all semesters in that schoolyear.
    void removeSchoolYear();

    bool enroll(const DataIter &student, const Data::UID &course_uid);
    bool enroll(const DataIter &student, const string &course_id);
    bool disenroll(const DataIter &student, const Data::UID &course_uid);
    bool disenroll(const DataIter &student, const string &course_id);
};