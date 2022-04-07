#include "Scenes.h"
#include "Utils.h"
#include "SchoolYear.h"

using std::cout, std::cin, std::shared_ptr;

void Scenes::run() {
    while (!stop_token) {
        std::cout << "\n--------------------\nIn any argument, enter -1 to cancel current action.\n";
        (this->*current_scene)();
    }
}

void Scenes::menu() {
    if (app->year()) {
        if (app->semester()) {
            current_scene = &Scenes::menuWithDefaultSemester;
        }
        else {
            current_scene = &Scenes::menuNoDefaultSemester;
        }
    }
    else {
        current_scene = &Scenes::menuNoDefaultYear;
    }
}

void Scenes::menuNoDefaultYear() {
    auto years = app->getAllYears();

    cout << "List of all schoolyears (" << years.size() << "): \n";
    for(const auto& year : years) {
        cout << "  " << year.ptr<SchoolYear>()->start_year << ' ' << year.ptr<SchoolYear>()->end_year << '\n';
    }

    cout << "Press:\n1 - Enter an existing schoolyear.\n2 - Create and enter a new schoolyear.\n";
    int opt; cin >> opt;

    if (opt == 1) {
        int start_year, end_year;
        cout << "Enter start year and end year of desired schoolyear: ";
        while (cin >> start_year >> end_year &&
               start_year != -1 &&
               end_year != -1 &&
               !app->setDefaultSchoolYear(start_year, end_year)) {
            cout << "Please try again: ";
        }

        current_scene = &Scenes::menu;
    }
    else if (opt == 2) {
        int start_year, end_year;
        cout << "Enter a new schoolyear (i.e., 2021 2022): ";
        while (cin >> start_year >> end_year &&
               start_year != -1 &&
               end_year != -1 &&
               !app->addDefaultSchoolYear(make_shared<SchoolYear>(start_year, end_year))) {
            cout << "Please try again: ";
        }

        current_scene = &Scenes::menu;
    }
}


void Scenes::menuNoDefaultSemester() {
    std::cout << "Default schoolyear: " << app->year()->start_year << ' ' << app->year()->end_year << '\n';

    cout << "List of all semesters (" << app->year()->semesters.size() << "): \n";
    for(const auto& s : app->year()->semesters) {
        auto semester = s.ptr<Semester>();
        std::cout << "  semester " << semester->no << ": " << semester->courses.size() << " course(s).\n";
    }

    cout << "Press:\n1 - Enter an existing semester.\n2 - Create and enter a new semester.\n3 - See a list of all classes.\n4 - Exit current schoolyear.\n5 - Delete current schoolyear.\n";
    int opt; cin >> opt;

    if (opt == 1) {
        cout << "Enter no. of the semester: ";
        int no;
        while (cin >> no && no != -1 && !app->setDefaultSemester(no)) {
            cout << "Please try again: ";
        }

        current_scene = &Scenes::menu;
    }
    else if (opt == 2) {
        cout << "Enter no. of the new semester: ";
        int no;
        while (cin >> no && no != -1 && !app->addDefaultSemester(make_shared<Semester>(no, tm{}, tm{}))) {
            cout << "Please try again: ";
        }

        current_scene = &Scenes::menu;
    }
    else if (opt == 3) {
        // TODO: Move to scene 3 (class-related).
    }
    else if (opt == 4) {
        app->exitDefaultSchoolYear();
        current_scene = &Scenes::menu;
    }
    else if (opt == 5) {
        app->deleteDefaultSchoolYear();
        current_scene = &Scenes::menu;
    }
}

void Scenes::menuWithDefaultSemester() {
    std::cout << "Default schoolyear: " << app->year()->start_year << ' ' << app->year()->end_year << '\n';
    std::cout << "Default semester: " << app->semester()->no << '\n';

    cout << "Press:\n1 - See details of current semester.\n2 - See a list of all classes.\n3 - Exit current semester.\n4 - Delete current semester.\n";
    int opt; cin >> opt;

    if (opt == 1) {
        // TODO: Move to scene 5 (semester-related).
    }
    else if (opt == 2) {
        // TODO: Move to scene 3 (class-related).
    }
    else if (opt == 3) {
        app->exitDefaultSemester();
        current_scene = &Scenes::menu;
    }
    else if (opt == 4) {
        app->deleteDefaultSemester();
        current_scene = &Scenes::menu;
    }
}
