#pragma once

#include <ctime>

#include "../Enums.h"
#include "Database.h"
#include "List.h"
#include "RegistrationSession.h"

using std::tm, std::string;

class Semester : public Data {
    friend class App;

    typedef Data super;
private:
    /// False if a course with the same course_id is already added, otherwise true.
    bool addCourse(const DataIter &course);

    /// False if no course with such UID is found, otherwise true.
    bool removeCourse(const Data::UID &course_uid);

    /// False if no couse with such couse_id is found, otherwise true.
    bool removeCourse(const string &course_id);

public:
    DataIter school_year;
    RegistrationSession reg_session{};
    int no{};
    tm start{}, end{};
    List<DataIter> courses;

    Semester() : school_year{} {
        data_type = DataType::Semester;
    }

    Semester(const int no, const tm &start, const tm &end) : no{no}, start{start}, end{end} {
        data_type = DataType::Semester;
    }

    void load(Database &database) override;

    /// Returns empty DataIter if no course is found.
    DataIter getCourse(const string &course_id);

    DataIter getCourse(const Data::UID &uid);

    /// Sorts courses by course ID.
    void sortCourse();

    friend std::ostream &operator<<(std::ostream &os, const Semester &obj) {
        os << (super &) obj;

        os << obj.school_year << '\n';
        os << obj.reg_session << '\n';
        os << obj.no << '\n';
        os << Utils::tmToStr(obj.start) << '\n';
        os << Utils::tmToStr(obj.end) << '\n';

        os << obj.courses.size() << '\n';
        for (const auto &e: obj.courses)
            os << e << '\n';

        return os;
    }

    friend std::istream &operator>>(std::istream &is, Semester &obj) {
        is >> (super &) obj;

        int sz;

        is >> obj.school_year;
        is >> obj.reg_session;
        is >> obj.no;

        string s;
        Utils::getline(is, s);
        obj.start = Utils::strToTm(s);
        Utils::getline(is, s);
        obj.end = Utils::strToTm(s);

        is >> sz;
        obj.courses.resize(sz);
        for (auto &e: obj.courses) {
            is >> e;
        }

        return is;
    }
};