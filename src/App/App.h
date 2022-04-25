#pragma once

#include "List.h"
#include "Database.h"
#include "SchoolYear.h"
#include "Semester.h"
#include "Course.h"
#include "Class.h"
#include "Account.h"
#include "CSV.h"
#include "../GUI/SceneManager.h"

using std::shared_ptr, std::string;

/// All methods work on default_year and default_semester,
/// unless default year or default semester is replaced by method's parameters.\n\n
///
/// Methods with DataIter returning types (adding methods):\n
/// If unsuccessful: returns empty DataIter. Otherwise, returns DataIter pointing to newly added data.
class App {
private:
    /// For loading purposes, since year and semester pointers cannot be saved.
    Data::UID default_year_uid, default_semester_uid;

    DataIter addAccount(const shared_ptr<Account> &account);

    void deleteSchoolYear(const DataIter &year);
    void deleteSemester(const DataIter &semester);

    /// Sets a schoolyear in database to default.\n
    /// Needs UID.\n\n
    /// True if succeeded, false if:\n
    /// > There is no existing schoolyear with such UID in database.
    bool setDefaultSchoolYear(const Data::UID &year_uid);
    /// Sets a school year in database to default.\n
    /// Needs start and end year of that schoolyear.\n\n
    /// True if succeeded, false if:\n
    /// > There is no existing schoolyear with such start_year and end_year in database.
    bool setDefaultSchoolYear(const int start_year, const int end_year);

    class CSV {
    public:
        static Score CSVToScore(const List<string> &headers, const List<string> &row, const DataIter &course);
        static Student CSVToStudent(const List<string> &headers, const List<string> &row);
        static void studentToCSV(const shared_ptr<Student> &student, CSVIO::CSVWriter &writer, const bool write_header = false);
        static void studentsToCSV(const List<DataIter> &students, CSVIO::CSVWriter &writer);
    };

public:
    Database database;
    DataIter default_year_iter;
    DataIter default_semester_iter;
    DataIter user_iter;

    SceneManager scenes;

    App() {
        user_iter = {};
        default_year_iter = database.get(default_year_uid);
        default_semester_iter = database.get(default_semester_uid);
    }

    bool isLoggedIn() const;
    /// Returns UserType of current user:\n
    /// > UserType::Student if current user is a student.\n
    /// > UserType::Staff if current user is a staff.\n
    /// > UserType::Unknown if no user has logged in.\n
    UserType userType() const;

    shared_ptr<Account> user() {return user_iter.ptr<Account>();}
    shared_ptr<SchoolYear> year() {return default_year_iter.ptr<SchoolYear>();}
    shared_ptr<Semester> semester() {return default_semester_iter.ptr<Semester>();}

    bool load();
    bool save();
    List<DataIter> getAllYears();
    List<DataIter> getAllClasses();

    /// Attempt to login given username and password.\n\n
    /// Returns DataIter to the Account if succeeded, or empty DataIter if:\n
    /// > Given username does not exist.\n
    /// > Given username and password don't match.\n
    /// NOTE: returning DataIter is the same as App.user_iter.\n
    DataIter login(const string &username, const string &password);
    /// Attempt to logout.\n\n
    /// True if succeeded, false if:\n
    /// > No user is logged in.\n
    bool logout();
    /// Attempt to change password.\n\n
    /// True if succeeded, false if:\n
    /// > No user is logged in.\n
    /// > ?\n
    bool changePassword(const string &password);
    /// Add a staff account.\n\n
    /// Returns DataIter to Staff if succeeded, or empty DataIter if:\n
    /// > username already exists.\n
    DataIter addStaff(const shared_ptr<Staff> &staff);

    /// Adds a new schoolyear to database.\n
    /// Needs a shared ptr of that schoolyear.\n\n
    /// True if succeeded, false if:\n
    /// > Another schoolyear with identical start_year and end_year already sits in database.
    bool addDefaultSchoolYear(const shared_ptr<SchoolYear> &year);
    /// Exits default schoolyear.\n\n
    /// True if succeeded, false if:\n
    /// > No default schoolyear has been set.
    bool exitDefaultSchoolYear();
    /// Deletes default schoolyear. Also deletes all semesters and courses in that schoolyear.\n\n
    /// True if succeeded, false if:\n
    /// > No default schoolyear has been set.
    bool deleteDefaultSchoolYear();


    /// Sets a semester in database to default.\n
    /// Needs UID.\n\n
    /// True if succeeded, false if:\n
    /// > There is no semester with such UID stored in default schoolyear.\n
    /// > Default schoolyear has not been set.
    bool setDefaultSemester(const Data::UID &semester_uid);
    /// Sets a semester already stored in default schoolyear to default.\n
    /// Needs no. of that semester.\n\n
    /// True if succeeded, false if:\n
    /// > There is no semester with such no. in default schoolyear.\n
    /// > Default schoolyear has not been set.
    bool setDefaultSemester(const int no);
    /// Adds a new semester to database and default schoolyear.\n
    /// Needs a shared ptr of that semester.\n\n
    /// True if succeeded, false if:\n
    /// > Another semester with the same no. already exists.\n
    /// > Default schoolyear has not been set.
    bool addDefaultSemester(const shared_ptr<Semester> &semester);
    /// Exits default semester.\n\n
    /// True if succeeded, false if:\n
    /// > No default semester has been set.
    bool exitDefaultSemester();
    /// Deletes default semester. Also deletes all courses in that semester.\n\n
    /// True if succeeded, false if:\n
    /// > No default semester has been set
    bool deleteDefaultSemester();

    /// Adds a new course to database and default semester.\n
    /// Needs a shared ptr of that course.\n\n
    /// Returns DataIter of the new course if succeeded, or empty DataIter if:\n
    /// > Another course with the same course_id already exists.\n
    /// > Default semester has not been set.
    DataIter addCourse(const shared_ptr<Course> &course);
    /// Deletes a course in default semester.\n
    /// Needs UID.\n\n
    /// True if succeeded, false if:\n
    /// > No course with such UID is found in default semester.\n
    /// > Default semester has not been set.
    bool deleteCourse(const Data::UID &course_uid);
    /// Deletes a course.\n
    /// Needs DataIter to the course.\n\n
    /// True if succeeded, false if:\n
    /// > DataIter is empty.
    bool deleteCourse(const DataIter &course);
    /// Exports a list of students of a course to a CSV file.\n
    /// Needs DataIter to the course, path of the CSV.
    void exportStudentsInCourse(const DataIter &course, const string &file_path);

    /// Gets a DataIter of a class by name.\n\n
    /// Returns empty DataIter if no class is found.
    DataIter getClass(const string &name);
    /// Gets a DataIter of a class by UID.\n\n
    /// Returns empty DataIter if no class is found.
    DataIter getClass(const Data::UID &uid);
    /// Adds a new class to database.\n
    /// Needs a shared ptr of that class.\n\n
    /// Returns DataIter of the new course if succeeded, or empty DataIter if:\n
    /// > Another class with the same name already exists.
    DataIter addClass(const shared_ptr<Class> &classroom);
    /// Removes all connections of a class from default schoolyear. Does not delete that class entirely.\n
    /// NOTE: This also removes the students from all courses belong to default schoolyear.\n
    /// Needs UID.\n\n
    /// True if succeeded, false if:\n
    /// > No class with such UID is found in default schoolyear.\n
    /// > Default schoolyear has not been set.
    bool removeClassFromYear(const Data::UID &class_uid);
    /// Removes all connections of a class from default schoolyear. Does not delete that class entirely.\n
    /// NOTE: This also removes the students from all courses belong to default schoolyear.\n
    /// Needs DataIter to the class.\n\n
    /// True if succeeded, false if:\n
    /// > DataIter is empty.\n
    /// > Default schoolyear has not been set.
    bool removeClassFromYear(const DataIter &classroom);
    /// Deletes a class entirely. Also deletes all students in that class.
    void deleteClass(const DataIter &classroom);

    /// Gets a DataIter of a student by studentID.\n\n
    /// Returns empty DataIter if no student is found.
    DataIter getStudent(const string &student_id);
    /// Adds students in a CSV file to database and to a class.\n
    /// Needs CSVData, DataIter class.\n\n
    /// Returns number of students successfully added. Adding fail when:\n
    /// > DataIter is empty.\n
    /// > Another student with the same student_id already exists in database.\n
    /// > Parsing fails.
    int addStudents(const CSVData &csv, const DataIter &classroom);
    /// Adds students in a CSV file to database and to a class.\n
    /// Needs CSVData, name of the class.\n\n
    /// Returns number of students successfully added. Adding fail when:\n
    /// > Another student with the same student_id already exists in database.\n
    /// > Parsing fails.
    int addStudents(const CSVData &csv, const string &class_name);
    /// Adds students in a CSV file to database and to a class.\n
    /// Needs CSVData, uid of the class.\n\n
    /// Returns number of students successfully added. Adding fail when:\n
    /// > Another student with the same student_id already exists in database.\n
    /// > Parsing fails.
    int addStudents(const CSVData &csv, const Data::UID &class_uid);
    /// Adds a new student to database and to a class.\n
    /// Needs a shared ptr of that student, DataIter to the class.\n\n
    /// Returns DataIter of the new course if succeeded, or empty DataIter if:\n
    /// > DataIter is empty.\n
    /// > Another student with the same student_id already exists in database.\n
    /// > Class is not found.
    DataIter addStudent(const shared_ptr<Student> &student, const DataIter &classroom);
    /// Adds a new student to database and to a class.\n
    /// Needs a shared ptr of that student, name of the class.\n\n
    /// Returns DataIter of the new course if succeeded, or empty DataIter if:\n
    /// > Another student with the same student_id already exists in database.\n
    /// > Class is not found.
    DataIter addStudent(const shared_ptr<Student> &student, const string &class_name);
    /// Adds a new student to database and to a class in default schoolyear.\n
    /// Needs a shared ptr of that student, uid of the class.\n\n
    /// Returns DataIter of the new course if succeeded, or empty DataIter if:\n
    /// > Another student with the same student_id already exists in database.\n
    /// > Class is not found.
    DataIter addStudent(const shared_ptr<Student> &student, const Data::UID &class_uid);
    /// Deletes a student.\n
    /// Note: This method deletes student from class and from all courses.\n
    /// Needs DataIter of that student.\n\n
    /// True if succeeded, false if:\n
    /// > DataIter is empty.\n
    /// > Student does not belong to any class.
    bool deleteStudent(const DataIter &student);
    /// Enrolls student to a course in default semester.\n
    /// Needs DataIter of that student, UID of the course.\n\n
    /// True if succeeded, false if:\n
    /// > DataIter is empty.\n
    /// > Course is not found in default semester.\n
    /// > Student has already enrolled in that course.\n
    /// > There are conflicts with enrolled courses' sessions.\n
    /// > Default semester has not been set.\n\n
    /// NOTE: call app.getOverlappingCourses to get a list of conflicting courses, if any.
    bool enroll(const DataIter &student, const Data::UID &course_uid);
    /// Enrolls student to a course in default semester.\n
    /// Needs DataIter of that student, course_id.\n\n
    /// True if succeeded, false if:\n
    /// > DataIter is empty.\n
    /// > Course is not found in default semester.\n
    /// > Student has already enrolled in that course.\n
    /// > There are conflicts with enrolled courses' sessions.\n
    /// > Default semester has not been set.\n\n
    /// NOTE: call app.getOverlappingCourses to get a list of conflicting courses, if any.
    bool enroll(const DataIter &student, const string &course_id);
    /// Disenrolls student from a course in default semester.\n
    /// Needs DataIter of that student, UID of the course.\n\n
    /// True if succeeded, false if:\n
    /// > DataIter is empty.\n
    /// > Course is not found in default semester.\n
    /// > Student has not enrolled in that course.\n
    /// > Default semester has not been set.
    bool disenroll(const DataIter &student, const Data::UID &course_uid);
    /// Disenrolls student from a course in default semester.\n
    /// Needs DataIter of that student, course_id.\n\n
    /// True if succeeded, false if:\n
    /// > DataIter is empty.\n
    /// > Course is not found in default semester.\n
    /// > Student has not enrolled in that course.\n
    /// > Default semester has not been set.
    bool disenroll(const DataIter &student, const string &course_id);

    /// Imports scores of a course from CSV.\n
    /// Needs CSVData and DataIter to the course.\n\n
    /// Returns number of scores (each for 1 student) successfully added. Adding fail when:\n
    /// > DataIter is empty.\n
    /// > Student is not found in course.\n
    /// > Parsing fails.
    int addScores(const CSVData &csv, const DataIter &course);
};