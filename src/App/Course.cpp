#include <iostream>
#include <sstream>

#include "Course.h"
#include "Account.h"

using std::shared_ptr, std::min, std::exception, std::stod, std::cerr;


DataIter Course::getStudent(const string &student_id) {
    auto it = students.find_if([&](const DataIter &ref) {
        auto studentOnList = ref.ptr<Student>();

        if (student_id == studentOnList->student_id) return true;

        return false;
    });

    if (it != students.end()) return *it;
    return {};
}

void Course::sortStudentsByID() {
    students.sort([](const DataIter &s1, const DataIter &s2) {
        return s1.ptr<Student>()->student_id < s2.ptr<Student>()->student_id;
    });
}

bool Course::addStudent(const DataIter &student) {
    /// Check by student_id if this student is not already added.
    auto student_id = student.ptr<Student>()->student_id;
    if (getStudent(student_id).empty()) {
        students.push_back(student);

        return true;
    }

    return false;
}

bool Course::removeStudent(const Data::UID& student_uid) {
    auto it = students.find_if([&](const DataIter &iter) {
        return iter.uid() == student_uid;
    });
    if (it != students.end()) {
        students.remove(it);

        return true;
    }

    return false;
}

bool Course::removeStudent(const string &student_id) {
    auto it = students.find_if([&](const DataIter &ref) {
        auto studentOnList = ref.ptr<Student>();

        if (student_id == studentOnList->student_id) return true;

        return false;
    });
    if (it != students.end()) {
        students.remove(it);

        return true;
    }

    return false;
}

Course::Session::Session(const string &s) {
    try {
        std::stringstream ss(s);
        string wday;
        ss >> wday;

        string session;
        ss >> session;

        string wday_s[] = { "sun", "mon", "tue", "wed", "thu", "fri", "sat" };
        Utils::toLowerStr(wday);
        int i;
        for (i = 0; i < 7 && wday != wday_s[i]; i++);

        if (i == 7) {
            start = end = tm{};
            return;
        }

        Utils::toLowerStr(session);
        if (session == "s1") {
            start = Utils::mksession(i + 1, 7, 30);
            end = Utils::mksession(i + 1, 9, 10);
        }
        else if (session == "s2") {
            start = Utils::mksession(i + 1, 9, 30);
            end = Utils::mksession(i + 1, 11, 10);
        }
        else if (session == "s3") {
            start = Utils::mksession(i + 1, 13, 30);
            end = Utils::mksession(i + 1, 15, 10);
        }
        else if (session == "s4") {
            start = Utils::mksession(i + 1, 15, 30);
            end = Utils::mksession(i + 1, 17, 10);
        }
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;

        start = end = tm{};
    }
}

bool Course::Session::inRange(const tm &time) const {
    tm tmp = time, tmp_st = start, tmp_en = end;
    return mktime(&tmp_st) < mktime(&tmp) && mktime(&tmp_en) > mktime(&tmp);
}

bool Course::Session::operator==(const Session &s) const {
    tm s_st = s.start, s_en = s.end, tmp_st = start, tmp_en = end;
    return mktime(&s_st) == mktime(&tmp_st) && mktime(&s_en) == mktime(&tmp_en);
}

bool Course::Session::valid() const {
    tm tmp_st = start, tmp_en = end;
    return mktime(&tmp_st) != -1 && mktime(&tmp_en) != -1;
}

string Course::Session::toStr() const {
    if (!valid()) return {};

    try {
        string s;
        tm tmp_st = start, tmp_en = end;
        mktime(&tmp_st);
        mktime(&tmp_en);

        string wday_s[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
        s += wday_s[tmp_st.tm_mday - 1] + " ";

        if (tmp_st.tm_hour == 7)
            s += "S1";
        else if (tmp_st.tm_hour == 9)
            s += "S2";
        else if (tmp_st.tm_hour == 13)
            s += "S3";
        else if (tmp_st.tm_hour == 15)
            s += "S4";

        return s;
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;

        return {};
    }
}

void Course::load(Database &database) {
    semester = database.get(semester.uid());

    for(auto &e : students)
        e = database.get(e.uid());
}

bool Score::valid() const {
    return midterm >= 0 && final >= 0 && total >= 0 && other >= 0;
}
