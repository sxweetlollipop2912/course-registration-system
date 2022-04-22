#include "App.h"

using std::shared_ptr, std::dynamic_pointer_cast, std::static_pointer_cast;

DataIter App::addAccount(const shared_ptr<Account> &account) {
    if (database.data.find_if([&](const shared_ptr<Data> &ptr) {
        return ptr->data_type == DataType::Account && dynamic_pointer_cast<Account>(ptr)->username == account->username;
    }) != database.data.end()) {
        return {};
    }

    return database.add(account);
}

DataIter App::addStaff(const shared_ptr<Staff> &staff) {
    return addAccount(static_pointer_cast<Account>(staff));
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
    return database.getAll([](const shared_ptr<Data> &ptr) {
        return ptr->data_type == DataType::SchoolYear;
    });
}

List<DataIter> App::getAllClasses() {
    return database.getAll([](const shared_ptr<Data> &ptr) {
        return ptr->data_type == DataType::Class;
    });
}

DataIter App::login(const string &username, const string &password) {
    auto account = database.get([&](const shared_ptr<Data> &ptr) {
        return ptr->data_type == DataType::Account && dynamic_pointer_cast<Account>(ptr)->username == username;
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
    auto it = database.get([&](const shared_ptr<Data> &ptr) {
        if (ptr->data_type != DataType::SchoolYear)
            return false;

        auto year_ptr = dynamic_pointer_cast<SchoolYear>(ptr);

        return year_ptr->start_year == year->start_year && year_ptr->end_year == year->end_year;
    });

    if (it) return false;

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

    for(const auto& student_iter : classroom->students) {
        auto student = student_iter.ptr<Student>();

        /// If another student with the same student_id already exists in database.
        if (database.data.find_if([&](const shared_ptr<Data> &ptr) {
            if (ptr->data_type != DataType::Account)
                return false;
            if (dynamic_pointer_cast<Account>(ptr)->getUserType() != UserType::Student)
                return false;
            return dynamic_pointer_cast<Student>(ptr)->student_id == student->student_id;
        }) != database.data.end())

            return {};
    }

    /// Add students to database.
    for(const auto& student_iter : classroom->students) {
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

int App::addStudents(const CSVData &csv, const string &class_name) {
    int count = 0;

    auto rows = csv.getData();
    auto headers = csv.getHeaders();

    for(const auto& row : rows) {
        auto student = make_shared<Student>(Student::tryParse(headers, row));
        if (student->valid())
            count += (bool)addStudent(student, class_name);
    }

    return count;
}

int App::addStudents(const CSVData &csv, const Data::UID &class_uid) {
    int count = 0;

    auto rows = csv.getData();
    auto headers = csv.getHeaders();

    for(const auto& row : rows) {
        auto student = make_shared<Student>(Student::tryParse(headers, row));
        if (student->valid())
            count += (bool)addStudent(student, class_uid);
    }

    return count;
}

DataIter App::addStudent(const shared_ptr<Student> &student, const string &class_name) {
    auto classroom = getClass(class_name);
    if (!classroom)
        return {};

    /// If another student with the same student_id already exists in database.
    if (database.data.find_if([&](const shared_ptr<Data> &ptr) {
        if (ptr->data_type != DataType::Account)
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
        if (ptr->data_type != DataType::Account)
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
    auto student_ptr = student.ptr<Student>();
    auto classroom_ptr = student_ptr->classroom.ptr<Class>();

    if (!classroom_ptr)
        return false;

    /// Remove student from class
    classroom_ptr->removeStudent(student.uid());

    /// Remove student from courses
    for(const auto& course : student_ptr->courses) {
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

bool App::removeClassFromYear(const Data::UID &class_uid) {
    if (!year())
        return false;

    auto class_ptr = getClass(class_uid).ptr<Class>();
    if (!class_ptr)
        return false;

    for(const auto& student_iter : class_ptr->students) {
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

List<DataIter> App::getOverlappingCourses(const DataIter &student, const List<Course::Session> sessions) const {
    return student.ptr<Student>()->overlappingCourses(sessions);
}

bool App::enroll(const DataIter &student, const Data::UID &course_uid) {
    if (!semester())
        return false;
    /// If no such course is found in default semester.
    auto course = semester()->getCourse(course_uid);
    if (!course)
        return false;
    /// If there are overlapping courses.
    if (!student.ptr<Student>()->overlappingCourses(course.ptr<Course>()->sessions).empty())
        return false;

    return student.ptr<Student>()->addCourse(course) && course.ptr<Course>()->addStudent(student);
}

bool App::enroll(const DataIter &student, const string &course_id) {
    if (!semester())
        return false;
    /// If no such course is found in default semester.
    auto course = semester()->getCourse(course_id);
    if (!course)
        return false;
    /// If there are overlapping courses.
    if (!student.ptr<Student>()->overlappingCourses(course.ptr<Course>()->sessions).empty())
        return false;

    return student.ptr<Student>()->addCourse(course) && course.ptr<Course>()->addStudent(student);
}

bool App::disenroll(const DataIter &student, const Data::UID &course_uid) {
    if (!semester())
        return false;
    /// If no such course is found in default semester.
    auto course = semester()->getCourse(course_uid);
    if (!course)
        return false;

    return student.ptr<Student>()->removeCourse(course.uid()) && course.ptr<Course>()->removeStudent(student.uid());
}

bool App::disenroll(const DataIter &student, const string &course_id) {
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
    auto year_ptr = year.ptr<SchoolYear>();

    for(const auto& semester : year_ptr->semesters) {
        deleteSemester(semester);
    }

    database.remove(year);
}

void App::deleteSemester(const DataIter &semester) {
    auto semester_ptr = semester.ptr<Semester>();

    semester_ptr->school_year.ptr<SchoolYear>()->removeSemester(semester.uid());
    for(const auto& course : semester_ptr->courses) {
        deleteCourse(course);
    }

    database.remove(semester);
}

void App::deleteCourse(const DataIter &course) {
    auto course_ptr = course.ptr<Course>();

    course_ptr->semester.ptr<Semester>()->removeCourse(course.uid());
    for(const auto& student : course_ptr->students) {
        student.ptr<Student>()->removeCourse(course.uid());
    }

    database.remove(course);
}

void App::deleteClass(const DataIter &classroom) {
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

shared_ptr<Score> App::getScoreOfStudent(const string &student_id, const string &course_id) {
    auto student_iter = getStudent(student_id);
    if (!student_iter)
        return nullptr;

    auto student_ptr = student_iter.ptr<Student>();
    auto score_ptr = student_ptr->getScore(course_id);

    return score_ptr;
}

shared_ptr<Score> App::getScoreOfStudent(const string &student_id, const Data::UID &course_uid) {
    auto student_iter = getStudent(student_id);
    if (!student_iter)
        return nullptr;

    auto student_ptr = student_iter.ptr<Student>();
    auto score_ptr = student_ptr->getScore(course_uid);

    return score_ptr;
}

DataIter App::getStudent(const string &student_id) {
    auto student_iter = database.get([&](const shared_ptr<Data> &ptr) {
        return ptr->data_type == DataType::Account &&
                dynamic_pointer_cast<Account>(ptr)->getUserType() == UserType::Student &&
                dynamic_pointer_cast<Student>(ptr)->student_id == student_id;
    });

    return student_iter;
}
