#pragma once

#include "List.h"
#include "Database.h"
#include "SchoolYear.h"
#include "Semester.h"
#include "Course.h"
#include "Class.h"
#include "Account.h"

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
    void deleteCourse(const DataIter &course);
public:
    Database database;
    DataIter default_year_iter;
    DataIter default_semester_iter;
    DataIter user_iter;

    App() {
        user_iter = {};
        default_year_iter = database.getByUID(default_year_uid);
        default_semester_iter = database.getByUID(default_semester_uid);
    }

    bool loggedIn() const;
    /// Returns UserType of current user.\n\n
    /// Returns:\n
    /// > UserType::Student if current user is a student.\n
    /// > UserType::Staff if current user is a staff.\n
    /// > UserType::Unknown if no user has logged in.\n
    UserType userType() const;

    shared_ptr<Account> user() {return user_iter.ptr<Account>();}
    shared_ptr<SchoolYear> year() {return default_year_iter.ptr<SchoolYear>();}
    shared_ptr<Semester> semester() {return default_semester_iter.ptr<Semester>();}

    void load();
    void save();
    List<DataIter> getAllYears();

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

    /// Adds a new class to database and default schoolyear.\n
    /// Needs a shared ptr of that class.\n\n
    /// Returns DataIter of the new course if succeeded, or empty DataIter if:\n
    /// > Another class with the same name already exists.
    /// > Default schoolyear has not been set.
    DataIter addClass(const shared_ptr<Class> &classroom);
    /// Removes a class from default schoolyear. Does not delete that class entirely.\n
    /// NOTE: This also removes the students from all courses belong to default schoolyear.\n
    /// Needs UID.\n\n
    /// True if succeeded, false if:\n
    /// > No class with such UID is found in default schoolyear.\n
    /// > Default schoolyear has not been set.
    bool removeClassFromYear(const Data::UID &class_uid);
    /// Deletes a class entirely. Also deletes all students in that class.
    void deleteClass(const DataIter &classroom);

    /// Adds a new student to database and to a class in default schoolyear.\n
    /// Needs a shared ptr of that student, name of the class.\n\n
    /// Returns DataIter of the new course if succeeded, or empty DataIter if:\n
    /// > Another student with the same student_id already exists in database.\n
    /// > Class is not found in default schoolyear.\n
    /// > Default schoolyear has not been set.
    DataIter addStudent(const shared_ptr<Student> &student, const string &class_name);
    /// Adds a new student to database and to a class in default schoolyear.\n
    /// Needs a shared ptr of that student, uid of the class.\n\n
    /// Returns DataIter of the new course if succeeded, or empty DataIter if:\n
    /// > Another student with the same student_id already exists in database.\n
    /// > Class is not found in default schoolyear.\n
    /// > Default schoolyear has not been set.
    DataIter addStudent(const shared_ptr<Student> &student, const Data::UID &class_uid);
    /// Deletes a student in default schoolyear.\n
    /// Note: This method deletes student from class and from all courses, even those in previous schoolyears.\n
    /// Needs DataIter of that student.\n\n
    /// True if succeeded, false if:\n
    /// > Student's class is not in default schoolyear.\n
    /// > Student does not belong to a class.\n
    /// > Default schoolyear has not been set.
    bool deleteStudent(const DataIter &student);
    /// Enrolls student to a course in default semester.\n
    /// Needs DataIter of that student, UID of the course.\n\n
    /// True if succeeded, false if:\n
    /// > Course is not found in default semester.\n
    /// > Student does not belong to default schoolyear.\n
    /// > Student has already enrolled in that course.\n
    /// > Default semester has not been set.
    bool enroll(const DataIter &student, const Data::UID &course_uid);
    /// Enrolls student to a course in default semester.\n
    /// Needs DataIter of that student, course_id.\n\n
    /// True if succeeded, false if:\n
    /// > Course is not found in default semester.\n
    /// > Student does not belong to default schoolyear.\n
    /// > Student has already enrolled in that course.\n
    /// > Default semester has not been set.
    bool enroll(const DataIter &student, const string &course_id);
    /// Disenrolls student from a course in default semester.\n
    /// Needs DataIter of that student, UID of the course.\n\n
    /// True if succeeded, false if:\n
    /// > Course is not found in default semester.\n
    /// > Student has not enrolled in that course.\n
    /// > Default semester has not been set.
    bool disenroll(const DataIter &student, const Data::UID &course_uid);
    /// Disenrolls student from a course in default semester.\n
    /// Needs DataIter of that student, course_id.\n\n
    /// True if succeeded, false if:\n
    /// > Course is not found in default semester.\n
    /// > Student has not enrolled in that course.\n
    /// > Default semester has not been set.
    bool disenroll(const DataIter &student, const string &course_id);
};