#include <iomanip>
#include <ctime>

#include "Scene5.h"
#include "Constants.h"
#include "Course.h"

using std::string, std::to_string, std::tm;

static App* app;
static sf::RenderWindow* window;
static Input_Textbox *regStartInput, *regEndInput, *idInput, *nameInput, *teacherInput, *ss1Input, *ss2Input, *creditsInput, *maxStudentsInput;

static int page_size;
static int num_page = 0, page_cnt;

static string double_to_string(const double& val, int d = 2) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(d) << val;
    return stream.str();
}

static void go_back(int dummy) {
    app->scenes.pop();
}

static void go_to_scene7(int dummy) {
    dummy += num_page * page_size;
    app->scenes.arg = app->semester()->courses[dummy];
    std::cout << "move to scene 7: " << dummy << '\n';

    app->scenes.push(SceneType::Scene7);
}

static void export_students(int dummy) {
    dummy += num_page * page_size;
    auto iter = app->semester()->courses[dummy];
    auto course = iter.ptr<Course>();

    app->exportStudentsInCourse(iter, PATH::EXPORTS + course->id + "_students.csv");
}

static void delete_course(int dummy) {
    dummy += num_page * page_size;
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
        int credits = std::stoi(creditsInput->text);
        int max_students = std::stoi(maxStudentsInput->text);

        auto course = make_shared<Course>(idInput->text,
                                          nameInput->text,
                                          FullName(teacherInput->text),
                                          Course::Session(ss1Input->text),
                                          Course::Session(ss2Input->text),
                                          credits,
                                          max_students);

        if (course->sessions.all_of([](const Course::Session &s) {
            return s.valid();
        }) &&
        credits >= 0 &&
        max_students >= 0) {
            app->addCourse(course);

            go_back(0);
        }
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

static void delete_semester(int dummy) {
    app->deleteDefaultSemester();

    app->scenes.push(SceneType::Scene2);
}

void add_course_scene(sf::RenderWindow& _window, App &_app) {
    app = &_app;
    window = &_window;
    SceneType current_scene = SceneType::SceneAddCourse;

    auto year = app->year();
    auto semester = app->semester();

    float diff_y = 60, current_y = -20, padding_left = GUI::windowWidth / 4, padding_top = GUI::windowHeight / 6;
    sf::Texture texture;
    List<Textbox> boxes;
    Interaction interaction;


    Textbox title_box("Add a course (session syntax: Mon S1/S2/S3/S4):",
                      24,
                      sf::Color::Black,
                      sf::Vector2f(padding_left, padding_top - 50),
                      sf::Vector2f(100, 30),
                      sf::Color::White);
    title_box.align_left();
    boxes.push_back(title_box);


    Textbox mainBackground("",
                           GUI::defaultSmallCharSize,
                           sf::Color::White,
                           sf::Vector2f(padding_left, padding_top),
                           sf::Vector2f(GUI::windowWidth / 2, 550),
                           sf::Color::Black);
    mainBackground.set_outline(sf::Color::Blue, 4);
    boxes.push_back(mainBackground);


    Textbox idText("Course ID",
                      GUI::defaultSmallCharSize,
                      sf::Color::Black,
                      sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                      sf::Vector2f(130, 50),
                      sf::Color::Cyan);
    boxes.push_back(idText);
    Textbox idBox("",
                     GUI::defaultSmallCharSize,
                     sf::Color::Black,
                     sf::Vector2f(padding_left + 170, idText.box.getPosition().y),
                     sf::Vector2f(410, 50),
                     sf::Color::White);
    Input_Textbox idInputBox(idBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(idInputBox);
    idInput = &idInputBox;


    Textbox nameText("Course name",
                    GUI::defaultSmallCharSize,
                    sf::Color::Black,
                    sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                    sf::Vector2f(130, 50),
                    sf::Color::Cyan);
    boxes.push_back(nameText);
    Textbox nameBox("",
                   GUI::defaultSmallCharSize,
                   sf::Color::Black,
                   sf::Vector2f(padding_left + 170, nameText.box.getPosition().y),
                   sf::Vector2f(410, 50),
                   sf::Color::White);
    Input_Textbox nameInputBox(nameBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(nameInputBox);
    nameInput = &nameInputBox;


    Textbox teacherText("Teacher",
                     GUI::defaultSmallCharSize,
                     sf::Color::Black,
                     sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                     sf::Vector2f(130, 50),
                     sf::Color::Cyan);
    boxes.push_back(teacherText);
    Textbox teacherBox("",
                    GUI::defaultSmallCharSize,
                    sf::Color::Black,
                    sf::Vector2f(padding_left + 170, teacherText.box.getPosition().y),
                    sf::Vector2f(410, 50),
                    sf::Color::White);
    Input_Textbox teacherInputBox(teacherBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(teacherInputBox);
    teacherInput = &teacherInputBox;


    Textbox ss1Text("Session 1",
                     GUI::defaultSmallCharSize,
                     sf::Color::Black,
                     sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                     sf::Vector2f(130, 50),
                     sf::Color::Cyan);
    boxes.push_back(ss1Text);
    Textbox ss1Box("",
                    GUI::defaultSmallCharSize,
                    sf::Color::Black,
                    sf::Vector2f(padding_left + 170, ss1Text.box.getPosition().y),
                    sf::Vector2f(410, 50),
                    sf::Color::White);
    Input_Textbox ss1InputBox(ss1Box, 20, sf::Color::Blue);
    interaction.add_input_textbox(ss1InputBox);
    ss1Input = &ss1InputBox;


    Textbox ss2Text("Session 2",
                      GUI::defaultSmallCharSize,
                      sf::Color::Black,
                      sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                      sf::Vector2f(130, 50),
                      sf::Color::Cyan);
    boxes.push_back(ss2Text);
    Textbox ss2Box("",
                     GUI::defaultSmallCharSize,
                     sf::Color::Black,
                     sf::Vector2f(padding_left + 170, ss2Text.box.getPosition().y),
                     sf::Vector2f(410, 50),
                     sf::Color::White);
    Input_Textbox ss2InputBox(ss2Box, 20, sf::Color::Blue);
    interaction.add_input_textbox(ss2InputBox);
    ss2Input = &ss2InputBox;


    Textbox creditText("Credits",
                      GUI::defaultSmallCharSize,
                      sf::Color::Black,
                      sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                      sf::Vector2f(130, 50),
                      sf::Color::Cyan);
    boxes.push_back(creditText);
    Textbox creditBox("",
                     GUI::defaultSmallCharSize,
                     sf::Color::Black,
                     sf::Vector2f(padding_left + 170, creditText.box.getPosition().y),
                     sf::Vector2f(410, 50),
                     sf::Color::White);
    Input_Textbox creditInputBox(creditBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(creditInputBox);
    creditsInput = &creditInputBox;


    Textbox maxStudentsText("Max students",
                       GUI::defaultSmallCharSize,
                       sf::Color::Black,
                       sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                       sf::Vector2f(130, 50),
                       sf::Color::Cyan);
    boxes.push_back(maxStudentsText);
    Textbox maxStudentsBox("",
                      GUI::defaultSmallCharSize,
                      sf::Color::Black,
                      sf::Vector2f(padding_left + 170, maxStudentsText.box.getPosition().y),
                      sf::Vector2f(410, 50),
                      sf::Color::White);
    Input_Textbox maxStudentsInputBox(maxStudentsBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(maxStudentsInputBox);
    maxStudentsInput = &maxStudentsInputBox;


    Textbox setBox("SET",
                   GUI::defaultMediumCharSize,
                   sf::Color::Black,
                   sf::Vector2f(padding_left * 2 - 65, padding_top + (current_y += (diff_y * 1.3))),
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
            box.draw(*window, app->default_font);
        interaction.draw(*window, app->default_font);
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
                           GUI::defaultMediumCharSize,
                           sf::Color::White,
                           sf::Vector2f(GUI::windowWidth / 4, GUI::windowHeight / 3),
                           sf::Vector2f(GUI::windowWidth / 2, GUI::windowHeight / 3),
                           sf::Color::Black);
    mainBackground.set_outline(sf::Color::Blue, 4);
    boxes.push_back(mainBackground);


    Textbox startText("Start",
                         GUI::defaultMediumCharSize,
                         sf::Color::Black,
                         sf::Vector2f(GUI::windowWidth / 4 + 20, GUI::windowHeight / 3 + 30),
                         sf::Vector2f(130, 50),
                         sf::Color::Cyan);
    boxes.push_back(startText);
    Textbox startBox("",
                        GUI::defaultMediumCharSize,
                        sf::Color::Black,
                        sf::Vector2f(GUI::windowWidth / 4 + 170, GUI::windowHeight / 3 + 30),
                        sf::Vector2f(410, 50),
                        sf::Color::White);
    Input_Textbox startInputBox(startBox, 20, sf::Color::Blue);
    regStartInput = &startInputBox;


    Textbox endText("End",
                    GUI::defaultMediumCharSize,
                    sf::Color::Black,
                    sf::Vector2f(GUI::windowWidth / 4 + 20, GUI::windowHeight / 3 + 100),
                    sf::Vector2f(130, 50),
                    sf::Color::Cyan);
    boxes.push_back(endText);
    Textbox endBox("",
                   GUI::defaultMediumCharSize,
                   sf::Color::Black,
                   sf::Vector2f(GUI::windowWidth / 4 + 170, GUI::windowHeight / 3 + 100),
                   sf::Vector2f(410, 50),
                   sf::Color::White);
    Input_Textbox endInputBox(endBox, 20, sf::Color::Blue);
    regEndInput = &endInputBox;


    Textbox setBox("SET",
                     GUI::defaultMediumCharSize,
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
            box.draw(*window, app->default_font);
        interaction.draw(*window, app->default_font);
        window->display();

        auto event = interaction.interact(*window);
        app->scenes.interact(event);
    }
}

static void previous(int dummy) {
    num_page = std::max(num_page - 1, 0);
    app->scenes.refresh = true;
}

static void next(int dummy) {
    num_page = std::min(num_page + 1, page_cnt - 1);
    app->scenes.refresh = true;
}

void scene5(sf::RenderWindow& _window, App &_app) {
    app = &_app;
    window = &_window;
    auto current_scene = SceneType::Scene5;

    auto year = app->year();
    auto semester = app->semester();

    semester->sortCourse();
    auto courses = semester->courses;

    float current_y = 80;
    List<Textbox> texts;


    Textbox semester_box("Semester " + to_string(semester->no) + " (" + to_string(year->start_year) + " - " + to_string(year->end_year) + ")",
                    GUI::defaultSmallCharSize,
                    sf::Color::Black,
                    sf::Vector2f(70, current_y),
                    sf::Vector2f(100, 30),
                    sf::Color::White);
    semester_box.align_left();
    texts.push_back(semester_box);

    Textbox semester_del_box("Delete",
                           GUI::defaultSmallCharSize,
                           sf::Color::Black,
                           sf::Vector2f(0, current_y),
                           sf::Vector2f(70, 30),
                           sf::Color::White);
    semester_del_box.align_center();
    semester_del_box.set_box_color(sf::Color::Red);
    Button_Textbox semester_del_button(semester_del_box, sf::Color::Yellow);
    semester_del_button.set_idle_outline(sf::Color::Black);

    current_y += 60;


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
                    GUI::defaultSmallCharSize,
                    sf::Color::Black,
                    sf::Vector2f(70, current_y),
                    sf::Vector2f(100, 30),
                    sf::Color::White);
    reg_box.align_left();
    texts.push_back(reg_box);

    Textbox reg_modify_box("Modify",
                    GUI::defaultSmallCharSize,
                    sf::Color::Black,
                    sf::Vector2f(0, current_y),
                    sf::Vector2f(70, 30),
                    sf::Color::White);
    reg_modify_box.align_center();
    reg_modify_box.set_box_color(sf::Color::Green);
    Button_Textbox reg_button(reg_modify_box, sf::Color::Yellow);
    reg_button.set_idle_outline(sf::Color::Black);


    Textbox add_course_box("Add a course",
                           GUI::defaultSmallCharSize,
                           sf::Color::Black,
                           sf::Vector2f(1050, current_y),
                           sf::Vector2f(150, 30),
                           sf::Color::White);
    add_course_box.align_center();
    add_course_box.set_box_color(sf::Color::Green);
    Button_Textbox add_course_button(add_course_box, sf::Color::Yellow);
    add_course_button.set_idle_outline(sf::Color::Black);


    sf::Texture texture;
    texture.loadFromFile("assets/images/go_back.png");
    Button_Sprite back_button(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));

    texture.loadFromFile("assets/images/previous.jpg");
    Button_Sprite previous_button(texture, sf::Vector2f(910, current_y), sf::Vector2f(40, 40));

    texture.loadFromFile("assets/images/next.jpg");
    Button_Sprite next_button(texture, sf::Vector2f(950, current_y), sf::Vector2f(40, 40));


    int n = courses.size();
    page_size = 5;
    int num_row = 0;
    List<DataIter> page;
    List<List<DataIter>> pages;
    courses.for_each([&](const auto& Iter) {
        ++num_row;
        if (num_row > page_size) {
            num_row -= page_size;
            pages.push_back(page);
            page.clear();
        }
        page.push_back(Iter);
    });
    pages.push_back(page);
    page_cnt = (n - 1) / page_size + 1;
    num_page = std::max(0, std::min(page_cnt - 1, num_page));


    std::string page_num_string;
    if (n > 0)
        page_num_string = std::to_string(num_page + 1) + "/" + std::to_string(page_cnt);
    else
        page_num_string = "0/0";
    Textbox page_num(page_num_string, GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(985, current_y - 5), sf::Vector2f(60, 40), sf::Color::White);
    texts.push_back(page_num);

    current_y += 40;


    Textbox course_id_header("Course ID", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(0, current_y), sf::Vector2f(100, 50), sf::Color::White);
    course_id_header.set_outline(sf::Color::Black);

    Textbox course_name_header("Course Name", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(100, current_y), sf::Vector2f(300, 50), sf::Color::White);
    course_name_header.set_outline(sf::Color::Black);

    Textbox credits_header("Credits", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(400, current_y), sf::Vector2f(100, 50), sf::Color::White);
    credits_header.set_outline(sf::Color::Black);

    Textbox sessions_header("Sessions", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(500, current_y), sf::Vector2f(200, 50), sf::Color::White);
    sessions_header.set_outline(sf::Color::Black);

    Textbox teacher_header("Teacher", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(700, current_y), sf::Vector2f(250, 50), sf::Color::White);
    teacher_header.set_outline(sf::Color::Black);

    Textbox slot_header("Slot", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(950, current_y), sf::Vector2f(100, 50), sf::Color::White);
    slot_header.set_outline(sf::Color::Black);

    Textbox action_header("Action", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(1050, current_y), sf::Vector2f(150, 50), sf::Color::White);
    action_header.set_outline(sf::Color::Black);

    current_y += 50;

    texts.push_back(course_id_header);
    texts.push_back(course_name_header);
    texts.push_back(credits_header);
    texts.push_back(sessions_header);
    texts.push_back(teacher_header);
    texts.push_back(slot_header);
    texts.push_back(action_header);


    Interaction interaction;
    interaction.add_button(back_button, go_back);
    interaction.add_button(semester_del_button, delete_semester);
    interaction.add_button(previous_button, previous);
    interaction.add_button(next_button, next);
    interaction.add_button(reg_button, go_to_modify_reg_scene);
    interaction.add_button(add_course_button, go_to_add_course_scene);
    Button_List<Button_Textbox> details_col, export_col, delete_col;


    if (n > 0) {
        pages[num_page].for_each([&](const DataIter& Iter) {
            auto course = Iter.ptr<Course>();

            Textbox course_id(course->id, GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(0, current_y), sf::Vector2f(100, 100), sf::Color::White);
            course_id.set_outline(sf::Color::Black);
            course_id.align_left();
            texts.push_back(course_id);


            Textbox course_name(course->name, GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(100, current_y), sf::Vector2f(300, 100), sf::Color::White);
            course_name.set_outline(sf::Color::Black);
            course_name.align_left();
            texts.push_back(course_name);


            Textbox credits(std::to_string(course->credits),
                            GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(400, current_y), sf::Vector2f(100, 100), sf::Color::White);
            credits.set_outline(sf::Color::Black);
            texts.push_back(credits);


            Textbox session1(Utils::sessionToStr(course->sessions[0].start) + " - " + Utils::sessionToStr(course->sessions[0].end).substr(4, 5),
                             GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(500, current_y), sf::Vector2f(200, 50), sf::Color::White);
            session1.set_outline(sf::Color::Black);
            texts.push_back(session1);


            Textbox session2(Utils::sessionToStr(course->sessions[1].start) + " - " + Utils::sessionToStr(course->sessions[1].end).substr(4, 5),
                             GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(500, current_y + 50), sf::Vector2f(200, 50), sf::Color::White);
            session2.set_outline(sf::Color::Black);
            texts.push_back(session2);


            Textbox teacher(course->teacher_name.last + " " + course->teacher_name.first,
                            GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(700, current_y), sf::Vector2f(250, 100), sf::Color::White);
            teacher.set_outline(sf::Color::Black);
            teacher.align_left();
            texts.push_back(teacher);


            Textbox slot(std::to_string(course->students.size()) + "/" + std::to_string(course->max_students),
                         GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(950, current_y), sf::Vector2f(100, 100), sf::Color::White);
            slot.set_box_color(sf::Color::Green);
            slot.set_outline(sf::Color::Black);
            //textboxes.push_back(slot);
            Button_Textbox slot_button(slot, sf::Color::Yellow);
            slot_button.set_idle_outline(sf::Color::Black);
            details_col.add_button(slot_button);


            Textbox export_text("Export", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(1050, current_y), sf::Vector2f(80, 100), sf::Color::White);
            export_text.set_box_color(sf::Color::Green);

            export_text.align_center();
            Button_Textbox export_button(export_text, sf::Color::Yellow);
            export_button.set_idle_outline(sf::Color::Black);
            export_col.add_button(export_button);


            Textbox delete_text("Delete", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(1050 + 80, current_y), sf::Vector2f(70, 100), sf::Color::White);
            delete_text.set_box_color(sf::Color::Red);

            delete_text.align_center();
            Button_Textbox delete_button(delete_text, sf::Color::Yellow);
            delete_button.set_idle_outline(sf::Color::Black);
            delete_col.add_button(delete_button);

            current_y += 100;
            current_y += 25;
        });
    }

    export_col.trigger = export_students;
    interaction.add_button_list(export_col);

    delete_col.trigger = delete_course;
    interaction.add_button_list(delete_col);

    details_col.trigger = go_to_scene7;
    interaction.add_button_list(details_col);


    while (window->isOpen() && !app->scenes.empty() && app->scenes.top() == current_scene && !app->scenes.refresh) {
        window->clear(sf::Color::White);

        texts.for_each([&](Textbox& textbox) {
            textbox.draw(*window, app->default_font);
        });

        interaction.draw(*window, app->default_font);
        window->display();

        auto event = interaction.interact(*window);
        app->scenes.interact(event);
    }
}