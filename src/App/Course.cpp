#include <iostream>

#include "Course.h"
#include "Account.h"
#include "Utils.h"

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

int Course::tryParseScore(const CSVData &csv) {
    int count = 0;

    auto rows = csv.getData();
    auto headers = csv.getHeaders();

    for(const auto &row : rows) {
        string student_id;
        FullName student_name;
        Score score;

        try {
            for (int i = 0; i < min(headers.size(), row.size()); i++) {
                auto header = headers[i];
                auto data = row[i];

                if (header.find("student") != string::npos && header.find("id") != string::npos) {
                    student_id = data;
                }

                else if (header.find("name") != string::npos && header.find("full") != string::npos) {
                    student_name.first = data;
                }

                else if (header.find("midterm") != string::npos) {
                    for (int pos = (int) header.find(' '); pos != string::npos; pos = (int) header.find(' '))
                        header.erase(pos, 1);

                    score.midterm = stod(data);
                }

                else if (header.find("final") != string::npos) {
                    for (int pos = (int) header.find(' '); pos != string::npos; pos = (int) header.find(' '))
                        header.erase(pos, 1);

                    score.final = stod(data);
                }

                else if (header.find("total") != string::npos) {
                    for (int pos = (int) header.find(' '); pos != string::npos; pos = (int) header.find(' '))
                        header.erase(pos, 1);

                    score.total = stod(data);
                }

                else if (header.find("other") != string::npos) {
                    for (int pos = (int) header.find(' '); pos != string::npos; pos = (int) header.find(' '))
                        header.erase(pos, 1);

                    score.other = stod(data);
                }
            }

            auto student = getStudent(student_id);
            if (student) {
                auto student_score = student.ptr<Student>()->getScore(this->uid);
                student_score->final = score.final;
                student_score->midterm = score.midterm;
                student_score->total = score.total;
                student_score->other = score.other;

                ++count;
            }
        }
        catch (exception &e) {
            cerr << e.what() << std::endl;
        }
    }

    return count;
}

bool Course::exportScore(CSVIO::CSVWriter &writer) {

}

bool Course::Session::inRange(const tm &time) const {
    tm tmp = time, tmp_st = start, tmp_en = end;
    return mktime(&tmp_st) < mktime(&tmp) && mktime(&tmp_en) > mktime(&tmp);
}

bool Course::Session::operator==(const Session &s) const {
    tm s_st = s.start, s_en = s.end, tmp_st = start, tmp_en = end;
    return mktime(&s_st) == mktime(&tmp_st) && mktime(&s_en) == mktime(&tmp_en);
}
