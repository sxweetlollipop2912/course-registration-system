#pragma once

#include <string>
#include <ctime>

#include "../Enums.h"
#include "Database.h"
#include "../Constants.h"
#include "Course.h"

using std::tm, std::string, std::shared_ptr;

class Account : public Data {
protected:
    UserType user_type;
public:
    string username;
    string password;
    FullName name;
    Gender gender;

    Account() : username{}, password{}, name{}, gender{}, user_type{UserType::Unknown} {
        data_type = DataType::Account;
    }
    Account(const string &username, const string &password, const FullName &name, const Gender &gender, const UserType user_type)
            : username{username}, password{password}, name{name}, gender{gender}, user_type{user_type} {
        data_type = DataType::Account;
    }

    bool checkPassword(const string& input) const;
    UserType getUserType() const;
};


class Student : public Account {
private:
    /// Returns ptr to the added score.
    shared_ptr<Score> addScore(const Score &score);
    bool removeScore(const Data::UID &course_id);
public:
    string student_id, social_id;
    tm birth;
    DataIter classroom;
    List<DataIter> courses;
    List<shared_ptr<Score>> scores;

    Student() : student_id{}, social_id{}, birth{}, classroom{} {
        this->password = ACCOUNT::DEFAULT_PASS;
        this->user_type = UserType::Student;
    }
    Student(const string &student_id,
            const string &social_id,
            const FullName &name,
            const Gender gender,
            const tm &birth) :
            Account(student_id, ACCOUNT::DEFAULT_PASS, name, gender, UserType::Student),
            student_id{student_id}, social_id{social_id}, birth{birth}  {}

    /// Returns a default Student if an exception was thrown.\n
    /// Otherwise returns Student with parsed info (there might have been failed parsing attempts).
    static Student tryParse(const List<string> &headers, const List<string> &row);

    List<DataIter> overlappingCourses(const List<Course::Session> &sessions) const;
    /// Returns empty DataIter if no course is found.
    DataIter getCourse(const string &course_id);
    DataIter getCourse(const Data::UID &uid);
    /// Returns false if course is already added.
    bool addCourse(const DataIter &course);
    /// Returns false if no course is found.
    bool removeCourse(const Data::UID &course_uid);
    /// This replaces if another score has already existed.\n
    /// Returns ptr to the added score.
    shared_ptr<Score> replaceScore(const Score &score);
    /// Returns nullptr if no score is found.
    shared_ptr<Score> getScore(const Data::UID &course_uid);
    /// Returns nullptr if no score is found.
    shared_ptr<Score> getScore(const string &course_id);
    /// Returns nullptr if no score is found.
    shared_ptr<const Score> getScore(const Data::UID &course_uid) const;
    /// Returns nullptr if no score is found.
    shared_ptr<const Score> getScore(const string &course_id) const;
};


class Staff : public Account {
public:
    Staff(const string &username, const FullName &name = {}, const Gender &gender = Gender::Unknown) :
    Account(username, ACCOUNT::DEFAULT_PASS, name, gender, UserType::Staff) {}

    Staff(const string &username, const string &password, const FullName &name = {}, const Gender &gender = Gender::Unknown) :
            Account(username, password, name, gender, UserType::Staff) {}
};