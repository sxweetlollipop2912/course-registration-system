#pragma once

#include <ctime>

#include "Enums.h"
#include "Database.h"
#include "List.h"

class SchoolYear : public Data {
public:
    int start_year, end_year;
    List<DataIter> semesters;
    List<DataIter> classes;

    SchoolYear() : start_year{}, end_year{} {
        this->data_type = DataType::SchoolYear;
    }
    SchoolYear(const int start_year, const int end_year) : start_year{start_year}, end_year{end_year} {
        this->data_type = DataType::SchoolYear;
    }

    /// Returns empty DataIter if no semester is found.
    DataIter getSemesterByNo(const int no);
    /// Returns empty DataIter if no semester is found.
    DataIter getSemesterByUID(const Data::UID &uid);
    /// False if a semester with the same no. is already added, otherwise true.
    bool addSemester(const DataIter &semester);
    /// False if no such semester is found, otherwise true.
    bool removeSemester(const Data::UID &semester_uid);
    /// False if no semester with such no. is found, otherwise true.
    bool removeSemester(const int no);

    /// Returns empty DataIter if no class is found.
    DataIter getClassByName(const string &name);
    DataIter getClassByUID(const Data::UID &uid);
    /// False if a class with the same name is already added, otherwise true.
    bool addClass(const DataIter &classroom);
    /// False if no such class is found, otherwise true.
    bool removeClass(const Data::UID &class_uid);
    /// False if no class with such name is found, otherwise true.
    bool removeClass(const string &name);
};