#include <iostream>
#include <string>
#include <ctime>

#include "Database.h"
#include "SchoolYear.h"
#include "Semester.h"
#include "Course.h"
#include "Account.h"
#include "Class.h"
#include "List.h"

using std::make_shared, std::tm, std::dynamic_pointer_cast;

int main() {
    //examples();

    /// Init database
    auto database = Database();


    /// Create year 2021 - 2022
    auto default_year = database.add(make_shared<SchoolYear>(2021, 2022));


    /// Create semester 1, spans from October 1 2021 to December 31 2021
    tm st{}, en{};
    // October 1 2021
    st.tm_mday = 1; st.tm_mon = 9; st.tm_year = 2021 - 1900;
    // December 31 2021
    en.tm_mday = 31; en.tm_mon = 11; en.tm_year = 2021 - 1900;

    auto semester1 = database.add(make_shared<Semester>(1, st, en));
    default_year.ptr<SchoolYear>()->addSemester(semester1);


    /// Create course CS162, with 2 sessions (7h30 - 9h30 Mon, 7h30 - 9h30 Sat)
    tm s11{}, s12{}, s21{}, s22{};
    // 7h30 Mon
    s11.tm_min = 30; s11.tm_hour = 7; s11.tm_wday = 1;
    // 9h30 Mon
    s12.tm_min = 30; s12.tm_hour = 9; s12.tm_wday = 1;
    // 7h30 Sat
    s21.tm_min = 30; s21.tm_hour = 7; s21.tm_wday = 6;
    // 9h30 Sat
    s22.tm_min = 30; s22.tm_hour = 9; s22.tm_wday = 6;

    auto cs162 = database.add(make_shared<Course>(semester1,
                                                  "CS162",
                                                  "Intro to CS",
                                                  FullName("Ten", "Ho"),
                                                  Course::Session(s11, s12),
                                                  Course::Session(s21, s22),
                                                  4,
                                                  50));
    semester1.ptr<Semester>()->addCourse(cs162);


    /// Create a class 21CTT1
    auto class_ctt1 = database.add(make_shared<Class>("21CTT1"));
    default_year.ptr<SchoolYear>()->addClass(class_ctt1);


    /// Create a student, id 21122211
    // mock date
    tm birth{};
    auto student = database.add(make_shared<Student>("21122211",
                                                     "012345678912",
                                                     FullName("Ten", "Ho"),
                                                     Gender::Male,
                                                     birth,
                                                     default_year.ptr<SchoolYear>()->getClassByName("21CTT1")));
    class_ctt1.ptr<Class>()->addStudent(student);
    student.ptr<Student>()->enroll(semester1.ptr<Semester>()->getCourseByID("CS162"));


    /// -------------- Output ---

    std::cout << "Schoolyear " << default_year.ptr<SchoolYear>()->start_year << ' ' << default_year.ptr<SchoolYear>()->end_year << ": ";
    std::cout << "there r " << default_year.ptr<SchoolYear>()->semesters.size() << " semester(s).\n";

    for(auto s : default_year.ptr<SchoolYear>()->semesters) {
        auto s_ptr = s.ptr<Semester>();
        std::cout << "  Semester " << s_ptr->no << ": ";
        std::cout << s_ptr->courses.size() << " course(s):\n";

        for(auto c : s_ptr->courses) {
            auto c_ptr = c.ptr<Course>();
            std::cout << "      Course " << c_ptr->id << '\n';
            std::cout << "          name: " << c_ptr->name << '\n';
            std::cout << "          teacher: " << c_ptr->teacher_name.last << ' ' << c_ptr->teacher_name.first << '\n';

            std::mktime(&c_ptr->sessions[0].start);
            std::cout << "          1st session starts on (test output): " << std::asctime(&c_ptr->sessions[0].start) << '\n';
        }
    }

    std::cout << "\nClasses:\n";
    for(auto c : default_year.ptr<SchoolYear>()->classes) {
        auto c_ptr = c.ptr<Class>();
        std::cout << "  " << c_ptr->name << ": " << c_ptr->students.size() << " student(s)\n";

        for(auto s : c_ptr->students) {
            auto s_ptr = s.ptr<Student>();
            std::cout << "      " << s_ptr->student_id << " - " << s_ptr->name.last << ' ' << s_ptr->name.first << ", ";
            std::cout << " enrolling in " << s_ptr->courses.size() << " course(s):\n";
            for(auto course : s_ptr->courses) {
                auto course_ptr = course.ptr<Course>();
                std::cout << "          " << course_ptr->name << '\n';
            }
        }
    }
}