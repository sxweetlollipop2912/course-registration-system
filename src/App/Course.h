#pragma once

#include <ctime>

#include "../Enums.h"
#include "Database.h"
#include "List.h"

using std::string, std::tm;

class Course;
class CourseScore;

class Course : public Data {
public:
    class Session {
    public:
        tm start, end;
        Session() = default;
        Session(const tm &start, const tm &end) : start{start}, end{end} {}
    };

    DataIter semester;
    string name, id;
    FullName teacher_name;
    int credits, max_students;
    List<Session> sessions;
    List<DataIter> students;

    Course() = default;
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
    }

    /// Returns empty DataIter if no student is found.
    DataIter getStudentByID(const string &student_id);
    /// False if a student with the same student_id is already added, otherwise true.
    bool addStudent(const DataIter& student);
    /// False if no such student is found, otherwise true.
    bool removeStudent(const Data::UID& student_uid);
    /// False if no student with such student_id is found, otherwise true.
    bool removeStudent(const string &student_id);

};


class CourseScore : public Data {
public:
    DataIter course;
    double midterm, final, total, other;

    CourseScore() = default;
    CourseScore(const DataIter& course, const double midterm, const double final, const double total, const double other)
    : course{course}, midterm{midterm}, final{final}, total{total}, other{other} {}
};