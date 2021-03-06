#pragma once

#include <ctime>

#include "../Enums.h"
#include "Database.h"
#include "List.h"

class SchoolYear : public Data {
    friend class App;

    typedef Data super;
private:
    /// False if a semester with the same no. is already added, otherwise true.
    bool addSemester(const DataIter &semester);

    /// False if no such semester is found, otherwise true.
    bool removeSemester(const Data::UID &semester_uid);

    /// False if no semester with such no. is found, otherwise true.
    bool removeSemester(const int no);

public:
    int start_year, end_year;
    List<DataIter> semesters;

    SchoolYear() : start_year{}, end_year{} {
        this->data_type = DataType::SchoolYear;
    }

    SchoolYear(const int start_year, const int end_year) : start_year{start_year}, end_year{end_year} {
        this->data_type = DataType::SchoolYear;
    }

    void load(Database &database) override;

    /// Returns empty DataIter if no semester is found.
    DataIter getSemester(const int no);

    /// Returns empty DataIter if no semester is found.
    DataIter getSemester(const Data::UID &uid);

    /// Sorts semesters by no.
    void sortSemesters();

    friend std::ostream &operator<<(std::ostream &os, const SchoolYear &obj) {
        os << (super &) obj;

        os << obj.start_year << '\n';
        os << obj.end_year << '\n';

        os << obj.semesters.size() << '\n';
        for (const auto &e: obj.semesters)
            os << e << '\n';

        return os;
    }

    friend std::istream &operator>>(std::istream &is, SchoolYear &obj) {
        is >> (super &) obj;

        int sz;

        is >> obj.start_year;
        is >> obj.end_year;

        is >> sz;
        obj.semesters.resize(sz);
        for (auto &e: obj.semesters)
            is >> e;

        return is;
    }
};