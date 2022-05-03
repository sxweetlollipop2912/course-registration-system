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
#include "GUI/Scene8.h"
#include "GUI/SceneManager.h"


using std::make_shared, std::tm, std::dynamic_pointer_cast, std::to_string;


int main() {
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Courses", sf::Style::Close);
    App app;
    std::cout << "loading " << app.load() << '\n';
//    {
//        /// Add admin account ("admin", "admin").
//        app.addStaff(make_shared<Staff>(ACCOUNT::ADMIN_USERNAME, ACCOUNT::ADMIN_PASS));
//        /// Login with admin account.
//        app.login(ACCOUNT::ADMIN_USERNAME, ACCOUNT::ADMIN_PASS);
//
//        if (!app.load()) {
//            /// Create year 2021 - 2022
//            auto default_year = make_shared<SchoolYear>(2021, 2022);
//            app.addDefaultSchoolYear(default_year);
//
//            /// Create semester 1, spanning from October 1 2021 to December 31 2021
//            auto semester1 = make_shared<Semester>(1,
//                                                   Utils::mktm(1, 10, 2021),
//                                                   Utils::mktm(31, 12, 2021));
//            app.addDefaultSemester(semester1);
//
//            /// Create a registration session spanning from October 1st, 2021 to October 1st, 2022.
//            app.semester()->reg_session.start = Utils::mktm(1, 10, 2021);
//            app.semester()->reg_session.end = Utils::mktm(1, 10, 2022);
//
//            /// Create courses
//            for (int i = 8; i >= 0; i--) {
//                auto course = make_shared<Course>("CS" + to_string(i),
//                                                  "Intro to CS",
//                                                  FullName("Ten", "Ho"),
//                                                  Course::Session(Utils::mksession(2, 7, 30),
//                                                                  Utils::mksession(2, 9, 10)),
//                                                  Course::Session(Utils::mksession(7, 7, 30),
//                                                                  Utils::mksession(7, 9, 10)),
//                                                  4,
//                                                  50);
//                app.addCourse(course);
//            }
//
//            /// CSV tests
//            {
//                app.addClass(make_shared<Class>("21CTT1"));
//                app.addClass(make_shared<Class>("21CTT2"));
//                app.addClass(make_shared<Class>("21CTT3"));
//
//                auto csvData = CSVIO::tryParse("./csv/21CTT1.csv");
//                app.addStudents(csvData, "21CTT1");
//
//                csvData = CSVIO::tryParse("./csv/21CTT2.csv");
//                app.addStudents(csvData, "21CTT2");
//
////                csvData = CSVIO::tryParse("./csv/21CTT3.csv");
////                app.addStudents(csvData, "21CTT3");
//
//                csvData = CSVIO::tryParse("./csv/CS4.csv");
//                app.addScores(csvData, app.semester()->getCourse("CS4"));
//            }
//
//            app.enroll(app.getStudent("21280009"), "CS0");
//            app.enroll(app.getStudent("21280001"), "CS0");
//            app.enroll(app.getStudent("21280012"), "CS0");
//            app.enroll(app.getStudent("21280007"), "CS0");
//            app.enroll(app.getStudent("21280011"), "CS0");
//            app.enroll(app.getStudent("21280004"), "CS0");
//            app.enroll(app.getStudent("21280010"), "CS0");
//            app.enroll(app.getStudent("21280006"), "CS0");
//            app.enroll(app.getStudent("21280008"), "CS0");
//            app.enroll(app.getStudent("21280003"), "CS0");
//        }
//    }


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
        if (sceneNow == SceneType::Scene8) {
            scene8(window, app);
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
            modifyRegistrationScene(window, app);
        }
        if (sceneNow == SceneType::SceneAddCourse) {
            addCourseScene(window, app);
        }
        if (sceneNow == SceneType::SceneModifyCourse) {
            modifyCourseScene(window, app);
        }
        if (sceneNow == SceneType::SceneModifyScore) {
            modifyScoreScene(window, app);
        }
        if (sceneNow == SceneType::ImportScoreScene) {
            importScoreScene(window, app);
        }
    }

    std::cout << "saving " << app.save() << '\n';
}