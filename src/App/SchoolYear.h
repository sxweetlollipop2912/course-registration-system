#pragma once

#include <ctime>

#include "../Enums.h"
#include "Database.h"
#include "List.h"

class SchoolYear : public Data {
public:
    int start_year, end_year;
    List<DataIter> semesters;

    SchoolYear() : start_year{}, end_year{} {
        this->data_type = DataType::SchoolYear;
    }
    SchoolYear(const int start_year, const int end_year) : start_year{start_year}, end_year{end_year} {
        this->data_type = DataType::SchoolYear;
    }

    /// Returns empty DataIter if no semester is found.
    DataIter getSemester(const int no);
    /// Returns empty DataIter if no semester is found.
    DataIter getSemester(const Data::UID &uid);
    /// False if a semester with the same no. is already added, otherwise true.
    bool addSemester(const DataIter &semester);
    /// False if no such semester is found, otherwise true.
    bool removeSemester(const Data::UID &semester_uid);
    /// False if no semester with such no. is found, otherwise true.
    bool removeSemester(const int no);
};