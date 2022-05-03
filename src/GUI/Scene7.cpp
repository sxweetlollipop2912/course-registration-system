#include <iomanip>
#include <ctime>
#include <stdexcept>

#include "Scene7.h"
#include "Constants.h"
#include "Account.h"

using std::string, std::to_string, std::tm;

static App *app;
static sf::RenderWindow *window;
static Input_Textbox *regStartInput, *regEndInput, *idInput, *nameInput, *teacherInput, *ss1Input, *ss2Input, *creditsInput, *maxStudentsInput;

static int page_size;
static int num_page = 0, page_cnt;

static string double_to_string(const double &val, int d = 2) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(d) << val;
    return stream.str();
}

static void go_back(int dummy) {
    app->scenes.pop();
}

static void export_students(int dummy) {
    auto course = app->scenes.arg.ptr<Course>();
    app->exportStudentsInCourse(app->scenes.arg, PATH::EXPORTS + course->id + "_students.csv");
}

static void delete_course(int dummy) {
    app->deleteCourse(app->scenes.arg);
    app->scenes.arg.clear();

    app->scenes.pop();
}

static void disenroll_student(int dummy) {
    dummy += num_page * page_size;
    auto course = app->scenes.arg.ptr<Course>();

    app->disenroll(course->students[dummy], course->uid);
    app->scenes.refresh = true;
}

static void go_to_scene8(int dummy) {
    app->scenes.push(SceneType::Scene8);
}

static void go_to_modify_course_scene(int dummy) {
    app->scenes.push(SceneType::SceneModifyCourse);
}

static void previous(int dummy) {
    num_page = std::max(num_page - 1, 0);
    app->scenes.refresh = true;
}

static void next(int dummy) {
    num_page = std::min(num_page + 1, page_cnt - 1);
    app->scenes.refresh = true;
}

static void modify_course(int dummy) {
    auto course = app->scenes.arg.ptr<Course>();

    try {
        int credits = std::stoi(creditsInput->text);
        int max_students = std::stoi(maxStudentsInput->text);

        auto new_course = Course(idInput->text,
                                 nameInput->text,
                                 FullName(teacherInput->text),
                                 Course::Session(ss1Input->text),
                                 Course::Session(ss2Input->text),
                                 credits,
                                 max_students);

        if (new_course.sessions.all_of([](const Course::Session &s) {
            return s.valid();
        }) &&
            credits > 0 &&
            course->students.size() <= max_students) {
            course->id = new_course.id;
            course->name = new_course.name;
            course->teacher_name = new_course.teacher_name;
            course->sessions = new_course.sessions;
            course->credits = new_course.credits;
            course->max_students = new_course.max_students;

            go_back(0);
        }
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void modifyCourseScene(sf::RenderWindow &_window, App &_app) {
    app = &_app;
    window = &_window;
    SceneType current_scene = SceneType::SceneModifyCourse;

    auto year = app->year();
    auto semester = app->semester();

    shared_ptr<Course> course;
    try {
        course = app->scenes.arg.ptr<Course>();

        if (!course)
            throw std::out_of_range("scene7: scenes.arg is null.");
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;

        app->scenes.pop();
        return;
    }

    float diff_y = 60, current_y = -20, padding_left = GUI::windowWidth / 4, padding_top = GUI::windowHeight / 6;
    sf::Texture texture;
    List<Textbox> boxes;
    Interaction interaction;


    Textbox title_box("Modify course (session syntax: Mon S1/S2/S3/S4):",
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
    Textbox idBox(course->id,
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
    Textbox nameBox(course->name,
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
    Textbox teacherBox(course->teacher_name.toStr(),
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
    Textbox ss1Box(course->sessions[0].toStr(),
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
    Textbox ss2Box(course->sessions[1].toStr(),
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
    Textbox creditBox(to_string(course->credits),
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
    Textbox maxStudentsBox(to_string(course->max_students),
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
    interaction.add_button(setButton, modify_course);


    texture.loadFromFile(PATH::IMAGES + "go_back.png");
    Button_Sprite back_button(texture,
                              sf::Vector2f(10, 5),
                              sf::Vector2f(40, 40));
    interaction.add_button(back_button, go_back);


    while (window->isOpen() && !app->scenes.empty() && app->scenes.top() == current_scene && !app->scenes.refresh) {
        window->clear(sf::Color::White);

        for (auto &box: boxes)
            box.draw(*window, app->default_font);
        interaction.draw(*window, app->default_font);
        window->display();

        auto event = interaction.interact(*window);
        app->scenes.interact(event);
    }
}

void scene7(sf::RenderWindow &_window, App &_app) {
    app = &_app;
    window = &_window;
    auto current_scene = SceneType::Scene7;

    auto year = app->year();
    auto semester = app->semester();

    shared_ptr<Course> course;
    try {
        course = app->scenes.arg.ptr<Course>();
        if (!course)
            throw std::out_of_range("scene7: scenes.arg is null.");
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;

        app->scenes.pop();
        return;
    }

    course->sortStudentsByID();
    auto students = course->students;

    float current_y = 80, height = 80;
    List<Textbox> texts;


    Textbox course_id_header("Course ID", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(0, current_y),
                             sf::Vector2f(100, 50), sf::Color::White);
    course_id_header.set_outline(sf::Color::Black);

    Textbox course_name_header("Course Name", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(100, current_y),
                               sf::Vector2f(260, 50), sf::Color::White);
    course_name_header.set_outline(sf::Color::Black);

    Textbox credits_header("Credits", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(360, current_y),
                           sf::Vector2f(100, 50), sf::Color::White);
    credits_header.set_outline(sf::Color::Black);

    Textbox sessions_header("Sessions", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(460, current_y),
                            sf::Vector2f(200, 50), sf::Color::White);
    sessions_header.set_outline(sf::Color::Black);

    Textbox teacher_header("Teacher", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(660, current_y),
                           sf::Vector2f(210, 50), sf::Color::White);
    teacher_header.set_outline(sf::Color::Black);

    Textbox slot_header("Slot", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(870, current_y),
                        sf::Vector2f(100, 50), sf::Color::White);
    slot_header.set_outline(sf::Color::Black);

    Textbox course_action_header("Action", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(970, current_y),
                                 sf::Vector2f(230, 50), sf::Color::White);
    course_action_header.set_outline(sf::Color::Black);

    current_y += 50;

    texts.push_back(course_id_header);
    texts.push_back(course_name_header);
    texts.push_back(credits_header);
    texts.push_back(sessions_header);
    texts.push_back(teacher_header);
    texts.push_back(slot_header);
    texts.push_back(course_action_header);


    Textbox course_id(course->id, GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(0, current_y),
                      sf::Vector2f(100, 100), sf::Color::White);
    course_id.set_outline(sf::Color::Black);
    course_id.align_left();
    texts.push_back(course_id);


    Textbox course_name(course->name, GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(100, current_y),
                        sf::Vector2f(260, 100), sf::Color::White);
    course_name.set_outline(sf::Color::Black);
    course_name.align_left();
    texts.push_back(course_name);


    Textbox credits(std::to_string(course->credits),
                    GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(360, current_y), sf::Vector2f(100, 100),
                    sf::Color::White);
    credits.set_outline(sf::Color::Black);
    texts.push_back(credits);


    Textbox session1(Utils::sessionToStr(course->sessions[0].start) + " - " +
                     Utils::sessionToStr(course->sessions[0].end).substr(4, 5),
                     GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(460, current_y), sf::Vector2f(200, 50),
                     sf::Color::White);
    session1.set_outline(sf::Color::Black);
    texts.push_back(session1);


    Textbox session2(Utils::sessionToStr(course->sessions[1].start) + " - " +
                     Utils::sessionToStr(course->sessions[1].end).substr(4, 5),
                     GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(460, current_y + 50),
                     sf::Vector2f(200, 50), sf::Color::White);
    session2.set_outline(sf::Color::Black);
    texts.push_back(session2);


    Textbox teacher(course->teacher_name.last + " " + course->teacher_name.first,
                    GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(660, current_y), sf::Vector2f(210, 100),
                    sf::Color::White);
    teacher.set_outline(sf::Color::Black);
    teacher.align_left();
    texts.push_back(teacher);


    Textbox slot(std::to_string(course->students.size()) + "/" + std::to_string(course->max_students),
                 GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(870, current_y), sf::Vector2f(100, 100),
                 sf::Color::White);
    slot.set_outline(sf::Color::Black);
    texts.push_back(slot);


    Textbox scores_text("Scores", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(970, current_y),
                        sf::Vector2f(80, 100), sf::Color::White);
    scores_text.set_box_color(sf::Color::Green);

    scores_text.align_center();
    Button_Textbox scores_button(scores_text, sf::Color::Yellow);
    scores_button.set_idle_outline(sf::Color::Black);


    Textbox export_text("Export", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(1050, current_y),
                        sf::Vector2f(80, 100), sf::Color::White);
    export_text.set_box_color(sf::Color::Green);

    export_text.align_center();
    Button_Textbox export_button(export_text, sf::Color::Yellow);
    export_button.set_idle_outline(sf::Color::Black);


    Textbox delete_text("Delete", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(1050 + 80, current_y),
                        sf::Vector2f(70, 100), sf::Color::White);
    delete_text.set_box_color(sf::Color::Red);

    delete_text.align_center();
    Button_Textbox delete_course_button(delete_text, sf::Color::Yellow);
    delete_course_button.set_idle_outline(sf::Color::Black);

    current_y += 100;


    Textbox modify_course_text("Modify course", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(0, current_y),
                               sf::Vector2f(150, 40), sf::Color::White);
    modify_course_text.set_box_color(sf::Color::Green);
    modify_course_text.align_center();
    Button_Textbox modify_course_button(modify_course_text, sf::Color::Yellow);
    modify_course_button.set_idle_outline(sf::Color::Black);

    current_y += 40;
    current_y += 25;


    sf::Texture texture;
    texture.loadFromFile("assets/images/go_back.png");
    Button_Sprite back_button(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));

    texture.loadFromFile("assets/images/previous.jpg");
    Button_Sprite previous_button(texture, sf::Vector2f(910, current_y), sf::Vector2f(40, 40));

    texture.loadFromFile("assets/images/next.jpg");
    Button_Sprite next_button(texture, sf::Vector2f(950, current_y), sf::Vector2f(40, 40));


    int n = students.size();
    page_size = 5;
    int num_row = 0;
    List<DataIter> page;
    List<List<DataIter>> pages;
    students.for_each([&](const auto &Iter) {
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
    Textbox page_num(page_num_string, GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(985, current_y - 5),
                     sf::Vector2f(60, 40), sf::Color::White);
    texts.push_back(page_num);

    current_y += 40;


    Textbox stt_header("No.", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(0, current_y),
                       sf::Vector2f(100, 50), sf::Color::White);
    stt_header.set_outline(sf::Color::Black);

    Textbox student_id_header("Student ID", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(100, current_y),
                              sf::Vector2f(200, 50), sf::Color::White);
    student_id_header.set_outline(sf::Color::Black);

    Textbox student_name_header("Name", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(300, current_y),
                                sf::Vector2f(400, 50), sf::Color::White);
    student_name_header.set_outline(sf::Color::Black);

    Textbox birth_header("Date of birth", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(700, current_y),
                         sf::Vector2f(200, 50), sf::Color::White);
    birth_header.set_outline(sf::Color::Black);

    Textbox class_name_header("Class", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(900, current_y),
                              sf::Vector2f(150, 50), sf::Color::White);
    class_name_header.set_outline(sf::Color::Black);

    Textbox student_action_header("Action", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(1050, current_y),
                                  sf::Vector2f(150, 50), sf::Color::White);
    student_action_header.set_outline(sf::Color::Black);

    current_y += 50;

    texts.push_back(stt_header);
    texts.push_back(class_name_header);
    texts.push_back(student_id_header);
    texts.push_back(student_name_header);
    texts.push_back(birth_header);
    texts.push_back(student_action_header);


    Interaction interaction;
    interaction.add_button(scores_button, go_to_scene8);
    interaction.add_button(export_button, export_students);
    interaction.add_button(delete_course_button, delete_course);
    interaction.add_button(modify_course_button, go_to_modify_course_scene);
    interaction.add_button(back_button, go_back);
    interaction.add_button(previous_button, previous);
    interaction.add_button(next_button, next);
    Button_List<Button_Textbox> disenroll_col;


    if (n > 0) {
        int idx = num_page * page_size;
        pages[num_page].for_each([&](const DataIter &Iter) {
            auto student = Iter.ptr<Student>();
            auto classroom = student->classroom.ptr<Class>();
            ++idx;

            Textbox stt(to_string(idx), GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(0, current_y),
                        sf::Vector2f(100, height), sf::Color::White);
            stt.set_outline(sf::Color::Black);
            stt.align_center();
            texts.push_back(stt);

            Textbox student_id(student->student_id, GUI::defaultSmallCharSize, sf::Color::Black,
                               sf::Vector2f(100, current_y), sf::Vector2f(200, height), sf::Color::White);
            student_id.set_outline(sf::Color::Black);
            student_id.align_center();
            texts.push_back(student_id);


            Textbox name(student->name.toStr(),
                         GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(300, current_y),
                         sf::Vector2f(400, height), sf::Color::White);
            name.set_outline(sf::Color::Black);
            name.align_left();
            texts.push_back(name);


            Textbox birth(Utils::dateToStr(student->birth),
                          GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(700, current_y),
                          sf::Vector2f(200, height), sf::Color::White);
            birth.set_outline(sf::Color::Black);
            texts.push_back(birth);


            Textbox class_name(classroom->name, GUI::defaultSmallCharSize, sf::Color::Black,
                               sf::Vector2f(900, current_y), sf::Vector2f(150, height), sf::Color::White);
            class_name.set_outline(sf::Color::Black);
            class_name.align_center();
            texts.push_back(class_name);


            Textbox delete_text("Disenroll", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(1050, current_y),
                                sf::Vector2f(150, height), sf::Color::White);
            delete_text.set_box_color(sf::Color::Red);
            delete_text.align_center();
            Button_Textbox delete_button(delete_text, sf::Color::Yellow);
            delete_button.set_idle_outline(sf::Color::Black);
            disenroll_col.add_button(delete_button);

            current_y += height;
        });
    }

    disenroll_col.trigger = disenroll_student;
    interaction.add_button_list(disenroll_col);


    while (window->isOpen() && !app->scenes.empty() && app->scenes.top() == current_scene && !app->scenes.refresh) {
        window->clear(sf::Color::White);

        texts.for_each([&](Textbox &textbox) {
            textbox.draw(*window, app->default_font);
        });

        interaction.draw(*window, app->default_font);
        window->display();

        auto event = interaction.interact(*window);
        app->scenes.interact(event);
    }
}