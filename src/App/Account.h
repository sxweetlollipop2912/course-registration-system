#pragma once

#include <string>
#include <ctime>

#include "../Enums.h"
#include "Database.h"
#include "../Constants.h"
#include "Course.h"
#include "Utils.h"

using std::tm, std::string, std::shared_ptr;

class Account : public Data {
    typedef Data super;
protected:
    UserType user_type;
public:
    string username;
    string password;
    FullName name;
    Gender gender;

    Account() : username{}, password{}, name{}, gender{}, user_type{UserType::Unknown} {
        data_type = DataType::Unknown;
    }
    Account(const string &username, const string &password, const FullName &name, const Gender &gender, const UserType user_type)
            : username{username}, password{password}, name{name}, gender{gender}, user_type{user_type} {
        data_type = DataType::Unknown;
    }

    friend std::ostream& operator<<(std::ostream &os, const Account &obj)  {
        os << (super&)obj;

        os << (int)obj.user_type << '\n';
        os << obj.username << '\n';
        os << obj.password << '\n';
        os << obj.name.last << '\n';
        os << obj.name.first << '\n';
        os << (int)obj.gender << '\n';

        return os;
    }
    friend std::istream& operator>>(std::istream &is, Account &obj) {
        is >> (super&)obj;

        int type; is >> type;
        obj.user_type = static_cast<UserType>(type);
        Utils::getline(is, obj.username);
        Utils::getline(is, obj.password);
        Utils::getline(is, obj.name.last);
        Utils::getline(is, obj.name.first);

        is >> type;
        obj.gender = static_cast<Gender>(type);

        return is;
    }

    bool checkPassword(const string& input) const;
    UserType getUserType() const;
};


class Student : public Account {
    friend class App;
    typedef Account super;
private:
    /// Returns false if course is already added.
    bool addCourse(const DataIter &course);
    /// Returns false if no course is found.
    bool removeCourse(const Data::UID &course_uid);
    /// This replaces if another score has already existed.\n
    /// Returns ptr to the added score.
    shared_ptr<Score> replaceScore(const Score &score);
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
        data_type = DataType::Student;
    }
    Student(const string &student_id,
            const string &social_id,
            const FullName &name,
            const Gender gender,
            const tm &birth) :
            Account(student_id, ACCOUNT::DEFAULT_PASS, name, gender, UserType::Student),
            student_id{student_id}, social_id{social_id}, birth{birth}  {
        data_type = DataType::Student;
    }

    void load(Database &database) override;

    friend std::ostream& operator<<(std::ostream &os, const Student &obj) {
        os << (super&)obj;

        os << obj.student_id << '\n';
        os << obj.social_id << '\n';
        os << Utils::tmToStr(obj.birth) << '\n';
        os << obj.classroom << '\n';

        os << obj.courses.size() << '\n';
        for(const auto& e : obj.courses)
            os << e << '\n';

        os << obj.scores.size() << '\n';
        for(const auto& e : obj.scores)
            os << *e << '\n';

        return os;
    }
    friend std::istream& operator>>(std::istream &is, Student &obj) {
        is >> (super&)obj;

        Utils::getline(is, obj.student_id);
        Utils::getline(is, obj.social_id);

        string date;
        Utils::getline(is, date);
        obj.birth = Utils::strToTm(date);

        is >> obj.classroom;

        int sz; is >> sz;
        obj.courses.resize(sz);
        for(auto& e : obj.courses) {
            is >> e;
        }

        is >> sz;
        obj.scores.resize(sz);
        for(auto& e : obj.scores) {
            e = make_shared<Score>();
            is >> *e;
        }

        return is;
    }

    /// Basically checks if student_id and social_id is initiated.
    bool valid() const;

    /// Returns a default Student if an exception was thrown.\n
    /// Otherwise returns Student with parsed info (there might have been failed parsing attempts).
    static Student tryParse(const List<string> &headers, const List<string> &row);

    /// Gets a list of courses that conflict with provided sessions.\n\n
    /// Returns an empty list if there are no conflicting courses.
    List<DataIter> getOverlappingCourses(const List<Course::Session> &sessions) const;
    List<DataIter> getCoursesInSemester(const Data::UID &semester_uid);
    /// Returns empty DataIter if no course is found.
    DataIter getCourse(const string &course_id);
    DataIter getCourse(const Data::UID &uid);
    /// Gets a pointer of a score of a course.\n\n
    /// Returns nullptr if:\n
    /// > Student hasn't enrolled in the course.
    shared_ptr<Score> getScore(const Data::UID &course_uid);
    /// Gets a pointer of a score of a course.\n\n
    /// Returns nullptr if:\n
    /// > Student hasn't enrolled in the course.
    shared_ptr<Score> getScore(const string &course_id);
    /// Gets a pointer of a score of a course.\n\n
    /// Returns nullptr if:\n
    /// > Student hasn't enrolled in the course.
    shared_ptr<const Score> getScore(const Data::UID &course_uid) const;
    /// Gets a pointer of a score of a course.\n\n
    /// Returns nullptr if:\n
    /// > Student hasn't enrolled in the course.
    shared_ptr<const Score> getScore(const string &course_id) const;
};


class Staff : public Account {
    typedef Account super;
public:
    Staff() : Account({}, ACCOUNT::DEFAULT_PASS, {}, Gender::Unknown, UserType::Staff) {
        data_type = DataType::Staff;
    }

    Staff(const string &username, const FullName &name = {}, const Gender &gender = Gender::Unknown) :
            Account(username, ACCOUNT::DEFAULT_PASS, name, gender, UserType::Staff) {
        data_type = DataType::Staff;
    }

    Staff(const string &username, const string &password, const FullName &name = {}, const Gender &gender = Gender::Unknown) :
            Account(username, password, name, gender, UserType::Staff) {
        data_type = DataType::Staff;
    }

    friend std::ostream& operator<<(std::ostream &os, const Staff &obj) {
        os << (super&)obj;
        return os;
    }
    friend std::istream& operator>>(std::istream &is, Staff &obj) {
        is >> (super&)obj;
        return is;
    }
};