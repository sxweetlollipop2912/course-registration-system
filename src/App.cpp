#include "App.h"

using std::shared_ptr, std::dynamic_pointer_cast;

List<DataIter> App::getAllYears() {
    return database.getAll([](const shared_ptr<Data> &ptr) {
        return ptr->data_type == DataType::SchoolYear;
    });
}

bool App::setDefaultSchoolYear(const Data::UID &year_uid) {
    auto year_iter = database.getByUID(year_uid);
    if (!year_iter)
        return false;

    default_year = year_iter;
    default_year_uid = default_year.uid();

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

    default_year = year_iter;
    default_year_uid = default_year.uid();

    return true;
}

bool App::setDefaultSemester(const Data::UID &semester_uid) {
    if (!year())
        return false;

    auto semester_iter = database.getByUID(semester_uid);
    if (!semester_iter || semester_iter.ptr<Semester>()->school_year != default_year)
        return false;

    default_semester = semester_iter;
    default_semester_uid = default_semester.uid();

    return true;
}

bool App::setDefaultSemester(const int no) {
    auto semester_iter = year()->getSemesterByNo(no);

    if (!semester_iter) return false;

    default_semester = semester_iter;
    default_semester_uid = default_semester.uid();

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

    default_year = database.add(year);
    default_year_uid = default_year.uid();

    return true;
}

bool App::addDefaultSemester(const shared_ptr<Semester> &semester) {
    if (!year())
        return false;
    /// If semester with the same no. is already added.
    if (year()->getSemesterByNo(semester->no))
        return false;

    semester->school_year = default_year;
    auto data = database.add(semester);
    year()->addSemester(data);
    default_semester = data;
    default_semester_uid = default_semester.uid();

    return true;
}

DataIter App::addClass(const shared_ptr<Class> &classroom) {
    if (!year())
        return {};
    /// If class with the same name is already added to default schoolyear.
    if (year()->getClassByName(classroom->name))
        return {};

    auto data = database.add(classroom);
    year()->addClass(data);

    return data;
}

DataIter App::addCourse(const shared_ptr<Course> &course) {
    if (!semester())
        return {};
    if (semester()->getCourseByID(course->id))
        return {};

    course->semester = default_semester;
    auto data = database.add(course);
    semester()->addCourse(data);

    return data;
}

DataIter App::addStudent(const shared_ptr<Student> &student, const string &class_name) {
    if (!year())
        return {};

    auto classroom = year()->getClassByName(class_name);
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
    if (!year())
        return {};

    auto classroom = year()->getClassByUID(class_uid);
    if (!classroom || classroom.ptr<Class>()->getStudentByID(student->student_id))
        return {};

    student->classroom = classroom;
    auto data = database.add(student);
    classroom.ptr<Class>()->addStudent(data);

    return data;
}

bool App::deleteStudent(const DataIter &student) {
    auto student_ptr = student.ptr<Student>();
    auto classroom_ptr = student_ptr->classroom.ptr<Class>();

    if (!year())
        return false;
    if (!classroom_ptr)
        return false;
    if (year()->classes.find(student_ptr->classroom) == year()->classes.end())
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

    auto course = semester()->getCourseByUID(course_uid);
    if (!course)
        return false;

    /// Remove course from default semester.
    semester()->removeCourse(course_uid);
    /// Remove course from all enrolled students.
    for(const auto& student_iter : course.ptr<Course>()->students) {
        student_iter.ptr<Student>()->removeCourse(course.uid());
    }

    database.remove(course);

    return true;
}

bool App::removeClassFromYear(const Data::UID &class_uid) {
    if (!year())
        return false;

    auto class_ptr = year()->getClassByUID(class_uid).ptr<Class>();
    if (!class_ptr)
        return false;

    for(const auto& student_iter : class_ptr->students) {
        auto student_ptr = student_iter.ptr<Student>();

        /// Remove student from courses on this year.
        student_ptr->courses
        .filter([&](const DataIter &iter) {
            auto semester_ptr = iter.ptr<Course>()->semester.ptr<Semester>();
            return semester_ptr->school_year == default_year;
        })
        .for_each([&](const DataIter &iter) {
            auto course_ptr = iter.ptr<Course>();
            course_ptr->removeStudent(student_iter.uid());
        });
        /// Remove courses from student.
        student_ptr->courses.remove_if([&](const DataIter &iter) {
            auto semester_ptr = iter.ptr<Course>()->semester.ptr<Semester>();
            return semester_ptr->school_year == default_year;
        });
    }

    /// Remove class from schoolyear.
    year()->removeClass(class_uid);

    return true;
}

bool App::deleteSemester(const Data::UID &semester_uid) {
    if (!year())
        return false;

    auto semester_iter = year()->getSemesterByUID(semester_uid);
    if (!semester_iter)
        return false;

    auto semester_ptr = semester_iter.ptr<Semester>();
    for(const auto& course_iter : semester_ptr->courses) {
        /// Remove course from all enrolled students.
        for(const auto& student_iter : course_iter.ptr<Course>()->students) {
            student_iter.ptr<Student>()->removeCourse(course_iter.uid());
        }
    }
    /// Remove all courses from semester.
    semester_ptr->courses.clear();
    /// Remove semester from default schoolyear.
    year()->removeSemester(semester_iter.uid());

    if (semester_uid == default_semester.uid())
        default_semester = {};

    database.remove(semester_iter);

    return true;
}

bool App::deleteSchoolYear() {
    if (!year())
        return false;

    for(const auto& semester_iter : year()->semesters) {
        auto semester_ptr = semester_iter.ptr<Semester>();

        for(const auto& course_iter : semester_ptr->courses) {
            /// Remove course from all enrolled students.
            for(const auto& student_iter : course_iter.ptr<Course>()->students) {
                student_iter.ptr<Student>()->removeCourse(course_iter.uid());
            }
        }
        /// Remove all courses from semester.
        semester_ptr->courses.clear();
        database.remove(semester_iter);
    }

    year()->semesters.clear();
    database.remove(default_year);
    default_year = default_semester = {};

    return true;
}

bool App::enroll(const DataIter &student, const Data::UID &course_uid) {
    if (!semester())
        return false;
    /// If no such course is found in default semester.
    auto course = semester()->getCourseByUID(course_uid);
    if (!course)
        return false;
    if (!year()->getClassByUID(student.ptr<Student>()->classroom.uid()))
        return false;

    return student.ptr<Student>()->addCourse(course) && course.ptr<Course>()->addStudent(student);
}

bool App::enroll(const DataIter &student, const string &course_id) {
    if (!semester())
        return false;
    /// If no such course is found in default semester.
    auto course = semester()->getCourseByID(course_id);
    if (!course)
        return false;
    if (!year()->getClassByUID(student.ptr<Student>()->classroom.uid()))
        return false;

    return student.ptr<Student>()->addCourse(course) && course.ptr<Course>()->addStudent(student);
}

bool App::disenroll(const DataIter &student, const Data::UID &course_uid) {
    if (!semester())
        return false;
    /// If no such course is found in default semester.
    auto course = semester()->getCourseByUID(course_uid);
    if (!course)
        return false;

    return student.ptr<Student>()->removeCourse(course.uid()) && course.ptr<Course>()->removeStudent(student.uid());
}

bool App::disenroll(const DataIter &student, const string &course_id) {
    if (!semester())
        return false;
    /// If no such course is found in default semester.
    auto course = semester()->getCourseByID(course_id);
    if (!course)
        return false;

    return student.ptr<Student>()->removeCourse(course.uid()) && course.ptr<Course>()->removeStudent(student.uid());
}

bool App::exitDefaultSchoolYear() {
    if (!year())
        return false;

    exitDefaultSemester();
    default_year = DataIter();
    default_year_uid = Data::UID();

    return true;
}

bool App::exitDefaultSemester() {
    if (!semester())
        return false;

    default_semester = DataIter();
    default_semester_uid = Data::UID();

    return true;
}
