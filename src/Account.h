#pragma once

#include <string>
#include <ctime>

#include "Enums.h"
#include "Database.h"

using std::tm, std::string;

class Account : public Data {
public:
    string username;
    string password;
    UserType user_type;

    Account() : username{}, password{}, user_type{UserType::Unknown} {}
    Account(const string &username, const string &password, const UserType user_type)
            : username{username}, password{password}, user_type{user_type} {}

    bool checkPassword(const string& input) const;
};


class Student : public Account {
public:
    int student_id, social_id;
    FullName name;
    Gender gender;
    tm birth;
    DataIter classroom;
    List<DataIter> courses;
    List<DataIter> scores;

    bool enroll(const DataIter &course);
    bool disenroll(const DataIter &course);
    bool setScore(const DataIter &score);
    bool removeScore(const DataIter &score);
    /// Returns empty DataIter if no score is found.
    DataIter getScore(const DataIter &course) const;

    Student() : student_id{}, social_id{}, name{}, birth{}, classroom{} {
        this->user_type = UserType::Student;
        gender = Gender::Unknown;
    }
};


class Staff : public Account {
public:
    Staff() {this->user_type = UserType::Staff;}
};