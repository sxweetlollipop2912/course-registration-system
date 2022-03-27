#include <iostream>
#include <string>
#include <ctime>

#include "Database.h"
#include "SchoolYear.h"
#include "Semester.h"
#include "Course.h"
#include "Account.h"
#include "Class.h"
#include "App.h"
#include "List.h"

using std::make_shared, std::tm, std::dynamic_pointer_cast, std::to_string;

void output(App &app) {
    std::cout << "\n------------ OUTPUT ------------\n\n";

    if (app.year()) {
        std::cout << "Schoolyear " << app.year()->start_year << ' ' << app.year()->end_year << ": ";
        std::cout << "there r " << app.year()->semesters.size() << " semester(s).\n";
    }

    if (app.semester()) {
        auto semester = app.semester();
        std::cout << "  Default semester: no. " << semester->no << ": ";
        std::cout << semester->courses.size() << " course(s):\n";

        for (auto c: semester->courses) {
            auto c_ptr = c.ptr<Course>();
            std::cout << "      Course " << c_ptr->id << '\n';
            std::cout << "          name: " << c_ptr->name << '\n';
            std::cout << "          teacher: " << c_ptr->teacher_name.last << ' ' << c_ptr->teacher_name.first << '\n';

            std::mktime(&c_ptr->sessions[0].start);
            std::cout << "          1st session starts on (test output): " << std::asctime(&c_ptr->sessions[0].start)
                      << '\n';
        }
    }

    if (app.year()) {
        std::cout << "Classes:\n";
        for (auto c: app.year()->classes) {
            auto c_ptr = c.ptr<Class>();
            std::cout << "  " << c_ptr->name << ": " << c_ptr->students.size() << " student(s)\n";

            for (auto s: c_ptr->students) {
                auto s_ptr = s.ptr<Student>();
                std::cout << "      " << s_ptr->student_id << " - " << s_ptr->name.last << ' ' << s_ptr->name.first
                          << ", ";
                std::cout << "enrolling in " << s_ptr->courses.size() << " course(s):\n";
                for (auto course: s_ptr->courses) {
                    auto course_ptr = course.ptr<Course>();
                    std::cout << "          " << course_ptr->id << '\n';
                }
            }
        }
    }

    if (app.semester()) {
        std::cout << "Courses:\n";
        for (auto c: app.semester()->courses) {
            auto c_ptr = c.ptr<Course>();
            std::cout << "  " << c_ptr->id << ": " << c_ptr->students.size() << " student(s)\n";

            for (auto s: c_ptr->students) {
                auto s_ptr = s.ptr<Student>();
                std::cout << "      " << s_ptr->student_id << " - " << s_ptr->name.last << ' ' << s_ptr->name.first
                          << ", ";
                std::cout << "class" << s_ptr->classroom.ptr<Class>()->name << '\n';
            }
        }
    }
}

int main() {
    //list_examples();
    App app;

    {
        /// Create year 2021 - 2022
        auto default_year = make_shared<SchoolYear>(2021, 2022);
        app.addDefaultSchoolYear(default_year);


        /// Create semester 1, spans from October 1 2021 to December 31 2021
        auto semester1 = make_shared<Semester>(1, tm{}, tm{});
        app.addDefaultSemester(semester1);


        /// Create courses
        for(int i = 0; i < 5; i++) {
            auto course = make_shared<Course>("CS" + to_string(i),
                                             "Intro to CS",
                                             FullName("Ten", "Ho"),
                                             Course::Session(tm{}, tm{}),
                                             Course::Session(tm{}, tm{}),
                                             4,
                                             50);
            app.addCourse(course);
        }


        /// Create a class 21CTT1
        auto ctt1 = make_shared<Class>("21CTT1");
        app.addClass(ctt1);


        /// Create students
        for(int i = 0; i < 5; i++) {
            auto student_ptr = make_shared<Student>(std::to_string(i),
                                                    "012345678912",
                                                    FullName("Ten", "Ho"),
                                                    Gender::Male,
                                                    tm{});
            auto student = app.addStudent(student_ptr, "21CTT1");
            app.enroll(student, "CS" + to_string(i));
        }

        output(app);
        std::cout << "data count 1: " << app.database.size() << '\n';


        /// Remove course CS0.
        app.removeCourse(app.semester()->getCourseByID("CS0").uid());
        output(app);
        std::cout << "data count 2: " << app.database.size() << '\n';


        /// Remove student with id 1.
        app.removeStudent(ctt1->getStudentByID("1"));
        output(app);
        std::cout << "data count 3: " << app.database.size() << '\n';


        /// Disenroll student 2 from course CS2
        app.disenroll(ctt1->getStudentByID("2"), app.semester()->getCourseByID("CS2").uid());
        output(app);


        /// Remove class 21CTT1 from 2021-2022 schoolyear.
        app.removeClassFromYear(ctt1->uid);
        output(app);
        std::cout << "data count 4: " << app.database.size() << '\n';


        /// Remove semester 1 from 2021-2022 schoolyear.
        app.removeSemester(app.semester()->uid);
        output(app);
        std::cout << "data count 5: " << app.database.size() << '\n';


        /// Remove 2021-2022 schoolyear.
        app.removeSchoolYear();
        output(app);
        std::cout << "data count 6: " << app.database.size() << '\n';
    }

    app.database.clean();
    std::cout << "data count 7: " << app.database.size() << '\n';
}