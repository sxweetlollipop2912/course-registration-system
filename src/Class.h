#pragma once

#include "Enums.h"
#include "Database.h"
#include "List.h"

using std::string, std::shared_ptr;

class Class : public Data {
public:
    string name;
    List<DataIter> students;

    Class() = default;
    Class(const string &name) : name{name} {}

    /// Returns empty DataIter if no student is found.
    DataIter getStudentByID(const string &student_id);
    /// False if a student with the same student_id is already added, otherwise true.
    bool addStudent(const DataIter &student);
    /// False if no such student is found, otherwise true.
    bool removeStudent(const DataIter &student);
    /// False if no student with such student_id is found, otherwise true.
    bool removeStudent(const string &student_id);
    void sortStudent();
};