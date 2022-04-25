#include <iomanip>
#include <ctime>

#include "Scene5.h"
#include "Constants.h"
#include "Course.h"

using std::string, std::to_string, std::tm;

static App* app;
static sf::RenderWindow* window;
static Input_Textbox *regStartInput, *regEndInput, *idInput, *nameInput, *teacherInput, *ss1StInput, *ss1EnInput, *ss2StInput, *ss2EnInput, *creditsInput, *maxStudentsInput;
static int num_page, page_cnt;

static string double_to_string(const double& val, int d = 2) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(d) << val;
    return stream.str();
}

static void go_back(int dummy) {
    app->scenes.pop();
}

static void go_to_scene7(int dummy) {
    app->scenes.arg = app->semester()->courses[dummy];

    app->scenes.push(SceneType::Scene7);

}

static void export_students(int dummy) {
    auto iter = app->semester()->courses[dummy];
    auto course = iter.ptr<Course>();

    app->exportStudentsInCourse(iter, PATH::EXPORTS + course->id + "_students.csv");
}

static void delete_course(int dummy) {
    auto iter = app->semester()->courses[dummy];
    app->deleteCourse(iter);

    app->scenes.refresh = true;
}

static void go_to_modify_reg_scene(int dummy) {
    app->scenes.push(SceneType::SceneModifyReg);
}

static void modify_reg(int dummy) {
    try {
        tm start = Utils::strToDate(regStartInput->text);
        tm end = Utils::strToDate(regEndInput->text);

        time_t mkstart = mktime(&start), mkend = mktime(&end);

        if (mkstart != -1 && mkend != -1 && mkstart < mkend) {
            auto &reg = app->semester()->reg_session;
            reg.start = start;
            reg.end = end;

            go_back(0);
        }
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

static void go_to_add_course_scene(int dummy) {
    app->scenes.push(SceneType::SceneAddCourse);
}

static void add_course(int dummy) {
    try {
        tm ss1_start = Utils::strToSession(ss1StInput->text);
        tm ss1_end = Utils::strToSession(ss1EnInput->text);
        tm ss2_start = Utils::strToSession(ss2StInput->text);
        tm ss2_end = Utils::strToSession(ss2EnInput->text);
        int credits = std::stoi(creditsInput->text);
        int max_students = std::stoi(maxStudentsInput->text);

        time_t mkstart = mktime(&ss1_start), mkend = mktime(&ss1_start);
        if (mkstart == -1 && mkend == -1 && mkstart >= mkend)
            return;

        mkstart = mktime(&ss2_start), mkend = mktime(&ss2_start);
        if (mkstart == -1 && mkend == -1 && mkstart >= mkend)
            return;

        auto course = make_shared<Course>(idInput->text,
                                          nameInput->text,
                                          FullName(teacherInput->text),
                                          Course::Session(ss1_start, ss1_end),
                                          Course::Session(ss2_start, ss2_end),
                                          credits,
                                          max_students);
        app->addCourse(course);

        go_back(0);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

static void delete_semester(int dummy) {
    app->deleteDefaultSemester();

    app->scenes.push(SceneType::Scene2);
}

static void previous(int dummy) {
    num_page = std::max(num_page - 1, 0);
}

static void next(int dummy) {
    num_page = std::min(num_page + 1, page_cnt - 1);
}

void add_course_scene(sf::RenderWindow& _window, App &_app) {
    app = &_app;
    window = &_window;
    SceneType current_scene = SceneType::SceneAddCourse;

    auto year = app->year();
    auto semester = app->semester();

    float diff_y = 60, current_y = -20, padding_left = GUI::windowWidth / 4, padding_top = GUI::windowHeight / 7, char_size = GUI::defaultCharSize / 1.3;
    sf::Texture texture;
    List<Textbox> boxes;
    Interaction interaction;


    Textbox title_box("Add a course (session syntax: Mon 9:30):",
                      24,
                      sf::Color::Black,
                      sf::Vector2f(padding_left, padding_top - 50),
                      sf::Vector2f(100, 30),
                      sf::Color::White);
    title_box.align_left();
    boxes.push_back(title_box);


    Textbox mainBackground("",
                           char_size,
                           sf::Color::White,
                           sf::Vector2f(padding_left, padding_top),
                           sf::Vector2f(GUI::windowWidth / 2, 700),
                           sf::Color::Black);
    mainBackground.set_outline(sf::Color::Blue, 4);
    boxes.push_back(mainBackground);


    Textbox idText("Course ID",
                      char_size,
                      sf::Color::Black,
                      sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                      sf::Vector2f(130, 50),
                      sf::Color::Cyan);
    boxes.push_back(idText);
    Textbox idBox("",
                     char_size,
                     sf::Color::Black,
                     sf::Vector2f(padding_left + 170, idText.box.getPosition().y),
                     sf::Vector2f(410, 50),
                     sf::Color::White);
    Input_Textbox idInputBox(idBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(idInputBox);
    idInput = &idInputBox;


    Textbox nameText("Course name",
                    char_size,
                    sf::Color::Black,
                    sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                    sf::Vector2f(130, 50),
                    sf::Color::Cyan);
    boxes.push_back(nameText);
    Textbox nameBox("",
                   char_size,
                   sf::Color::Black,
                   sf::Vector2f(padding_left + 170, nameText.box.getPosition().y),
                   sf::Vector2f(410, 50),
                   sf::Color::White);
    Input_Textbox nameInputBox(nameBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(nameInputBox);
    nameInput = &nameInputBox;


    Textbox teacherText("Teacher",
                     char_size,
                     sf::Color::Black,
                     sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                     sf::Vector2f(130, 50),
                     sf::Color::Cyan);
    boxes.push_back(teacherText);
    Textbox teacherBox("",
                    char_size,
                    sf::Color::Black,
                    sf::Vector2f(padding_left + 170, teacherText.box.getPosition().y),
                    sf::Vector2f(410, 50),
                    sf::Color::White);
    Input_Textbox teacherInputBox(teacherBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(teacherInputBox);
    teacherInput = &teacherInputBox;


    Textbox ss1StText("Session 1 starts",
                     char_size,
                     sf::Color::Black,
                     sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                     sf::Vector2f(130, 50),
                     sf::Color::Cyan);
    boxes.push_back(ss1StText);
    Textbox ss1StBox("",
                    char_size,
                    sf::Color::Black,
                    sf::Vector2f(padding_left + 170, ss1StText.box.getPosition().y),
                    sf::Vector2f(410, 50),
                    sf::Color::White);
    Input_Textbox ss1StInputBox(ss1StBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(ss1StInputBox);
    ss1StInput = &ss1StInputBox;


    Textbox ss1EnText("Session 1 ends",
                      char_size,
                      sf::Color::Black,
                      sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                      sf::Vector2f(130, 50),
                      sf::Color::Cyan);
    boxes.push_back(ss1EnText);
    Textbox ss1EnBox("",
                     char_size,
                     sf::Color::Black,
                     sf::Vector2f(padding_left + 170, ss1EnText.box.getPosition().y),
                     sf::Vector2f(410, 50),
                     sf::Color::White);
    Input_Textbox ss1EnInputBox(ss1EnBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(ss1EnInputBox);
    ss1EnInput = &ss1EnInputBox;


    Textbox ss2StText("Session 2 starts",
                      char_size,
                      sf::Color::Black,
                      sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                      sf::Vector2f(130, 50),
                      sf::Color::Cyan);
    boxes.push_back(ss2StText);
    Textbox ss2StBox("",
                     char_size,
                     sf::Color::Black,
                     sf::Vector2f(padding_left + 170, ss2StText.box.getPosition().y),
                     sf::Vector2f(410, 50),
                     sf::Color::White);
    Input_Textbox ss2StInputBox(ss2StBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(ss2StInputBox);
    ss2StInput = &ss2StInputBox;


    Textbox ss2EnText("Session 2 ends",
                      char_size,
                      sf::Color::Black,
                      sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                      sf::Vector2f(130, 50),
                      sf::Color::Cyan);
    boxes.push_back(ss2EnText);
    Textbox ss2EnBox("",
                     char_size,
                     sf::Color::Black,
                     sf::Vector2f(padding_left + 170, ss2EnText.box.getPosition().y),
                     sf::Vector2f(410, 50),
                     sf::Color::White);
    Input_Textbox ss2EnInputBox(ss2EnBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(ss2EnInputBox);
    ss2EnInput = &ss2EnInputBox;


    Textbox creditText("Credits",
                      char_size,
                      sf::Color::Black,
                      sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                      sf::Vector2f(130, 50),
                      sf::Color::Cyan);
    boxes.push_back(creditText);
    Textbox creditBox("",
                     char_size,
                     sf::Color::Black,
                     sf::Vector2f(padding_left + 170, creditText.box.getPosition().y),
                     sf::Vector2f(410, 50),
                     sf::Color::White);
    Input_Textbox creditInputBox(creditBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(creditInputBox);
    creditsInput = &creditInputBox;


    Textbox maxStudentsText("Max students",
                       char_size,
                       sf::Color::Black,
                       sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                       sf::Vector2f(130, 50),
                       sf::Color::Cyan);
    boxes.push_back(maxStudentsText);
    Textbox maxStudentsBox("",
                      char_size,
                      sf::Color::Black,
                      sf::Vector2f(padding_left + 170, maxStudentsText.box.getPosition().y),
                      sf::Vector2f(410, 50),
                      sf::Color::White);
    Input_Textbox maxStudentsInputBox(maxStudentsBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(maxStudentsInputBox);
    maxStudentsInput = &maxStudentsInputBox;


    Textbox setBox("SET",
                   GUI::defaultCharSize,
                   sf::Color::Black,
                   sf::Vector2f(padding_left * 2 - 65, padding_top + (current_y += (diff_y * 1.5))),
                   sf::Vector2f(130, 50),
                   sf::Color::Cyan);
    Button_Textbox setButton(setBox, sf::Color::White);
    interaction.add_button(setButton, add_course);


    texture.loadFromFile(PATH::IMAGES + "go_back.png");
    Button_Sprite back_button(texture,
                              sf::Vector2f(10, 5),
                              sf::Vector2f(40, 40));
    interaction.add_button(back_button, go_back);


    while (window->isOpen() && !app->scenes.empty() && app->scenes.top() == current_scene && !app->scenes.refresh) {
        window->clear(sf::Color::White);

        for(auto &box : boxes)
            box.draw(*window);
        interaction.draw(*window);
        window->display();

        auto event = interaction.interact(*window);
        app->scenes.interact(event);
    }
}

void modify_registration_scene(sf::RenderWindow& _window, App &_app) {
    app = &_app;
    window = &_window;
    SceneType current_scene = SceneType::SceneModifyReg;

    auto year = app->year();
    auto semester = app->semester();

    float current_y = 50, diff_y = 10, padding_left = 30;
    sf::Texture texture;
    List<Textbox> boxes;
    Interaction interaction;


    Textbox title_box("Modifying registration session (syntax: dd/mm/yyyy):",
                      24,
                      sf::Color::Black,
                      sf::Vector2f(GUI::windowWidth / 4, GUI::windowHeight / 3 - 50),
                      sf::Vector2f(100, 30),
                      sf::Color::White);
    title_box.align_left();
    boxes.push_back(title_box);


    Textbox mainBackground("",
                           GUI::defaultCharSize,
                           sf::Color::White,
                           sf::Vector2f(GUI::windowWidth / 4, GUI::windowHeight / 3),
                           sf::Vector2f(GUI::windowWidth / 2, GUI::windowHeight / 3),
                           sf::Color::Black);
    mainBackground.set_outline(sf::Color::Blue, 4);
    boxes.push_back(mainBackground);


    Textbox startText("Start",
                         GUI::defaultCharSize,
                         sf::Color::Black,
                         sf::Vector2f(GUI::windowWidth / 4 + 20, GUI::windowHeight / 3 + 30),
                         sf::Vector2f(130, 50),
                         sf::Color::Cyan);
    boxes.push_back(startText);
    Textbox startBox("",
                        GUI::defaultCharSize,
                        sf::Color::Black,
                        sf::Vector2f(GUI::windowWidth / 4 + 170, GUI::windowHeight / 3 + 30),
                        sf::Vector2f(410, 50),
                        sf::Color::White);
    Input_Textbox startInputBox(startBox, 20, sf::Color::Blue);
    regStartInput = &startInputBox;


    Textbox endText("End",
                    GUI::defaultCharSize,
                    sf::Color::Black,
                    sf::Vector2f(GUI::windowWidth / 4 + 20, GUI::windowHeight / 3 + 100),
                    sf::Vector2f(130, 50),
                    sf::Color::Cyan);
    boxes.push_back(endText);
    Textbox endBox("",
                   GUI::defaultCharSize,
                   sf::Color::Black,
                   sf::Vector2f(GUI::windowWidth / 4 + 170, GUI::windowHeight / 3 + 100),
                   sf::Vector2f(410, 50),
                   sf::Color::White);
    Input_Textbox endInputBox(endBox, 20, sf::Color::Blue);
    regEndInput = &endInputBox;


    Textbox setBox("SET",
                     GUI::defaultCharSize,
                     sf::Color::Black,
                     sf::Vector2f(GUI::windowWidth / 2 - 65, GUI::windowHeight / 3 + 220),
                     sf::Vector2f(130, 50),
                     sf::Color::Cyan);
    Button_Textbox setButton(setBox, sf::Color::White);


    interaction.add_input_textbox(startInputBox);
    interaction.add_input_textbox(endInputBox);
    interaction.add_button(setButton, modify_reg);


    texture.loadFromFile(PATH::IMAGES + "go_back.png");
    Button_Sprite back_button(texture,
                              sf::Vector2f(10, 5),
                              sf::Vector2f(40, 40));
    interaction.add_button(back_button, go_back);


    while (window->isOpen() && !app->scenes.empty() && app->scenes.top() == current_scene && !app->scenes.refresh) {
        window->clear(sf::Color::White);

        for(auto &box : boxes)
            box.draw(*window);
        interaction.draw(*window);
        window->display();

        auto event = interaction.interact(*window);
        app->scenes.interact(event);
    }
}

void scene5(sf::RenderWindow& _window, App &_app) {
    app = &_app;
    window = &_window;
    SceneType current_scene = SceneType::Scene5;

    auto year = app->year();
    auto semester = app->semester();

    float current_y = 50, diff_y = 10, padding_left = 30;
    sf::Texture del_texture, back_texture, modify_texture;
    List<Textbox> boxes;
    Button_List<Button_Textbox> courses, exports, dels;
    Interaction interaction;

    modify_texture.loadFromFile(PATH::IMAGES + "modify.png");
    del_texture.loadFromFile(PATH::IMAGES + "delete.png");
    back_texture.loadFromFile(PATH::IMAGES + "go_back.png");


    Textbox year_box("School year " + to_string(year->start_year) + " " + to_string(year->end_year),
                     18,
                     sf::Color::Black,
                     sf::Vector2f(padding_left, (current_y += diff_y)),
                     sf::Vector2f(100, 30),
                     sf::Color::White);
    current_y += 30;
    year_box.align_left();
    boxes.push_back(year_box);



    Textbox semester_box("Semester " + to_string(semester->no) + ": " + Utils::dateToStr(semester->start) + " - " +
                         Utils::dateToStr(semester->end),
                         18,
                         sf::Color::Black,
                         sf::Vector2f(padding_left, (current_y += diff_y)),
                         sf::Vector2f(100, 30),
                         sf::Color::White);
    current_y += 30;
    semester_box.align_left();
    boxes.push_back(semester_box);

    Textbox semester_del_box("Delete semester",
                       18,
                       sf::Color::Black,
                       sf::Vector2f(padding_left, (current_y += diff_y)),
                       sf::Vector2f(150, 30),
                       sf::Color::Cyan);
    current_y += 30;
    semester_del_box.align_left();
    Button_Textbox semester_del_button(semester_del_box, sf::Color::Yellow);
    semester_del_button.set_idle_outline(sf::Color::Blue);


    string s;
    if (!semester->reg_session.hasInit())
        s = "Registration session in current semester is not set up.";
    else if (semester->reg_session.hasNotStarted())
        s = "Registration has not started: " + Utils::tmToStr(semester->reg_session.start) + " - " +
            Utils::tmToStr(semester->reg_session.end);
    else if (semester->reg_session.isOngoing())
        s = "Registration is ongoing: " + Utils::tmToStr(semester->reg_session.start) + " - " +
            Utils::tmToStr(semester->reg_session.end);
    else if (semester->reg_session.hasEnded())
        s = "Registration has ended: " + Utils::tmToStr(semester->reg_session.start) + " - " +
            Utils::tmToStr(semester->reg_session.end);
    Textbox reg_box(s,
                    18,
                    sf::Color::Black,
                    sf::Vector2f(padding_left, (current_y += diff_y)),
                    sf::Vector2f(100, 30),
                    sf::Color::White);
    current_y += 30;
    reg_box.align_left();
    boxes.push_back(reg_box);

    Button_Sprite reg_modify(modify_texture,
                             sf::Vector2f(padding_left, (current_y += 5)),
                             sf::Vector2f(30, 30));
    current_y += 30;



    Textbox course_title_box("Courses:",
                             24,
                             sf::Color::Black,
                             sf::Vector2f(padding_left, (current_y += diff_y)),
                             sf::Vector2f(100, 30),
                             sf::Color::White);
    current_y += 50;
    course_title_box.align_left();
    boxes.push_back(course_title_box);


    for (const auto &iter: semester->courses) {
        auto course = iter.ptr<Course>();
        Textbox course_box(
                course->id + " - " + course->name + ", teacher: " + course->teacher_name.toStr(),
                18,
                sf::Color::Black,
                sf::Vector2f(padding_left, (current_y += diff_y)),
                sf::Vector2f(500, 50),
                sf::Color::Cyan);
        current_y += 50;
        Button_Textbox course_button(course_box, sf::Color::Yellow);
        course_button.set_idle_outline(sf::Color::Blue);
        courses.add_button(course_button);

        Textbox export_box("Export students",
                           18,
                           sf::Color::Black,
                           sf::Vector2f(padding_left, (current_y += diff_y)),
                           sf::Vector2f(150, 30),
                           sf::Color::Cyan);
        current_y += 30;
        Button_Textbox export_button(export_box, sf::Color::Yellow);
        export_button.set_idle_outline(sf::Color::Blue);
        exports.add_button(export_button);

        Textbox del_box("Delete",
                           18,
                           sf::Color::Black,
                        sf::Vector2f(export_box.box.getPosition().x + 200, export_box.box.getPosition().y),
                           sf::Vector2f(100, 30),
                           sf::Color::Cyan);
        current_y += 30;
        Button_Textbox del_button(del_box, sf::Color::Yellow);
        del_button.set_idle_outline(sf::Color::Blue);
        dels.add_button(del_button);
    }

    
    Textbox add_course_box("Add a course",
                       18,
                       sf::Color::Black,
                       sf::Vector2f(padding_left, (current_y += diff_y)),
                       sf::Vector2f(150, 30),
                       sf::Color::Cyan);
    current_y += 30;
    Button_Textbox add_course_button(add_course_box, sf::Color::Yellow);
    add_course_button.set_idle_outline(sf::Color::Blue);


    Button_Sprite back_button(back_texture,
                              sf::Vector2f(10, 5),
                              sf::Vector2f(40, 40));


    courses.add_trigger(go_to_scene7);
    interaction.add_button_list(courses);
    exports.add_trigger(export_students);
    interaction.add_button_list(exports);
    dels.add_trigger(delete_course);
    interaction.add_button_list(dels);
    interaction.add_button(add_course_button, go_to_add_course_scene);
    interaction.add_button(semester_del_button, delete_semester);
    interaction.add_button(reg_modify, go_to_modify_reg_scene);
    interaction.add_button(back_button, go_back);


    while (window->isOpen() && !app->scenes.empty() && app->scenes.top() == current_scene && !app->scenes.refresh) {
        window->clear(sf::Color::White);

        for(auto &box : boxes)
            box.draw(*window);
        interaction.draw(*window);
        window->display();

        auto event = interaction.interact(*window);
        app->scenes.interact(event);
    }
}