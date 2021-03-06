#include "Scene2.h"

using std::to_string;

static App *app;
static sf::RenderWindow *windowP;
static bool inCreate;

static Input_Textbox *dayInputBoxP;
static Input_Textbox *monthInputBoxP;
static Input_Textbox *yearInputBoxP;

static Input_Textbox *dayInputBox2P;
static Input_Textbox *monthInputBox2P;
static Input_Textbox *yearInputBox2P;

static void go_back() {
    app->scenes.pop();
}

static void draw_semester(sf::RenderWindow &window, sf::Vector2i mousePos) {
    Textbox semesterBox("", defaultMediumCharSize, sf::Color::White, sf::Vector2f(0, 0), sf::Vector2f(200, 50),
                        sf::Color::Black);
    for (auto &semester: app->year()->semesters) {
        auto ptr = semester.ptr<Semester>(); // ptr cua semester
        semesterBox.set_text("Semester " + to_string(ptr->no));
        semesterBox.set_box_position(sf::Vector2f(windowWidth / 2 - 100, 200 + (ptr->no) * 75));
        if (semesterBox.inside(mousePos.x, mousePos.y)) {
            semesterBox.set_outline(sf::Color::Blue);
        } else semesterBox.set_outline(sf::Color::Transparent);
        semesterBox.draw(window, app->default_font);
    }
}

static void create_semester_function(int dummy) {
    try {
        int maxNo = 0;
        for (auto &semester: app->year()->semesters) {
            auto ptr = semester.ptr<Semester>(); // ptr cua semester
            if (ptr->no > maxNo) maxNo = ptr->no;
        }
        maxNo++;
        auto semesterTmp = make_shared<Semester>(maxNo,
                                                 Utils::mktm(std::stoi(dayInputBoxP->text),
                                                             std::stoi(monthInputBoxP->text),
                                                             std::stoi(yearInputBoxP->text)),
                                                 Utils::mktm(std::stoi(dayInputBox2P->text),
                                                             std::stoi(monthInputBox2P->text),
                                                             std::stoi(yearInputBox2P->text)));

        inCreate = !app->addDefaultSemester(semesterTmp);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;

        inCreate = true;
    }
}

static void stop_create(int dummy) {
    inCreate = false;
}

static void create_semester() {
    Textbox mainBackground("", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4, windowHeight / 3),
                           sf::Vector2f(windowWidth / 2, windowHeight / 3), sf::Color::Black);
    mainBackground.set_outline(sf::Color::Blue, 4);

    Interaction interaction;
    Textbox startDateText("Start Date", defaultMediumCharSize, sf::Color::White,
                          sf::Vector2f(windowWidth / 3 - 50, 370), sf::Vector2f(100, 50), sf::Color::Transparent);
    Textbox endDateText("End Date", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 3 - 50, 470),
                        sf::Vector2f(100, 50), sf::Color::Transparent);

    Textbox dayText("Day", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 3 + 90, 300),
                    sf::Vector2f(100, 50), sf::Color::Transparent);
    Textbox monthText("Month", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 3 + 240, 300),
                      sf::Vector2f(100, 50), sf::Color::Transparent);
    Textbox yearText("Year", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 3 + 390, 300),
                     sf::Vector2f(100, 50), sf::Color::Transparent);

    Textbox dayBox("", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 3 + 90, 370),
                   sf::Vector2f(100, 50), sf::Color::White);
    Textbox monthBox("", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 3 + 240, 370),
                     sf::Vector2f(100, 50), sf::Color::White);
    Textbox yearBox("", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 3 + 390, 370),
                    sf::Vector2f(100, 50), sf::Color::White);

    Textbox dayBox2("", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 3 + 90, 470),
                    sf::Vector2f(100, 50), sf::Color::White);
    Textbox monthBox2("", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 3 + 240, 470),
                      sf::Vector2f(100, 50), sf::Color::White);
    Textbox yearBox2("", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 3 + 390, 470),
                     sf::Vector2f(100, 50), sf::Color::White);

    Input_Textbox dayInputBox(dayBox, 2, sf::Color::Blue);
    Input_Textbox monthInputBox(monthBox, 2, sf::Color::Blue);
    Input_Textbox yearInputBox(yearBox, 4, sf::Color::Blue);
    dayInputBoxP = &dayInputBox;
    monthInputBoxP = &monthInputBox;
    yearInputBoxP = &yearInputBox;


    Input_Textbox dayInputBox2(dayBox2, 2, sf::Color::Blue);
    Input_Textbox monthInputBox2(monthBox2, 2, sf::Color::Blue);
    Input_Textbox yearInputBox2(yearBox2, 4, sf::Color::Blue);

    dayInputBox2P = &dayInputBox2;
    monthInputBox2P = &monthInputBox2;
    yearInputBox2P = &yearInputBox2;

    interaction.add_input_textbox(dayInputBox);
    interaction.add_input_textbox(monthInputBox);
    interaction.add_input_textbox(yearInputBox);

    interaction.add_input_textbox(dayInputBox2);
    interaction.add_input_textbox(monthInputBox2);
    interaction.add_input_textbox(yearInputBox2);

    Textbox enterBox("ENTER", defaultMediumCharSize, sf::Color::White,
                     sf::Vector2f(windowWidth / 2 - 65, windowHeight / 3 + 235), sf::Vector2f(130, 50),
                     sf::Color::Blue);
    Button_Textbox enterButton(enterBox, sf::Color::White);

    interaction.add_button(enterButton, create_semester_function);

    sf::Texture texture;
    texture.loadFromFile("assets/images/go_back.png");
    Button_Sprite back_button = Button_Sprite(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));
    interaction.add_button(back_button, stop_create);

    inCreate = true;
    while (windowP->isOpen() && inCreate) {
        windowP->clear(sf::Color::White);
        mainBackground.draw(*windowP, app->default_font);
        startDateText.draw(*windowP, app->default_font);
        endDateText.draw(*windowP, app->default_font);

        dayText.draw(*windowP, app->default_font);
        monthText.draw(*windowP, app->default_font);
        yearText.draw(*windowP, app->default_font);

        interaction.draw(*windowP, app->default_font);
        windowP->display();

        auto event = interaction.interact(*windowP);
        app->scenes.interact(event);
    }
}

static void clickSemester(sf::Vector2i mousePos) {
    for (auto &semester: app->year()->semesters) {
        auto ptr = semester.ptr<Semester>(); // ptr cua semester
        if (mousePos.x >= windowWidth / 2 - 100 && mousePos.x <= windowWidth / 2 + 100
            && mousePos.y >= 200 + (ptr->no) * 75 && mousePos.y <= 250 + (ptr->no) * 75) {
            app->setDefaultSemester(ptr->no);
            app->scenes.push(SceneType::Scene5);
            return;
            //printf("%d\n", ptr->no);
        }
    }
}

static void remove_year() {
    app->deleteDefaultSchoolYear();
    go_back();
}

void scene2(sf::RenderWindow &window, App &_app) {
    windowP = &window;
    app = &_app;

    Textbox semesterText("Semester", 40, sf::Color::Black, sf::Vector2f(windowWidth / 2 - 100, 200),
                         sf::Vector2f(200, 50), sf::Color::Transparent);
    Textbox createSemesterBox("Create semester", defaultMediumCharSize, sf::Color::Black,
                              sf::Vector2f(windowWidth / 2 - 100, 200 - 75), sf::Vector2f(200, 50), sf::Color::Green);

    Textbox viewClassBox("View classes", defaultMediumCharSize, sf::Color::Black,
                         sf::Vector2f(windowWidth / 2 - 100, 200 - 150), sf::Vector2f(200, 50), sf::Color::Green);
    Textbox removeYearBox("Remove year", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth - 250, 100),
                          sf::Vector2f(200, 50), sf::Color::Red);
    sf::Texture texture;
    texture.loadFromFile("assets/images/go_back.png");
    Button_Sprite back_button = Button_Sprite(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));

    while (window.isOpen() && !app->scenes.empty() && app->scenes.top() == SceneType::Scene2) {
        sf::Event event;
        sf::Vector2i mousePos = Mouse::getPosition(window);

        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
                app->scenes.clear();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (viewClassBox.inside(mousePos.x, mousePos.y)) {
                        app->scenes.push(SceneType::Scene3);
                        return;
                    }
                    if (removeYearBox.inside(mousePos.x, mousePos.y)) {
                        remove_year();
                        return;
                    }
                    if (mousePos.x >= 10 && mousePos.x <= 50 && mousePos.y >= 5 && mousePos.y <= 45) {
                        go_back();
                        return;
                    }
                    if (mousePos.x >= windowWidth / 2 - 100 && mousePos.x <= windowWidth / 2 + 100 &&
                        mousePos.y >= 125 && mousePos.y <= 175) {
                        create_semester();
                    }
                    clickSemester(mousePos);
                }
            }
        }
        window.clear(sf::Color::White);
        semesterText.draw(window, app->default_font);
        if (mousePos.x >= 10 && mousePos.x <= 50 && mousePos.y >= 5 && mousePos.y <= 45) {
            back_button.idle = false;
        } else back_button.idle = true;
        back_button.draw(window, app->default_font);

        if (mousePos.x >= windowWidth / 2 - 100 && mousePos.x <= windowWidth / 2 + 100 && mousePos.y >= 125 &&
            mousePos.y <= 175) {
            createSemesterBox.set_outline(sf::Color::Blue);
        } else createSemesterBox.set_outline(sf::Color::Transparent);
        createSemesterBox.draw(window, app->default_font);

        if (viewClassBox.inside(mousePos.x, mousePos.y)) viewClassBox.set_outline(sf::Color::Blue);
        else viewClassBox.set_outline(sf::Color::Transparent);
        viewClassBox.draw(window, app->default_font);

        if (removeYearBox.inside(mousePos.x, mousePos.y)) removeYearBox.set_outline(sf::Color::Blue);
        else removeYearBox.set_outline(sf::Color::Transparent);
        removeYearBox.draw(window, app->default_font);

        draw_semester(window, mousePos);
        window.display();
    }
}