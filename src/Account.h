#pragma once

#include <string>
#include <ctime>

#include "Enums.h"
#include "Database.h"

using std::tm, std::string;

class Account : public Data {
protected:
    UserType user_type;
public:
    string username;
    string password;

    Account() : username{}, password{}, user_type{UserType::Unknown} {
        data_type = DataType::Account;
    }
    Account(const string &username, const string &password, const UserType user_type)
            : username{username}, password{password}, user_type{user_type} {
        data_type = DataType::Account;
    }

    bool checkPassword(const string& input) const;
    UserType getUserType() const;
};


class Student : public Account {
public:
    string student_id, social_id;
    FullName name;
    Gender gender;
    tm birth;
    DataIter classroom;
    List<DataIter> courses;
    List<DataIter> scores;

    bool addCourse(const DataIter &course);
    bool removeCourse(const Data::UID &course_uid);
    bool addScore(const DataIter &score);
    bool removeScore(const Data::UID &score_uid);
    /// Returns empty DataIter if no score is found.
    DataIter getScore(const Data::UID &course_uid) const;

    Student() : student_id{}, social_id{}, name{}, birth{}, classroom{} {
        this->user_type = UserType::Student;
        gender = Gender::Unknown;
    }
    Student(const string &student_id,
            const string &social_id,
            const FullName &name,
            const Gender gender,
            const tm &birth) :
            student_id{student_id}, social_id{social_id}, name{name}, gender{gender}, birth{birth}  {
        this->user_type = UserType::Student;
    }
};


class Staff : public Account {
public:
    Staff() {this->user_type = UserType::Staff;}
};