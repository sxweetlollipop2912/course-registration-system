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
#include "TestingScenes.h"
#include "List.h"
#include "Utils.h"
#include "CSV.h"

using std::make_shared, std::tm, std::dynamic_pointer_cast, std::to_string;

void output(App &app) {
    std::cout << "\n------------ OUTPUT ------------\n\n";
    std::cout << "Username: " << app.user()->username << ", pass: " << app.user()->password << "\n\n";

    if (app.year()) {
        std::cout << "Schoolyear " << app.year()->start_year << ' ' << app.year()->end_year << ": ";
        std::cout << "there r " << app.year()->semesters.size() << " semester(s).\n";
    }

    if (app.semester()) {
        auto semester = app.semester();
        std::cout << "  Default semester: no. " << semester->no << ": ";
        std::cout << semester->courses.size() << " course(s):\n";

        for (const auto& c: semester->courses) {
            auto c_ptr = c.ptr<Course>();
            std::cout << "      Course " << c_ptr->id << '\n';
            std::cout << "          name: " << c_ptr->name << '\n';
            std::cout << "          teacher: " << c_ptr->teacher_name.toStr() << '\n';

            std::cout << "          1st session: " << Utils::sessionToStr(c_ptr->sessions[0].start) << " - " << Utils::sessionToStr(c_ptr->sessions[0].end) << '\n';
            std::cout << "          2nd session: " << Utils::sessionToStr(c_ptr->sessions[1].start) << " - " << Utils::sessionToStr(c_ptr->sessions[1].end) << '\n';
        }
    }

    if (app.year()) {
        std::cout << "Classes:\n";
        for (const auto& c: app.getAllClasses()) {
            auto c_ptr = c.ptr<Class>();
            std::cout << "  " << c_ptr->name << ": " << c_ptr->students.size() << " student(s)\n";

            for (const auto& s: c_ptr->students) {
                auto s_ptr = s.ptr<Student>();
                std::cout << "      " << s_ptr->student_id << " - " << s_ptr->name.toStr() << ", ";
                std::cout << (s_ptr->gender == Gender::Male? "male, " : "female, ");
                std::cout << "social: " << s_ptr->social_id << ", ";
                std::cout << "birth: " << Utils::dateToStr(s_ptr->birth) << ", ";

                std::cout << "enrolling in " << s_ptr->courses.size() << " course(s):\n";
                for (const auto& course: s_ptr->courses) {
                    auto course_ptr = course.ptr<Course>();
                    std::cout << "          " << course_ptr->id << '\n';
                }
            }
        }
    }

    if (app.semester()) {
        std::cout << "Courses:\n";
        for (const auto& c: app.semester()->courses) {
            auto c_ptr = c.ptr<Course>();
            std::cout << "  " << c_ptr->id << ": " << c_ptr->students.size() << " student(s)\n";

            for (const auto& s: c_ptr->students) {
                auto s_ptr = s.ptr<Student>();
                std::cout << "      " << s_ptr->student_id << " - " << s_ptr->name.last << ' ' << s_ptr->name.first
                          << ", ";
                std::cout << "class " << s_ptr->classroom.ptr<Class>()->name << ", ";
                std::cout << "total " << s_ptr->getScore(c_ptr->uid)->total << ", ";
                std::cout << "final " << s_ptr->getScore(c_ptr->uid)->final << ", ";
                std::cout << "midterm " << s_ptr->getScore(c_ptr->uid)->midterm << ", ";
                std::cout << "other " << s_ptr->getScore(c_ptr->uid)->other << '\n';
            }
        }
    }
}

int main() {
//    list_examples();

//    App app;
//    Scenes scenes(app);
//    scenes.run();

    App app;
    /// Add admin account ("admin", "admin").
    app.addStaff(make_shared<Staff>(ACCOUNT::ADMIN_USERNAME, ACCOUNT::ADMIN_PASS));
    /// Login with admin account.
    app.login(ACCOUNT::ADMIN_USERNAME, ACCOUNT::ADMIN_PASS);

    {
        /// Create year 2021 - 2022
        auto default_year = make_shared<SchoolYear>(2021, 2022);
        app.addDefaultSchoolYear(default_year);


        /// Create semester 1, spanning from October 1 2021 to December 31 2021
        auto semester1 = make_shared<Semester>(1,
                                               Utils::mktm(1, 10, 2021),
                                               Utils::mktm(31, 12, 2021));
        app.addDefaultSemester(semester1);

        /// Create a registration session spanning from October 1st, 2021 to October 1st, 2022.
        app.semester()->reg_session.start = Utils::mktm(1, 10, 2021);
        app.semester()->reg_session.end = Utils::mktm(1, 10, 2022);


        /// Create courses
        for(int i = 0; i < 4; i++) {
            auto course = make_shared<Course>("CS" + to_string(i),
                                             "Intro to CS",
                                             FullName("Ten", "Ho"),
                                             Course::Session(Utils::mksession(2, 7, 30), Utils::mksession(2, 9, 10)),
                                             Course::Session(Utils::mksession(7, 7, 30), Utils::mksession(7, 9, 10)),
                                             4,
                                             50);
            app.addCourse(course);
        }
        auto course = make_shared<Course>("CS4",
                                          "Intro to CS",
                                          FullName("Ten", "Ho"),
                                          Course::Session(Utils::mksession(3, 7, 30), Utils::mksession(3, 9, 10)),
                                          Course::Session(Utils::mksession(6, 8, 30), Utils::mksession(6, 10, 10)),
                                          4,
                                          50);
        app.addCourse(course);


        /// Create a class 21CTT1
        auto ctt1 = make_shared<Class>("21CTT1");
        app.addClass(ctt1);


        /// Create students
        for(int i = 0; i < 5; i++) {
            auto student_ptr = make_shared<Student>(std::to_string(i),
                                                    "012345678912",
                                                    FullName("Ten", "Ho"),
                                                    Gender::Male,
                                                    Utils::mktm(1, 1, 2003));
            auto student = app.addStudent(student_ptr, "21CTT1");
            app.enroll(student, "CS" + to_string(i));
            app.enroll(student, "CS4");
        }

        /// CSV tests
        {
            app.addClass(make_shared<Class>("21CTT1"));
            app.addClass(make_shared<Class>("21CTT2"));
            app.addClass(make_shared<Class>("21CTT3"));

            auto csvData = CSVIO::tryParse("./csv/21CTT1.csv");
            app.addStudents(csvData, "21CTT1");

            csvData = CSVIO::tryParse("./csv/21CTT2.csv");
            app.addStudents(csvData, "21CTT2");

            csvData = CSVIO::tryParse("./csv/21CTT3.csv");
            app.addStudents(csvData, "21CTT3");
        }

        /// Check if registration session is ongoing.
        std::cout << "Is registration session ongoing: " << app.semester()->reg_session.isOngoing() << '\n';
        std::cout << Utils::tmToStr(app.semester()->reg_session.start) << " - " << Utils::tmToStr(app.semester()->reg_session.end) << '\n';

        output(app);
        std::cout << "data count 1: " << app.database.size() << '\n';


        /// Enroll student 0 to CS0 (fail, due to overlapping courses).
        /// 0
        std::cout << '\n' << app.enroll(app.getStudent("0"), "CS0") << '\n';
        /// 1
        std::cout << app.getStudent("0").ptr<Student>()->getOverlappingCourses(
                app.semester()->getCourse("CS0").ptr<Course>()->sessions)
                .size() << "\n\n";


        /// Import score from CSV to course CS4
        auto csvData = CSVIO::tryParse("./csv/CS4.csv");
        app.semester()->getCourse("CS4").ptr<Course>()->tryParseScore(csvData);
        output(app);


        /// Remove course CS0.
        app.deleteCourse(app.semester()->getCourse("CS0").uid());
        output(app);
        std::cout << "data count 2: " << app.database.size() << '\n';


        /// Remove student with id 1.
        app.deleteStudent(ctt1->getStudent("1"));
        output(app);
        std::cout << "data count 3: " << app.database.size() << '\n';


        /// Disenroll student 2 from course CS2
        app.disenroll(ctt1->getStudent("2"), app.semester()->getCourse("CS2").uid());
        output(app);


        /// Remove class 21CTT1 from 2021-2022 schoolyear.
        app.removeClassFromYear(ctt1->uid);
        output(app);
        std::cout << "data count 4: " << app.database.size() << '\n';


        /// Remove semester 1 from 2021-2022 schoolyear.
        app.deleteDefaultSemester();
        output(app);
        std::cout << "data count 5: " << app.database.size() << '\n';


        /// Remove 2021-2022 schoolyear.
        app.deleteDefaultSchoolYear();
        output(app);
        std::cout << "data count 6: " << app.database.size() << '\n';
    }

    app.database.clean();
    std::cout << "data count 7: " << app.database.size() << '\n';
}