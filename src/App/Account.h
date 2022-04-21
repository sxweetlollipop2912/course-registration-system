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
private:
    /// Returns ptr to the added score.
    shared_ptr<Score> addScore(const Score &score);
    bool removeScore(const Data::UID &course_id);
public:
    string student_id, social_id;
    FullName name;
    Gender gender;
    tm birth;
    DataIter classroom;
    List<DataIter> courses;
    List<shared_ptr<Score>> scores;

    List<DataIter> overlappingCourses(const List<Course::Session> &sessions) const;
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
        this->username = student_id;
        this->password = ACCOUNT::DEFAULT_PASS;
    }
};


class Staff : public Account {
public:
    Staff(const string &username) {
        this->user_type = UserType::Staff;
        this->username = username;
        this->password = ACCOUNT::DEFAULT_PASS;
    }
    Staff(const string &username, const string &password) {
        this->user_type = UserType::Staff;
        this->username = username;
        this->password = password;
    }
};