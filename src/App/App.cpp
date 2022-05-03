#include <filesystem>
#include <fstream>

#include "App.h"
#include "Utils.h"

namespace fs = std::filesystem;
using std::shared_ptr, std::dynamic_pointer_cast, std::static_pointer_cast, std::min, std::stoi, std::exception, std::cerr;

bool App::save() {
    database.clean();

    fs::remove_all(PATH::SAVES);
    fs::create_directories(PATH::SAVES);

    std::ofstream ofs(PATH::APP);
    if (!ofs.is_open())
        return false;
    ofs << default_year_iter << '\n';
    ofs << default_semester_iter << '\n';
    ofs.close();
    ofs.clear();

    ofs.open(PATH::DATABASE);
    if (!ofs.is_open())
        return false;
    ofs << database;
    ofs.close();
    ofs.clear();

    for (const auto &e: database.data) {
        ofs.open(PATH::SAVES + e->uid.id + ".txt");

        if (!ofs.is_open())
            return false;

        switch (e->data_type) {
            case DataType::SchoolYear: {
                ofs << *dynamic_pointer_cast<SchoolYear>(e);
                break;
            }
            case DataType::Student: {
                ofs << *dynamic_pointer_cast<Student>(e);
                break;
            }
            case DataType::Staff: {
                ofs << *dynamic_pointer_cast<Staff>(e);
                break;
            }
            case DataType::Class: {
                ofs << *dynamic_pointer_cast<Class>(e);
                break;
            }
            case DataType::Course: {
                ofs << *dynamic_pointer_cast<Course>(e);
                break;
            }
            case DataType::Semester: {
                ofs << *dynamic_pointer_cast<Semester>(e);
                break;
            }
            case DataType::Other:
            case DataType::Unknown: {
                break;
            }
        }

        ofs.close();
        ofs.clear();
    }

    return true;
}

bool App::load() {
    std::ifstream ifs(PATH::APP);
    if (!ifs.is_open())
        return false;
    ifs >> default_year_iter;
    ifs >> default_semester_iter;
    ifs.close();
    ifs.clear();

    ifs.open(PATH::DATABASE);
    if (!ifs.is_open())
        return false;
    ifs >> database;
    ifs.close();
    ifs.clear();

    for (auto &e: database.data) {
        ifs.open(PATH::SAVES + e->uid.id + ".txt");

        if (!ifs.is_open()) {
            database.data.clear();

            return false;
        }

        switch (e->data_type) {
            case DataType::SchoolYear: {
                SchoolYear obj;
                ifs >> obj;
                e = dynamic_pointer_cast<Data>(make_shared<SchoolYear>(obj));
                break;
            }
            case DataType::Student: {
                Student obj;
                ifs >> obj;
                e = dynamic_pointer_cast<Data>(make_shared<Student>(obj));
                break;
            }
            case DataType::Staff: {
                Staff obj;
                ifs >> obj;
                e = dynamic_pointer_cast<Data>(make_shared<Staff>(obj));
                break;
            }
            case DataType::Class: {
                Class obj;
                ifs >> obj;
                e = dynamic_pointer_cast<Data>(make_shared<Class>(obj));
                break;
            }
            case DataType::Course: {
                Course obj;
                ifs >> obj;
                e = dynamic_pointer_cast<Data>(make_shared<Course>(obj));
                break;
            }
            case DataType::Semester: {
                Semester obj;
                ifs >> obj;
                e = dynamic_pointer_cast<Data>(make_shared<Semester>(obj));
                break;
            }
            case DataType::Other:
            case DataType::Unknown: {
                break;
            }
        }

        ifs.close();
        ifs.clear();
    }

    for (auto &e: database.data)
        e->load(database);

    default_year_iter = database.get(default_year_iter.uid());
    default_semester_iter = database.get(default_semester_iter.uid());

    return true;
}

DataIter App::addAccount(const shared_ptr<Account> &account) {
    if (database.data.find_if([&](const shared_ptr<Data> &ptr) {
        return (ptr->data_type == DataType::Student || ptr->data_type == DataType::Staff) &&
               dynamic_pointer_cast<Account>(ptr)->username == account->username;
    }) != database.data.end()) {
        return {};
    }

    return database.add(account);
}

DataIter App::addStaff(const shared_ptr<Staff> &staff) {
    return addAccount(static_pointer_cast<Account>(staff));
}

int App::addStaffs(const CSVData &csv) {
    return 0;
}

bool App::isLoggedIn() const {
    return user_iter;
}

UserType App::userType() const {
    if (!isLoggedIn())
        return UserType::Unknown;

    return user_iter.ptr<Account>()->getUserType();
}

List<DataIter> App::getAllYears() {
    auto list = database.getAll([](const shared_ptr<Data> &ptr) {
        return ptr->data_type == DataType::SchoolYear;
    });
    list.sort([](const DataIter &iter1, const DataIter &iter2) {
        return iter1.ptr<SchoolYear>()->start_year < iter2.ptr<SchoolYear>()->start_year;
    });

    return list;
}

List<DataIter> App::getAllClasses() {
    auto list = database.getAll([](const shared_ptr<Data> &ptr) {
        return ptr->data_type == DataType::Class;
    });
    list.sort([](const DataIter &iter1, const DataIter &iter2) {
        return iter1.ptr<Class>()->name < iter2.ptr<Class>()->name;
    });

    return list;
}

DataIter App::login(const string &username, const string &password) {
    auto account = database.get([&](const shared_ptr<Data> &ptr) {
        return (ptr->data_type == DataType::Student || ptr->data_type == DataType::Staff) &&
               dynamic_pointer_cast<Account>(ptr)->username == username;
    });

    if (account && account.ptr<Account>()->checkPassword(password))
        user_iter = account;

    return user_iter;
}

bool App::logout() {
    if (!user_iter) { return false; }

    user_iter = {};
    return true;
}

bool App::changePassword(const string &password) {
    if (!user_iter) { return false; }

    user_iter.ptr<Account>()->password = password;
    return true;
}

bool App::setDefaultSchoolYear(const Data::UID &year_uid) {
    auto year_iter = database.get(year_uid);
    if (!year_iter)
        return false;

    default_year_iter = year_iter;
    default_year_uid = year_iter.uid();

    return true;
}

bool App::setDefaultSchoolYear(const int start_year, const int end_year) {
    auto year_iter = database.get([&](const shared_ptr<Data> &ptr) {
        if (ptr->data_type != DataType::SchoolYear)
            return false;

        auto year_ptr = dynamic_pointer_cast<SchoolYear>(ptr);

        return year_ptr->start_year == start_year && year_ptr->end_year == end_year;
    });

    if (!year_iter) return false;

    default_year_iter = year_iter;
    default_year_uid = year_iter.uid();

    return true;
}

bool App::setDefaultSemester(const Data::UID &semester_uid) {
    if (!year())
        return false;

    auto semester_iter = database.get(semester_uid);
    if (!semester_iter || semester_iter.ptr<Semester>()->school_year != default_year_iter)
        return false;

    default_semester_iter = semester_iter;
    default_semester_uid = semester_iter.uid();

    return true;
}

bool App::setDefaultSemester(const int no) {
    auto semester_iter = year()->getSemester(no);

    if (!semester_iter) return false;

    default_semester_iter = semester_iter;
    default_semester_uid = semester_iter.uid();

    return true;
}

bool App::addDefaultSchoolYear(const shared_ptr<SchoolYear> &year) {
    auto years = getAllYears();
    if (years.any_of([&](const DataIter &iter) {
        auto year_ptr = iter.ptr<SchoolYear>();

        return year_ptr->start_year == year->start_year && year_ptr->end_year == year->end_year;
    }))
        return false;

    for (const auto &y: years)
        deleteSchoolYear(y);

    default_year_iter = database.add(year);
    default_year_uid = default_year_iter.uid();

    return true;
}

bool App::addDefaultSemester(const shared_ptr<Semester> &semester) {
    if (!year())
        return false;
    /// If semester with the same no. is already added.
    if (year()->getSemester(semester->no))
        return false;

    semester->school_year = default_year_iter;
    auto data = database.add(semester);
    year()->addSemester(data);
    default_semester_iter = data;
    default_semester_uid = default_semester_iter.uid();

    return true;
}

DataIter App::addClass(const shared_ptr<Class> &classroom) {
    /// If class with the same name already exists.
    if (getClass(classroom->name))
        return {};

    for (const auto &student_iter: classroom->students) {
        auto student = student_iter.ptr<Student>();

        /// If another student with the same student_id already exists in database.
        if (database.data.find_if([&](const shared_ptr<Data> &ptr) {
            if (ptr->data_type != DataType::Student && ptr->data_type != DataType::Staff)
                return false;
            if (dynamic_pointer_cast<Account>(ptr)->getUserType() != UserType::Student)
                return false;
            return dynamic_pointer_cast<Student>(ptr)->student_id == student->student_id;
        }) != database.data.end())

            return {};
    }

    /// Add students to database.
    for (const auto &student_iter: classroom->students) {
        auto student = student_iter.ptr<Student>();
        database.add(student);
    }

    auto data = database.add(classroom);

    return data;
}

DataIter App::addCourse(const shared_ptr<Course> &course) {
    if (!semester())
        return {};
    if (semester()->getCourse(course->id))
        return {};

    course->semester = default_semester_iter;
    auto data = database.add(course);
    semester()->addCourse(data);

    return data;
}

int App::addStudents(const CSVData &csv, const DataIter &classroom) {
    int count = 0;

    auto rows = csv.getData();
    auto headers = csv.getHeaders();

    for (const auto &row: rows) {
        auto student = make_shared<Student>(CSV::CSVToStudent(headers, row));
        if (student->valid())
            count += (bool) addStudent(student, classroom);
    }

    return count;
}

int App::addStudents(const CSVData &csv, const string &class_name) {
    int count = 0;

    auto rows = csv.getData();
    auto headers = csv.getHeaders();

    for (const auto &row: rows) {
        auto student = make_shared<Student>(CSV::CSVToStudent(headers, row));
        if (student->valid())
            count += (bool) addStudent(student, class_name);
    }

    return count;
}

int App::addStudents(const CSVData &csv, const Data::UID &class_uid) {
    int count = 0;

    auto rows = csv.getData();
    auto headers = csv.getHeaders();

    for (const auto &row: rows) {
        auto student = make_shared<Student>(CSV::CSVToStudent(headers, row));
        if (student->valid())
            count += (bool) addStudent(student, class_uid);
    }

    return count;
}

DataIter App::addStudent(const shared_ptr<Student> &student, const DataIter &classroom) {
    if (!classroom)
        return {};

    /// If another student with the same student_id already exists in database.
    if (database.data.find_if([&](const shared_ptr<Data> &ptr) {
        if (ptr->data_type != DataType::Student && ptr->data_type != DataType::Staff)
            return false;
        if (dynamic_pointer_cast<Account>(ptr)->getUserType() != UserType::Student)
            return false;
        return dynamic_pointer_cast<Student>(ptr)->student_id == student->student_id;
    }) != database.data.end())
        return {};

    student->classroom = classroom;
    auto data = database.add(student);
    classroom.ptr<Class>()->addStudent(data);

    return data;
}

DataIter App::addStudent(const shared_ptr<Student> &student, const string &class_name) {
    auto classroom = getClass(class_name);
    if (!classroom)
        return {};

    /// If another student with the same student_id already exists in database.
    if (database.data.find_if([&](const shared_ptr<Data> &ptr) {
        if (ptr->data_type != DataType::Student && ptr->data_type != DataType::Staff)
            return false;
        if (dynamic_pointer_cast<Account>(ptr)->getUserType() != UserType::Student)
            return false;
        return dynamic_pointer_cast<Student>(ptr)->student_id == student->student_id;
    }) != database.data.end())
        return {};

    student->classroom = classroom;
    auto data = database.add(student);
    classroom.ptr<Class>()->addStudent(data);

    return data;
}

DataIter App::addStudent(const shared_ptr<Student> &student, const Data::UID &class_uid) {
    auto classroom = getClass(class_uid);
    if (!classroom)
        return {};

    /// If another student with the same student_id already exists in database.
    if (database.data.find_if([&](const shared_ptr<Data> &ptr) {
        if (ptr->data_type != DataType::Student && ptr->data_type != DataType::Staff)
            return false;
        if (dynamic_pointer_cast<Account>(ptr)->getUserType() != UserType::Student)
            return false;
        return dynamic_pointer_cast<Student>(ptr)->student_id == student->student_id;
    }) != database.data.end())
        return {};

    student->classroom = classroom;
    auto data = database.add(student);
    classroom.ptr<Class>()->addStudent(data);

    return data;
}

bool App::deleteStudent(const DataIter &student) {
    if (!student)
        return false;

    auto student_ptr = student.ptr<Student>();
    auto classroom_ptr = student_ptr->classroom.ptr<Class>();

    if (!classroom_ptr)
        return false;

    /// Remove student from class
    classroom_ptr->removeStudent(student.uid());

    /// Remove student from courses
    for (const auto &course: student_ptr->courses) {
        auto course_ptr = course.ptr<Course>();
        course_ptr->removeStudent(student.uid());
    }

    database.remove(student);

    return true;
}

bool App::deleteCourse(const Data::UID &course_uid) {
    if (!semester())
        return false;

    auto course = semester()->getCourse(course_uid);
    if (!course)
        return false;

    deleteCourse(course);

    return true;
}

DataIter App::getClass(const string &name) {
    auto classes = getAllClasses();

    auto it = classes.find_if([&](const DataIter &iter) {
        return name == iter.ptr<Class>()->name;
    });

    if (it != classes.end()) return *it;
    return {};
}

DataIter App::getClass(const Data::UID &uid) {
    auto classes = getAllClasses();

    auto it = classes.find_if([&](const DataIter &iter) {
        return uid == iter.uid();
    });

    if (it != classes.end()) return *it;
    return {};
}

bool App::removeClassFromYear(const DataIter &classroom) {
    if (!year())
        return false;

    if (!classroom)
        return false;

    auto class_ptr = classroom.ptr<Class>();

    for (const auto &student_iter: class_ptr->students) {
        auto student_ptr = student_iter.ptr<Student>();

        /// Remove student from courses on this year.
        student_ptr->courses
                .filter([&](const DataIter &iter) {
                    auto semester_ptr = iter.ptr<Course>()->semester.ptr<Semester>();
                    return semester_ptr->school_year == default_year_iter;
                })
                .for_each([&](const DataIter &iter) {
                    auto course_ptr = iter.ptr<Course>();
                    course_ptr->removeStudent(student_iter.uid());
                });
        /// Remove courses from student.
        student_ptr->courses.remove_if([&](const DataIter &iter) {
            auto semester_ptr = iter.ptr<Course>()->semester.ptr<Semester>();
            return semester_ptr->school_year == default_year_iter;
        });
    }

    return true;
}

bool App::removeClassFromYear(const Data::UID &class_uid) {
    if (!year())
        return false;

    auto class_ptr = getClass(class_uid).ptr<Class>();
    if (!class_ptr)
        return false;

    for (const auto &student_iter: class_ptr->students) {
        auto student_ptr = student_iter.ptr<Student>();

        /// Remove student from courses on this year.
        student_ptr->courses
                .filter([&](const DataIter &iter) {
                    auto semester_ptr = iter.ptr<Course>()->semester.ptr<Semester>();
                    return semester_ptr->school_year == default_year_iter;
                })
                .for_each([&](const DataIter &iter) {
                    auto course_ptr = iter.ptr<Course>();
                    course_ptr->removeStudent(student_iter.uid());
                });
        /// Remove courses from student.
        student_ptr->courses.remove_if([&](const DataIter &iter) {
            auto semester_ptr = iter.ptr<Course>()->semester.ptr<Semester>();
            return semester_ptr->school_year == default_year_iter;
        });
    }

    return true;
}

bool App::deleteDefaultSemester() {
    if (!semester())
        return false;

    deleteSemester(default_semester_iter);
    default_semester_iter = {};

    return true;
}

bool App::deleteDefaultSchoolYear() {
    if (!year())
        return false;

    deleteSchoolYear(default_year_iter);
    default_year_iter = default_semester_iter = {};

    return true;
}

bool App::enroll(const DataIter &student, const Data::UID &course_uid) {
    if (!student)
        return false;
    if (!semester())
        return false;
    /// If no such course is found in default semester.
    auto course = semester()->getCourse(course_uid);
    if (!course)
        return false;
    /// If there are overlapping courses.
    if (!student.ptr<Student>()->getOverlappingCourses(course.ptr<Course>()->sessions).empty())
        return false;

    return student.ptr<Student>()->addCourse(course) && course.ptr<Course>()->addStudent(student);
}

bool App::enroll(const DataIter &student, const string &course_id) {
    if (!student)
        return false;
    if (!semester())
        return false;
    /// If no such course is found in default semester.
    auto course = semester()->getCourse(course_id);
    if (!course)
        return false;
    /// If there are overlapping courses.
    if (!student.ptr<Student>()->getOverlappingCourses(course.ptr<Course>()->sessions).empty())
        return false;

    return student.ptr<Student>()->addCourse(course) && course.ptr<Course>()->addStudent(student);
}

bool App::disenroll(const DataIter &student, const Data::UID &course_uid) {
    if (!student)
        return false;
    if (!semester())
        return false;
    /// If no such course is found in default semester.
    auto course = semester()->getCourse(course_uid);
    if (!course)
        return false;

    return student.ptr<Student>()->removeCourse(course.uid()) && course.ptr<Course>()->removeStudent(student.uid());
}

bool App::disenroll(const DataIter &student, const string &course_id) {
    if (!student)
        return false;
    if (!semester())
        return false;
    /// If no such course is found in default semester.
    auto course = semester()->getCourse(course_id);
    if (!course)
        return false;

    return student.ptr<Student>()->removeCourse(course.uid()) && course.ptr<Course>()->removeStudent(student.uid());
}

bool App::exitDefaultSchoolYear() {
    if (!year())
        return false;

    exitDefaultSemester();
    default_year_iter = DataIter();
    default_year_uid = Data::UID();

    return true;
}

bool App::exitDefaultSemester() {
    if (!semester())
        return false;

    default_semester_iter = DataIter();
    default_semester_uid = Data::UID();

    return true;
}

void App::deleteSchoolYear(const DataIter &year) {
    if (!year)
        return;

    auto year_ptr = year.ptr<SchoolYear>();

    for (const auto &semester: year_ptr->semesters) {
        deleteSemester(semester);
    }

    database.remove(year);
}

void App::deleteSemester(const DataIter &semester) {
    if (!semester)
        return;

    auto semester_ptr = semester.ptr<Semester>();

    semester_ptr->school_year.ptr<SchoolYear>()->removeSemester(semester.uid());
    for (const auto &course: semester_ptr->courses) {
        deleteCourse(course);
    }

    database.remove(semester);
}

bool App::deleteCourse(const DataIter &course) {
    if (!course)
        return false;

    auto course_ptr = course.ptr<Course>();

    course_ptr->semester.ptr<Semester>()->removeCourse(course.uid());
    for (const auto &student: course_ptr->students) {
        student.ptr<Student>()->removeCourse(course.uid());
    }

    database.remove(course);

    return true;
}

void App::deleteClass(const DataIter &classroom) {
    if (!classroom)
        return;

    auto class_ptr = classroom.ptr<Class>();

    for (const auto &student: class_ptr->students) {
        auto student_ptr = student.ptr<Student>();

        /// Remove student from all courses.
        for (const auto &course: student_ptr->courses) {
            auto course_ptr = course.ptr<Course>();
            course_ptr->removeStudent(student.uid());
        }

        database.remove(student);
    }

    database.remove(classroom);
}

DataIter App::getStudent(const string &student_id) {
    auto student_iter = database.get([&](const shared_ptr<Data> &ptr) {
        return (ptr->data_type == DataType::Student || ptr->data_type == DataType::Staff) &&
               dynamic_pointer_cast<Account>(ptr)->getUserType() == UserType::Student &&
               dynamic_pointer_cast<Student>(ptr)->student_id == student_id;
    });

    return student_iter;
}

Score App::CSV::CSVToScore(const List<string> &headers, const List<string> &row, const DataIter &course) {
    if (!course)
        return {};

    Score score(course);

    for (int i = 0; i < min(headers.size(), row.size()); i++) {
        try {
            auto header = headers[i];
            auto data = row[i];

            if (header.find("midterm") != string::npos) {
                for (int pos = (int) header.find(' '); pos != string::npos; pos = (int) header.find(' '))
                    header.erase(pos, 1);

                score.midterm = stod(data);
            } else if (header.find("final") != string::npos) {
                for (int pos = (int) header.find(' '); pos != string::npos; pos = (int) header.find(' '))
                    header.erase(pos, 1);

                score.final = stod(data);
            } else if (header.find("total") != string::npos) {
                for (int pos = (int) header.find(' '); pos != string::npos; pos = (int) header.find(' '))
                    header.erase(pos, 1);

                score.total = stod(data);
            } else if (header.find("other") != string::npos) {
                for (int pos = (int) header.find(' '); pos != string::npos; pos = (int) header.find(' '))
                    header.erase(pos, 1);

                score.other = stod(data);
            }
        }
        catch (exception &e) {
            cerr << e.what() << std::endl;
        }
    }

    return score;
}

Student App::CSV::CSVToStudent(const List<string> &headers, const List<string> &row) {
    string student_id = {}, social_id = {};
    FullName name = {};
    Gender gender = Gender::Unknown;
    tm birth = {};

    for (int i = 0; i < min(headers.size(), row.size()); i++) {
        try {
            auto header = headers[i];
            auto data = row[i];

            if (header.find("student") != string::npos && header.find("id") != string::npos) {
                student_id = data;
            } else if (header.find("social") != string::npos) {
                social_id = data;
            } else if (header.find("name") != string::npos && header.find("last") != string::npos) {
                name.last = data;
            } else if (header.find("name") != string::npos && header.find("first") != string::npos) {
                name.first = data;
            } else if (header.find("gender") != string::npos) {
                Utils::toLowerStr(data);
                if (data == "male")
                    gender = Gender::Male;
                else if (data == "female")
                    gender = Gender::Female;
                else if (data == "other")
                    gender = Gender::Other;
            } else if (header.find("birth") != string::npos) {
                for (int pos = (int) data.find(' '); pos != string::npos; pos = (int) data.find(' '))
                    data.erase(pos, 1);

                char sep[] = {'/', '-', '.', '\\'};
                for (const auto &c: sep) {
                    int s1 = (int) data.find(c), s2 = (int) data.find(c, s1 + 1);
                    if (s1 == string::npos || s2 == string::npos)
                        continue;

                    try {
                        int day = stoi(data.substr(0, s1));
                        int month = stoi(data.substr(s1 + 1, s2 - s1 - 1));
                        int year = stoi(data.substr(s2 + 1, data.size() - s2 - 1));
                        birth = Utils::mktm(day, month, year);

                        break;
                    }
                    catch (exception &e) {
                        cerr << e.what() << std::endl;
                    }
                }
            }
        }
        catch (exception &e) {
            cerr << e.what() << std::endl;

            return {};
        }
    }

    return {student_id, social_id, name, gender, birth};
}

void App::CSV::studentToCSV(const shared_ptr<Student> &student, CSVIO::CSVWriter &writer, const bool write_header) {
    if (write_header)
        writer << "Student ID" << "Last Name" << "First Name" << "Gender" << "Date of Birth" << "Social ID" << "Class";

    writer << student->student_id
           << student->name.last
           << student->name.first
           << genderStr[(int) student->gender]
           << Utils::dateToStr(student->birth)
           << student->social_id
           << student->classroom.ptr<Class>()->name;
}

void App::CSV::studentsToCSV(const List<DataIter> &students, CSVIO::CSVWriter &writer) {
    writer.resetContent();
    writer.newRow() << "No" << "Student ID" << "Last Name" << "First Name" << "Gender" << "Date of Birth"
                    << "Social ID" << "Class";

    int no = 0;
    for (const auto &iter: students) {
        if (!iter) continue;
        auto student = iter.ptr<Student>();

        writer.newRow();

        ++no;
        writer << no;
        studentToCSV(student, writer);
    }
}

int App::addScores(const CSVData &csv, const DataIter &course) {
    if (!course)
        return 0;

    int count = 0;

    auto rows = csv.getData();
    auto headers = csv.getHeaders();

    for (const auto &row: rows) {
        string student_id;
        FullName student_name;

        for (int i = 0; i < min(headers.size(), row.size()); i++) {
            try {
                auto header = headers[i];
                auto data = row[i];

                if (header.find("student") != string::npos && header.find("id") != string::npos) {
                    student_id = data;
                } else if (header.find("name") != string::npos && header.find("full") != string::npos) {
                    student_name.first = data;
                }

                auto student = course.ptr<Course>()->getStudent(student_id);
                if (student) {
                    auto student_score = student.ptr<Student>()->getScore(course.uid());
                    *student_score = CSV::CSVToScore(headers, row, course);
                    ++count;
                }
            }
            catch (exception &e) {
                cerr << e.what() << std::endl;
            }
        }
    }

    return count;
}

void App::exportStudentsInCourse(const DataIter &course, const string &file_path) {
    if (!course)
        return;

    fs::path path(file_path);
    fs::create_directories(path.parent_path());

    course.ptr<Course>()->sortStudentsByID();

    CSVIO::CSVWriter writer;
    CSV::studentsToCSV(course.ptr<Course>()->students, writer);
    writer.writeToFile(file_path);
}