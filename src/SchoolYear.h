#pragma once

#include <ctime>

#include "Enums.h"
#include "Database.h"
#include "List.h"

class SchoolYear : public Data {
public:
    int start_year, end_year;
    List<DataIter> semesters;

    SchoolYear() : start_year{}, end_year{} {}
    SchoolYear(const int start_year, const int end_year) : start_year{start_year}, end_year{end_year} {}

    /// Returns empty DataIter if no semester is found.
    DataIter getSemesterByNo(const int no);
    /// False if a semester with the same no. is already added, otherwise true.
    bool addSemester(const DataIter &semester);
    /// False if no such semester is found, otherwise true.
    bool removeSemester(const DataIter &semester);
    /// False if no semester with such no. is found, otherwise true.
    bool removeSemester(const int no);
};