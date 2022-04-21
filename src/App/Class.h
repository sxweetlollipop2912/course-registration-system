#pragma once

#include "../Enums.h"
#include "Database.h"
#include "List.h"

using std::string, std::shared_ptr;

class Class : public Data {
public:
    string name;
    List<DataIter> students;

    Class() {
        data_type = DataType::Class;
    }
    Class(const string &name) : name{name} {
        data_type = DataType::Class;
    }

    /// Returns empty DataIter if no student is found.
    DataIter getStudentByID(const string &student_id);
    /// False if a student with the same student_id is already added, otherwise true.
    bool addStudent(const DataIter &student);
    /// False if no such student is found, otherwise true.
    bool removeStudent(const Data::UID &student_uid);
    /// False if no student with such student_id is found, otherwise true.
    bool removeStudent(const string &student_id);
    void sortStudent();

    bool operator ==(const Class &other) const {return name == other.name;}
};