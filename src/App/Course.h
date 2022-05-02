#pragma once

#include <ctime>

#include "../Enums.h"
#include "Database.h"
#include "List.h"
#include "CSV.h"
#include "Utils.h"

using std::string, std::tm;

class Course;
class Score;

class Course : public Data {
    friend class App;
    typedef Data super;
private:
    /// False if a student with the same student_id is already added, otherwise true.
    bool addStudent(const DataIter& student);
    /// False if no such student is found, otherwise true.
    bool removeStudent(const Data::UID& student_uid);
    /// False if no student with such student_id is found, otherwise true.
    bool removeStudent(const string &student_id);

public:
    class Session {
    public:
        tm start, end;
        Session() = default;
        Session(const tm &start, const tm &end) : start{start}, end{end} {}
        /// string example: mon s4 (case-insensitive).
        Session(const string &s);

        bool valid() const;
        bool inRange(const tm &time) const;
        string toStr() const;
        bool operator==(const Session &s) const;

        friend std::ostream& operator<<(std::ostream &os, const Session &obj) {
            os << Utils::tmToStr(obj.start) << '\n';
            os << Utils::tmToStr(obj.end) << '\n';

            return os;
        }
        friend std::istream& operator>>(std::istream &is, Session &obj) {
            string s;
            Utils::getline(is, s);
            obj.start = Utils::strToTm(s);
            Utils::getline(is, s);
            obj.end = Utils::strToTm(s);

            return is;
        }
    };

    DataIter semester;
    string name, id;
    FullName teacher_name;
    int credits{}, max_students{};
    List<Session> sessions;
    List<DataIter> students;

    Course() : semester{}, name{}, id{}, teacher_name{} {
        data_type = DataType::Course;
    }
    Course(const string &id,
           const string &name,
           const FullName &teacher_name,
           const Session &session1,
           const Session &session2,
           const int credits,
           const int max_students = 50)
            : id{id}, name{name}, teacher_name{teacher_name}, credits{credits}, max_students{max_students} {
        sessions.push_back(session1);
        sessions.push_back(session2);
        data_type = DataType::Course;
    }

    void load(Database &database) override;

    /// Returns empty DataIter if no student is found.
    DataIter getStudent(const string &student_id);
    /// Returns number of successful parsing attempt (attempts without any exception).\n
    int tryParseScore(const CSVData &csv);
    /// True if succeeded, otherwise false.
    bool exportScore(CSVIO::CSVWriter &writer);
    /// Sorts student by student ID.
    void sortStudentsByID();

    friend std::ostream& operator<<(std::ostream &os, const Course &obj) {
        os << (super&)obj;

        os << obj.semester << '\n';
        os << obj.name << '\n';
        os << obj.id << '\n';
        os << obj.teacher_name.last << '\n';
        os << obj.teacher_name.first << '\n';
        os << obj.credits << '\n';
        os << obj.max_students << '\n';

        os << obj.sessions.size() << '\n';
        for(const auto &e : obj.sessions)
            os << e << '\n';

        os << obj.students.size() << '\n';
        for(const auto& e : obj.students)
            os << e << '\n';

        return os;
    }
    friend std::istream& operator>>(std::istream &is, Course &obj) {
        is >> (super&)obj;

        int sz;

        is >> obj.semester;
        Utils::getline(is, obj.name);
        Utils::getline(is, obj.id);
        Utils::getline(is, obj.teacher_name.last);
        Utils::getline(is, obj.teacher_name.first);
        is >> obj.credits;
        is >> obj.max_students;

        is >> sz; obj.sessions.resize(sz);
        for(auto &e : obj.sessions)
            is >> e;

        is >> sz; obj.students.resize(sz);
        for(auto &e : obj.students)
            is >> e;

        return is;
    }
};


class Score {
public:
    DataIter course;
    double midterm = -1, final = -1, total = -1, other = -1;

    Score() = default;
    Score(const DataIter &course): course{course} {}
    Score(const DataIter &course, const double midterm, const double final, const double total, const double other)
            : course{course}, midterm{midterm}, final{final}, total{total}, other{other} {}

    bool valid() const;

    friend std::ostream& operator<<(std::ostream &os, const Score &obj) {
        os << obj.course << '\n';
        os << obj.midterm << '\n';
        os << obj.final << '\n';
        os << obj.total << '\n';
        os << obj.other << '\n';

        return os;
    }
    friend std::istream& operator>>(std::istream &is, Score &obj) {
        is >> obj.course;
        is >> obj.midterm;
        is >> obj.final;
        is >> obj.total;
        is >> obj.other;

        return is;
    }
};