#include <iomanip>
#include <ctime>

#include "Scene8.h"
#include "../Constants.h"
#include "../App/Account.h"

using std::string, std::to_string, std::tm;

static App *app;
static sf::RenderWindow *window;
static Input_Textbox *midtermInput, *finalInput, *totalInput, *otherInput, *importSource;
static DataIter *student_iter;

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

static void previous(int dummy) {
    num_page = std::max(num_page - 1, 0);
    app->scenes.refresh = true;
}

static void next(int dummy) {
    num_page = std::min(num_page + 1, page_cnt - 1);
    app->scenes.refresh = true;
}

static void go_to_modify_score_scene(int dummy) {
    dummy += num_page * page_size;
    auto course = app->scenes.arg.ptr<Course>();
    student_iter = &course->students[dummy];

    app->scenes.push(SceneType::SceneModifyScore);
}

static void go_to_modify_course_scene(int dummy) {
    app->scenes.push(SceneType::SceneModifyCourse);
}

static void go_to_import_score_scene(int dummy) {
    app->scenes.push(SceneType::ImportScoreScene);
}

static void export_students(int dummy) {
    auto course = app->scenes.arg.ptr<Course>();
    app->exportStudentsInCourse(app->scenes.arg, PATH::EXPORTS + course->id + "_students.csv");
}

static void delete_course(int dummy) {
    if (app->deleteCourse(app->scenes.arg)) {
        app->scenes.arg.clear();

        app->scenes.pop();
    }
}

static void erase_score(int dummy) {
    dummy += num_page * page_size;
    auto course = app->scenes.arg.ptr<Course>();

    auto score = course->students[dummy].ptr<Student>()->getScore(course->uid);
    score->erase();

    app->scenes.refresh = true;
}

static void modify_score(int dummy) {
    auto score = student_iter->ptr<Student>()->getScore(app->scenes.arg.ptr<Course>()->uid);

    try {
        double midterm = std::stod(midtermInput->text);
        double final = std::stod(finalInput->text);
        double total = std::stod(totalInput->text);
        double other = std::stod(otherInput->text);

        score->midterm = std::max((double) -1, midterm);
        score->final = std::max((double) -1, final);
        score->total = std::max((double) -1, total);
        score->other = std::max((double) -1, other);

        go_back(0);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

static void import_score(int dummy) {
    auto csvData = CSVIO::tryParse("./csv/" + importSource->text);

    if (!csvData.empty() && app->addScores(csvData, app->scenes.arg)) {
        go_back(0);
    }
}

void modifyScoreScene(sf::RenderWindow &_window, App &_app) {
    app = &_app;
    window = &_window;
    SceneType current_scene = SceneType::SceneModifyScore;

    auto year = app->year();
    auto semester = app->semester();

    shared_ptr<Course> course;
    try {
        course = app->scenes.arg.ptr<Course>();
        if (!course)
            throw std::out_of_range("modifyScoreScene: scenes.arg is null.");
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;

        app->scenes.pop();
        return;
    }

    shared_ptr<Student> student;
    try {
        student = student_iter->ptr<Student>();
        if (!student)
            throw std::out_of_range("modifyScoreScene: student_iter is null.");
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;

        app->scenes.pop();
        return;
    }
    auto score = student->getScore(course->uid);


    float diff_y = 60, current_y = -20, padding_left = GUI::windowWidth / 4, padding_top = GUI::windowHeight / 4;
    sf::Texture texture;
    List<Textbox> texts;
    Interaction interaction;


    Textbox titleBox("Modify score of student " + student->student_id + " - " + student->name.toStr() + ":",
                     24,
                     sf::Color::Black,
                     sf::Vector2f(padding_left, padding_top - 50),
                     sf::Vector2f(100, 30),
                     sf::Color::White);
    titleBox.align_left();
    texts.push_back(titleBox);


    Textbox mainBackground("",
                           GUI::defaultSmallCharSize,
                           sf::Color::White,
                           sf::Vector2f(padding_left, padding_top),
                           sf::Vector2f(GUI::windowWidth / 2, 380),
                           sf::Color::Black);
    mainBackground.set_outline(sf::Color::Blue, 4);
    texts.push_back(mainBackground);


    Textbox midtermText("Midterm",
                        GUI::defaultSmallCharSize,
                        sf::Color::Black,
                        sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                        sf::Vector2f(130, 50),
                        sf::Color::Cyan);
    texts.push_back(midtermText);
    Textbox midtermBox((score->valid() ? double_to_string(score->midterm) : "0"),
                       GUI::defaultSmallCharSize,
                       sf::Color::Black,
                       sf::Vector2f(padding_left + 170, midtermText.box.getPosition().y),
                       sf::Vector2f(410, 50),
                       sf::Color::White);
    Input_Textbox midtermInputBox(midtermBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(midtermInputBox);
    midtermInput = &midtermInputBox;


    Textbox finalText("Final",
                      GUI::defaultSmallCharSize,
                      sf::Color::Black,
                      sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                      sf::Vector2f(130, 50),
                      sf::Color::Cyan);
    texts.push_back(finalText);
    Textbox finalBox((score->valid() ? double_to_string(score->final) : "0"),
                     GUI::defaultSmallCharSize,
                     sf::Color::Black,
                     sf::Vector2f(padding_left + 170, finalText.box.getPosition().y),
                     sf::Vector2f(410, 50),
                     sf::Color::White);
    Input_Textbox finalInputBox(finalBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(finalInputBox);
    finalInput = &finalInputBox;


    Textbox totalText("Total",
                      GUI::defaultSmallCharSize,
                      sf::Color::Black,
                      sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                      sf::Vector2f(130, 50),
                      sf::Color::Cyan);
    texts.push_back(totalText);
    Textbox totalBox((score->valid() ? double_to_string(score->total) : "0"),
                     GUI::defaultSmallCharSize,
                     sf::Color::Black,
                     sf::Vector2f(padding_left + 170, totalText.box.getPosition().y),
                     sf::Vector2f(410, 50),
                     sf::Color::White);
    Input_Textbox teacherInputBox(totalBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(teacherInputBox);
    totalInput = &teacherInputBox;


    Textbox otherText("Other",
                      GUI::defaultSmallCharSize,
                      sf::Color::Black,
                      sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                      sf::Vector2f(130, 50),
                      sf::Color::Cyan);
    texts.push_back(otherText);
    Textbox otherBox((score->valid() ? double_to_string(score->other) : "0"),
                     GUI::defaultSmallCharSize,
                     sf::Color::Black,
                     sf::Vector2f(padding_left + 170, otherText.box.getPosition().y),
                     sf::Vector2f(410, 50),
                     sf::Color::White);
    Input_Textbox otherInputBox(otherBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(otherInputBox);
    otherInput = &otherInputBox;


    Textbox setBox("SET",
                   GUI::defaultMediumCharSize,
                   sf::Color::Black,
                   sf::Vector2f(padding_left * 2 - 65, padding_top + (current_y += (diff_y * 1.3))),
                   sf::Vector2f(130, 50),
                   sf::Color::Cyan);
    Button_Textbox setButton(setBox, sf::Color::White);
    interaction.add_button(setButton, modify_score);


    texture.loadFromFile(PATH::IMAGES + "go_back.png");
    Button_Sprite back_button(texture,
                              sf::Vector2f(10, 5),
                              sf::Vector2f(40, 40));
    interaction.add_button(back_button, go_back);


    while (window->isOpen() && !app->scenes.empty() && app->scenes.top() == current_scene && !app->scenes.refresh) {
        window->clear(sf::Color::White);

        for (auto &box: texts)
            box.draw(*window, app->default_font);
        interaction.draw(*window, app->default_font);
        window->display();

        auto event = interaction.interact(*window);
        app->scenes.interact(event);
    }
}

void importScoreScene(sf::RenderWindow &_window, App &_app) {
    app = &_app;
    window = &_window;
    SceneType current_scene = SceneType::ImportScoreScene;

    shared_ptr<Course> course;
    try {
        course = app->scenes.arg.ptr<Course>();
        if (!course)
            throw std::out_of_range("importScoreScene: scenes.arg is null.");
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;

        app->scenes.pop();
        return;
    }


    float diff_y = 60, current_y = -20, padding_left = GUI::windowWidth / 4, padding_top = GUI::windowHeight / 3;
    sf::Texture texture;
    List<Textbox> texts;
    Interaction interaction;


    Textbox titleBox("Enter filename to import:",
                     24,
                     sf::Color::Black,
                     sf::Vector2f(padding_left, padding_top - 50),
                     sf::Vector2f(100, 30),
                     sf::Color::White);
    titleBox.align_left();
    texts.push_back(titleBox);


    Textbox mainBackground("",
                           GUI::defaultSmallCharSize,
                           sf::Color::White,
                           sf::Vector2f(padding_left, padding_top),
                           sf::Vector2f(GUI::windowWidth / 2, 200),
                           sf::Color::Black);
    mainBackground.set_outline(sf::Color::Blue, 4);
    texts.push_back(mainBackground);


    Textbox pathText("Filename",
                     GUI::defaultSmallCharSize,
                     sf::Color::Black,
                     sf::Vector2f(padding_left + 20, padding_top + (current_y += diff_y)),
                     sf::Vector2f(130, 50),
                     sf::Color::Cyan);
    texts.push_back(pathText);
    Textbox pathBox({},
                    GUI::defaultSmallCharSize,
                    sf::Color::Black,
                    sf::Vector2f(padding_left + 170, pathText.box.getPosition().y),
                    sf::Vector2f(410, 50),
                    sf::Color::White);
    Input_Textbox pathInputBox(pathBox, 20, sf::Color::Blue);
    interaction.add_input_textbox(pathInputBox);
    importSource = &pathInputBox;


    Textbox setBox("SET",
                   GUI::defaultMediumCharSize,
                   sf::Color::Black,
                   sf::Vector2f(padding_left * 2 - 65, padding_top + (current_y += (diff_y * 1.3))),
                   sf::Vector2f(130, 50),
                   sf::Color::Cyan);
    Button_Textbox setButton(setBox, sf::Color::White);
    interaction.add_button(setButton, import_score);


    texture.loadFromFile(PATH::IMAGES + "go_back.png");
    Button_Sprite back_button(texture,
                              sf::Vector2f(10, 5),
                              sf::Vector2f(40, 40));
    interaction.add_button(back_button, go_back);


    while (window->isOpen() && !app->scenes.empty() && app->scenes.top() == current_scene && !app->scenes.refresh) {
        window->clear(sf::Color::White);

        for (auto &box: texts)
            box.draw(*window, app->default_font);
        interaction.draw(*window, app->default_font);
        window->display();

        auto event = interaction.interact(*window);
        app->scenes.interact(event);
    }
}

void scene8(sf::RenderWindow &_window, App &_app) {
    app = &_app;
    window = &_window;
    auto current_scene = SceneType::Scene8;

    auto year = app->year();
    auto semester = app->semester();

    shared_ptr<Course> course;
    try {
        course = app->scenes.arg.ptr<Course>();
        if (!course)
            throw std::out_of_range("scene8: scenes.arg is null.");
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
                               sf::Vector2f(300, 50), sf::Color::White);
    course_name_header.set_outline(sf::Color::Black);

    Textbox credits_header("Credits", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(400, current_y),
                           sf::Vector2f(100, 50), sf::Color::White);
    credits_header.set_outline(sf::Color::Black);

    Textbox sessions_header("Sessions", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(500, current_y),
                            sf::Vector2f(200, 50), sf::Color::White);
    sessions_header.set_outline(sf::Color::Black);

    Textbox teacher_header("Teacher", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(700, current_y),
                           sf::Vector2f(250, 50), sf::Color::White);
    teacher_header.set_outline(sf::Color::Black);

    Textbox slot_header("Slot", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(950, current_y),
                        sf::Vector2f(100, 50), sf::Color::White);
    slot_header.set_outline(sf::Color::Black);

    Textbox course_action_header("Action", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(1050, current_y),
                                 sf::Vector2f(150, 50), sf::Color::White);
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
                        sf::Vector2f(300, 100), sf::Color::White);
    course_name.set_outline(sf::Color::Black);
    course_name.align_left();
    texts.push_back(course_name);


    Textbox credits(std::to_string(course->credits),
                    GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(400, current_y), sf::Vector2f(100, 100),
                    sf::Color::White);
    credits.set_outline(sf::Color::Black);
    texts.push_back(credits);


    Textbox session1(Utils::sessionToStr(course->sessions[0].start) + " - " +
                     Utils::sessionToStr(course->sessions[0].end).substr(4, 5),
                     GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(500, current_y), sf::Vector2f(200, 50),
                     sf::Color::White);
    session1.set_outline(sf::Color::Black);
    texts.push_back(session1);


    Textbox session2(Utils::sessionToStr(course->sessions[1].start) + " - " +
                     Utils::sessionToStr(course->sessions[1].end).substr(4, 5),
                     GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(500, current_y + 50),
                     sf::Vector2f(200, 50), sf::Color::White);
    session2.set_outline(sf::Color::Black);
    texts.push_back(session2);


    Textbox teacher(course->teacher_name.last + " " + course->teacher_name.first,
                    GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(700, current_y), sf::Vector2f(250, 100),
                    sf::Color::White);
    teacher.set_outline(sf::Color::Black);
    teacher.align_left();
    texts.push_back(teacher);


    Textbox slot(std::to_string(course->students.size()) + "/" + std::to_string(course->max_students),
                 GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(950, current_y), sf::Vector2f(100, 100),
                 sf::Color::White);
    slot.set_outline(sf::Color::Black);
    texts.push_back(slot);


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

    Textbox import_text("Import score", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(0, current_y),
                        sf::Vector2f(150, 40), sf::Color::White);
    import_text.set_box_color(sf::Color::Green);
    import_text.align_center();
    Button_Textbox import_button(import_text, sf::Color::Yellow);
    import_button.set_idle_outline(sf::Color::Black);


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
                       sf::Vector2f(95, 50), sf::Color::White);
    stt_header.set_outline(sf::Color::Black);

    Textbox student_id_header("Student ID", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(95, current_y),
                              sf::Vector2f(175, 50), sf::Color::White);
    student_id_header.set_outline(sf::Color::Black);

    Textbox student_name_header("Name", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(270, current_y),
                                sf::Vector2f(380, 50), sf::Color::White);
    student_name_header.set_outline(sf::Color::Black);

    Textbox midterm_header("Midterm", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(650, current_y),
                           sf::Vector2f(100, 50), sf::Color::White);
    midterm_header.set_outline(sf::Color::Black);

    Textbox final_header("Final", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(750, current_y),
                         sf::Vector2f(100, 50), sf::Color::White);
    final_header.set_outline(sf::Color::Black);

    Textbox total_header("Total", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(850, current_y),
                         sf::Vector2f(100, 50), sf::Color::White);
    total_header.set_outline(sf::Color::Black);

    Textbox other_header("Other", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(950, current_y),
                         sf::Vector2f(100, 50), sf::Color::White);
    other_header.set_outline(sf::Color::Black);

    Textbox student_action_header("Action", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(1050, current_y),
                                  sf::Vector2f(150, 50), sf::Color::White);
    student_action_header.set_outline(sf::Color::Black);

    current_y += 50;

    texts.push_back(stt_header);
    texts.push_back(student_id_header);
    texts.push_back(student_name_header);
    texts.push_back(midterm_header);
    texts.push_back(final_header);
    texts.push_back(total_header);
    texts.push_back(other_header);
    texts.push_back(student_action_header);


    Interaction interaction;
    interaction.add_button(export_button, export_students);
    interaction.add_button(delete_course_button, delete_course);
    interaction.add_button(modify_course_button, go_to_modify_course_scene);
    interaction.add_button(back_button, go_back);
    interaction.add_button(previous_button, previous);
    interaction.add_button(next_button, next);
    interaction.add_button(import_button, go_to_import_score_scene);
    Button_List<Button_Textbox> erase_col, modify_col;


    if (n > 0) {
        int idx = num_page * page_size;
        pages[num_page].for_each([&](const DataIter &Iter) {
            auto student = Iter.ptr<Student>();
            auto score = student->getScore(course->uid);
            ++idx;

            Textbox stt(to_string(idx), GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(0, current_y),
                        sf::Vector2f(95, height), sf::Color::White);
            stt.set_outline(sf::Color::Black);
            stt.align_center();
            texts.push_back(stt);

            Textbox student_id(student->student_id, GUI::defaultSmallCharSize, sf::Color::Black,
                               sf::Vector2f(95, current_y), sf::Vector2f(175, height), sf::Color::White);
            student_id.set_outline(sf::Color::Black);
            student_id.align_center();
            texts.push_back(student_id);


            Textbox name(student->name.toStr(),
                         GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(270, current_y),
                         sf::Vector2f(380, height), sf::Color::White);
            name.set_outline(sf::Color::Black);
            name.align_left();
            texts.push_back(name);


            Textbox midterm((score->valid() ? double_to_string(score->midterm) : "X"),
                            GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(650, current_y),
                            sf::Vector2f(100, height), sf::Color::White);
            midterm.set_outline(sf::Color::Black);
            texts.push_back(midterm);


            Textbox final((score->valid() ? double_to_string(score->final) : "X"),
                          GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(750, current_y),
                          sf::Vector2f(100, height), sf::Color::White);
            final.set_outline(sf::Color::Black);
            texts.push_back(final);

            Textbox total((score->valid() ? double_to_string(score->total) : "X"),
                          GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(850, current_y),
                          sf::Vector2f(100, height), sf::Color::White);
            total.set_outline(sf::Color::Black);
            texts.push_back(total);

            Textbox other((score->valid() ? double_to_string(score->other) : "X"),
                          GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(950, current_y),
                          sf::Vector2f(100, height), sf::Color::White);
            other.set_outline(sf::Color::Black);
            texts.push_back(other);


            Textbox modify_text("Modify", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(1050, current_y),
                                sf::Vector2f(80, height), sf::Color::White);
            modify_text.set_box_color(sf::Color::Green);
            modify_text.align_center();
            Button_Textbox modify_button(modify_text, sf::Color::Yellow);
            modify_button.set_idle_outline(sf::Color::Black);
            modify_col.add_button(modify_button);


            Textbox erase_text("Erase", GUI::defaultSmallCharSize, sf::Color::Black, sf::Vector2f(1050 + 80, current_y),
                               sf::Vector2f(70, height), sf::Color::White);
            erase_text.set_box_color(sf::Color::Red);
            erase_text.align_center();
            Button_Textbox erase_button(erase_text, sf::Color::Yellow);
            erase_button.set_idle_outline(sf::Color::Black);
            erase_col.add_button(erase_button);

            current_y += height;
        });
    }

    modify_col.trigger = go_to_modify_score_scene;
    interaction.add_button_list(modify_col);
    erase_col.trigger = erase_score;
    interaction.add_button_list(erase_col);


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