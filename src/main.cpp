#include <iostream>
#include <string>
#include <ctime>

#include "App/Database.h"
#include "App/SchoolYear.h"
#include "App/Semester.h"
#include "App/Course.h"
#include "App/Account.h"
#include "App/Class.h"
#include "App/App.h"
#include "App/Utils.h"
#include "App/CSV.h"
#include "GUI/Scene0.h"
#include "GUI/Scene9.h"
#include "GUI/Scene1.h"
#include "GUI/Scene2.h"
#include "GUI/Scene5.h"
#include "GUI/Scene3.h"
#include "GUI/Scene10.h"
#include "GUI/Scene11.h"
#include "GUI/Scene7.h"
#include "GUI/SceneManager.h"


using std::make_shared, std::tm, std::dynamic_pointer_cast, std::to_string;

void output(App &app) {
    std::cout << "\n------------ OUTPUT ------------\n\n";
    //std::cout << "Username: " << app.user()->username << ", pass: " << app.user()->password << "\n\n";

    if (app.year()) {
        std::cout << "Schoolyear " << app.year()->start_year << ' ' << app.year()->end_year << ": ";
        std::cout << "there r " << app.year()->semesters.size() << " semester(s).\n";
    }

    if (app.semester()) {
        auto semester = app.semester();
        std::cout << "  Default semester: no. " << semester->no << ": ";
        std::cout << semester->courses.size() << " course(s):\n";

        for (const auto &c: semester->courses) {
            auto c_ptr = c.ptr<Course>();
            std::cout << "      Course " << c_ptr->id << '\n';
            std::cout << "          name: " << c_ptr->name << '\n';
            std::cout << "          teacher: " << c_ptr->teacher_name.toStr() << '\n';

            std::cout << "          1st session: " << Utils::sessionToStr(c_ptr->sessions[0].start) << " - "
                      << Utils::sessionToStr(c_ptr->sessions[0].end) << '\n';
            std::cout << "          2nd session: " << Utils::sessionToStr(c_ptr->sessions[1].start) << " - "
                      << Utils::sessionToStr(c_ptr->sessions[1].end) << '\n';
        }
    }

    if (app.year()) {
        std::cout << "Classes:\n";
        for (const auto &c: app.getAllClasses()) {
            auto c_ptr = c.ptr<Class>();
            std::cout << "  " << c_ptr->name << ": " << c_ptr->students.size() << " student(s)\n";

            for (const auto &s: c_ptr->students) {
                auto s_ptr = s.ptr<Student>();
                std::cout << "      " << s_ptr->student_id << " - " << s_ptr->name.toStr() << ", ";
                std::cout << (s_ptr->gender == Gender::Male ? "male, " : "female, ");
                std::cout << "social: " << s_ptr->social_id << ", ";
                std::cout << "birth: " << Utils::dateToStr(s_ptr->birth) << ", ";

                std::cout << "enrolling in " << s_ptr->courses.size() << " course(s):\n";
                for (const auto &course: s_ptr->courses) {
                    auto course_ptr = course.ptr<Course>();
                    std::cout << "          " << course_ptr->id << '\n';
                }
            }
        }
    }

    if (app.semester()) {
        std::cout << "Courses:\n";
        for (const auto &c: app.semester()->courses) {
            auto c_ptr = c.ptr<Course>();
            std::cout << "  " << c_ptr->id << ": " << c_ptr->students.size() << " student(s)\n";

            for (const auto &s: c_ptr->students) {
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
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Courses", sf::Style::Close);
    App app;
    {
        /// Add admin account ("admin", "admin").
        app.addStaff(make_shared<Staff>(ACCOUNT::ADMIN_USERNAME, ACCOUNT::ADMIN_PASS));
        /// Login with admin account.
        app.login(ACCOUNT::ADMIN_USERNAME, ACCOUNT::ADMIN_PASS);

        if (!app.load()) {
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
            for (int i = 8; i >= 0; i--) {
                auto course = make_shared<Course>("CS" + to_string(i),
                                                  "Intro to CS",
                                                  FullName("Ten", "Ho"),
                                                  Course::Session(Utils::mksession(2, 7, 30),
                                                                  Utils::mksession(2, 9, 10)),
                                                  Course::Session(Utils::mksession(7, 7, 30),
                                                                  Utils::mksession(7, 9, 10)),
                                                  4,
                                                  50);
                app.addCourse(course);
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

//                csvData = CSVIO::tryParse("./csv/21CTT3.csv");
//                app.addStudents(csvData, "21CTT3");

                csvData = CSVIO::tryParse("./csv/CS4.csv");
                app.addScores(csvData, app.semester()->getCourse("CS4"));
            }

            app.enroll(app.getStudent("21280009"), "CS0");
            app.enroll(app.getStudent("21280001"), "CS0");
            app.enroll(app.getStudent("21280012"), "CS0");
            app.enroll(app.getStudent("21280007"), "CS0");
            app.enroll(app.getStudent("21280011"), "CS0");
            app.enroll(app.getStudent("21280004"), "CS0");
            app.enroll(app.getStudent("21280010"), "CS0");
            app.enroll(app.getStudent("21280006"), "CS0");
            app.enroll(app.getStudent("21280008"), "CS0");
            app.enroll(app.getStudent("21280003"), "CS0");
        }

        output(app);
    }


    /// Add admin account ("admin", "admin").
    app.addStaff(make_shared<Staff>(ACCOUNT::ADMIN_USERNAME, ACCOUNT::ADMIN_PASS));

    //app.login("21280009", ACCOUNT::DEFAULT_PASS);
    app.scenes.push(SceneType::Scene0);

    while (!app.scenes.empty()) {
        app.scenes.refresh = false;
        auto sceneNow = app.scenes.top();

        if (sceneNow == SceneType::Scene0) {
            scene0(window, app);
        }
        if (sceneNow == SceneType::Scene1) {
            scene1(window, app);
        }
        if (sceneNow == SceneType::Scene2) {
            scene2(window, app);
        }
        if (sceneNow == SceneType::Scene3) {
            scene3(window, app);
        }
        if (sceneNow == SceneType::Scene4) {
            scene4(window, app);
        }
        if (sceneNow == SceneType::Scene5) {
            scene5(window, app);
        }
        if (sceneNow == SceneType::Scene7) {
            scene7(window, app);
        }
        if (sceneNow == SceneType::Scene9) {
            scene9(window, app);
        }
        if (sceneNow == SceneType::Scene10) {
            scene10(window, app);
        }
        if (sceneNow == SceneType::Scene11) {
            scene11(window, app);
        }
        if (sceneNow == SceneType::SceneModifyReg) {
            modify_registration_scene(window, app);
        }
        if (sceneNow == SceneType::SceneAddCourse) {
            add_course_scene(window, app);
        }
        if (sceneNow == SceneType::SceneModifyCourse) {
            modify_course_scene(window, app);
        }
    }

    std::cout << "saving " << app.save() << '\n';
}