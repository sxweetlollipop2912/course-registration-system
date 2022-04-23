#pragma once

#include "../Enums.h"
#include "Database.h"
#include "List.h"

using std::string, std::shared_ptr;

class Class : public Data {
    friend class App;
    typedef Data super;
private:
    /// False if a student with the same student_id is already added, otherwise true.
    bool addStudent(const DataIter &student);
    /// False if no such student is found, otherwise true.
    bool removeStudent(const Data::UID &student_uid);
    /// False if no student with such student_id is found, otherwise true.
    bool removeStudent(const string &student_id);
public:
    string name;
    List<DataIter> students;

    Class() {
        data_type = DataType::Class;
    }
    Class(const string &name) : name{name} {
        data_type = DataType::Class;
    }

    void load(Database &database) override;

    /// Returns empty DataIter if no student is found.
    DataIter getStudent(const string &student_id);
    void sortStudent();

    bool operator ==(const Class &other) const {return name == other.name;}
    friend std::ostream& operator<<(std::ostream &os, const Class &obj) {
        os << (super&)obj;

        os << obj.name << '\n';
        os << obj.students.size() << '\n';
        for(const auto& e : obj.students)
            os << e << '\n';

        return os;
    }
    friend std::istream& operator>>(std::istream &is, Class &obj) {
        is >> (super&)obj;

        int sz;

        Utils::getline(is, obj.name);
        is >> sz;
        obj.students.resize(sz);
        for(auto& e : obj.students) {
            is >> e;
        }

        return is;
    }
};