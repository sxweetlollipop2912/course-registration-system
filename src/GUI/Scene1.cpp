#include "Scene1.h"

using std::to_string;

static Input_Textbox *startInputBoxP;
static Input_Textbox *endInputBoxP;
static App *app;
static sf::RenderWindow *windowP;
static Button_Textbox *currentYearButtonP;
static Interaction *interactionP;
static Input_Textbox *studenIdInputBoxP;

static bool inCreate;

static void go_back(int dummy) {
    app->scenes.pop();
}

static void go_to_scene2(int dummy) {
    app->scenes.push(SceneType::Scene2);
}

static void create_new_year_function(int dummy) {
    try {
        int start_year = std::stoi(startInputBoxP->text);
        int end_year = std::stoi(endInputBoxP->text);
        auto default_year = make_shared<SchoolYear>(start_year, end_year);

        inCreate = !app->addDefaultSchoolYear(default_year);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        inCreate = true;
    }
}

static void create_new_year(int dummy) {
    Textbox mainBackground("", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4, windowHeight / 3),
                           sf::Vector2f(windowWidth / 2, windowHeight / 3), sf::Color::Black);
    mainBackground.set_outline(sf::Color::Blue, 4);

    Textbox startText("Start year:", defaultMediumCharSize, sf::Color::White,
                      sf::Vector2f(windowWidth / 4 + 20, windowHeight / 3 + 30), sf::Vector2f(130, 50),
                      sf::Color::Transparent);
    Textbox startBox("", defaultMediumCharSize, sf::Color::Black,
                     sf::Vector2f(windowWidth / 4 + 170, windowHeight / 3 + 30), sf::Vector2f(410, 50),
                     sf::Color::White);
    Input_Textbox startInputBox(startBox, 20, sf::Color::Blue);
    startInputBoxP = &startInputBox;

    Textbox endText("End year:", defaultMediumCharSize, sf::Color::White,
                    sf::Vector2f(windowWidth / 4 + 20, windowHeight / 3 + 100), sf::Vector2f(130, 50),
                    sf::Color::Transparent);
    Textbox endBox("", defaultMediumCharSize, sf::Color::Black,
                   sf::Vector2f(windowWidth / 4 + 170, windowHeight / 3 + 100), sf::Vector2f(410, 50),
                   sf::Color::White);
    Input_Textbox endInputBox(endBox, 20, sf::Color::Blue);
    endInputBoxP = &endInputBox;


    Textbox enterBox("ENTER", defaultMediumCharSize, sf::Color::White,
                     sf::Vector2f(windowWidth / 2 - 65, windowHeight / 3 + 220), sf::Vector2f(130, 50),
                     sf::Color::Blue);
    Button_Textbox enterButton(enterBox, sf::Color::White);

    Interaction interaction;

    interaction.add_input_textbox(startInputBox);
    interaction.add_input_textbox(endInputBox);


    interaction.add_button(enterButton, create_new_year_function);
    while (windowP->isOpen() && app->year() == nullptr) {
        windowP->clear(sf::Color::White);
        mainBackground.draw(*windowP, app->default_font);
        interaction.draw(*windowP, app->default_font);
        startText.draw(*windowP, app->default_font);
        endText.draw(*windowP, app->default_font);
        windowP->display();

        auto event = interaction.interact(*windowP);
        app->scenes.interact(event);
    }

    if (app->year()) {
        currentYearButtonP->textbox.set_text(
                to_string(app->year()->start_year) + "-" + to_string(app->year()->end_year));
        interactionP->add_button(*currentYearButtonP, go_to_scene2);
    }

}

void stop_add_staff(int dummy)
{
    inCreate = false;
}

void add_staff_function(int dummy) {
    auto csvData = CSVIO::tryParse("csv/" + studenIdInputBoxP->text);
    if (!csvData.empty() && app->addStaffs(csvData))
        inCreate = false;
}

void add_staff(int dummy) {
    Interaction interaction;
    Textbox mainBackground("", defaultMediumCharSize, sf::Color::White,
                           sf::Vector2f(windowWidth / 2 - 300, windowHeight / 2 - 250), sf::Vector2f(600, 300),
                           sf::Color::Black);
    mainBackground.set_outline(sf::Color::Blue, 4);

    Textbox studenIdText("file Name:", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4 + 20, 275),
                         sf::Vector2f(130, 50), sf::Color::Transparent);
    Textbox studenIdBox("", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 4 + 170, 275),
                        sf::Vector2f(410, 50), sf::Color::White);
    Input_Textbox studenIdInputBox(studenIdBox, 25, sf::Color::Blue);
    interaction.add_input_textbox(studenIdInputBox);

    Textbox enterBox("ENTER", defaultMediumCharSize, sf::Color::White,
                     sf::Vector2f(windowWidth / 2 - 65, windowHeight / 2 - 50), sf::Vector2f(130, 50), sf::Color::Blue);
    Button_Textbox enterButton(enterBox, sf::Color::White);
    interaction.add_button(enterButton, add_staff_function);

    sf::Texture texture;
    texture.loadFromFile("assets/images/go_back.png");
    Button_Sprite back_button = Button_Sprite(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));
    interaction.add_button(back_button, stop_add_staff);

    studenIdInputBoxP = &studenIdInputBox;

    inCreate = true;
    while (windowP->isOpen() && inCreate) {

        windowP->clear(sf::Color::White);
        mainBackground.draw(*windowP, app->default_font);
        interaction.draw(*windowP, app->default_font);
        studenIdText.draw(*windowP, app->default_font);
        windowP->display();

        auto event = interaction.interact(*windowP);
        app->scenes.interact(event);
    }
}

void scene1(sf::RenderWindow &window, App &_app) {
    windowP = &window;
    app = &_app;
    auto user = app->user();
    Interaction interaction, interaction2;

    Textbox nameText(
            "Welcome, " + (string) (user->gender == Gender::Female ? "Ms. " : "Mr. ") + user->name.toStr() + ".",
            defaultMediumCharSize, sf::Color::Black,
            sf::Vector2f(10, 100), sf::Vector2f(200, 50),
            sf::Color::Transparent);
    nameText.align_left();

    Textbox currentYearText("Current year", defaultMediumCharSize, sf::Color::Black,
                            sf::Vector2f(windowWidth / 2 - 100, windowHeight / 2 - 100), sf::Vector2f(200, 50),
                            sf::Color::Transparent);
    Textbox currentYear("", defaultMediumCharSize, sf::Color::White,
                        sf::Vector2f(windowWidth / 2 - 100, windowHeight / 2 - 25), sf::Vector2f(200, 50),
                        sf::Color::Black);
    Button_Textbox currentYearButton(currentYear, sf::Color::Blue);
    currentYearButtonP = &currentYearButton;

    Textbox noCurrentYearText("No current year!", defaultMediumCharSize, sf::Color::Black,
                              sf::Vector2f(windowWidth / 2 - 100, windowHeight / 2 - 100), sf::Vector2f(200, 50),
                              sf::Color::Transparent);
    Textbox createYearBox("Create new year", defaultMediumCharSize, sf::Color::White,
                          sf::Vector2f(windowWidth / 2 - 100, windowHeight / 2 - 25), sf::Vector2f(200, 50),
                          sf::Color::Black);
    Button_Textbox createYearButton(createYearBox, sf::Color::Blue);
    interaction2.add_button(createYearButton, create_new_year);

    Textbox addStaffBox("Add staff", defaultMediumCharSize, sf::Color::White,
                        sf::Vector2f(windowWidth / 2 - 100, 200), sf::Vector2f(200, 50), sf::Color::Black);
    Button_Textbox addStaffButton(addStaffBox, sf::Color::Blue);
    interaction.add_button(addStaffButton, add_staff);
    interaction2.add_button(addStaffButton, add_staff);


    sf::Texture texture;
    texture.loadFromFile("assets/images/go_back.png");
    Button_Sprite back_button = Button_Sprite(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));
    interaction.add_button(back_button, go_back);
    interaction2.add_button(back_button, go_back);

    interactionP = &interaction;

    if (app->year() != nullptr) {
        currentYearButton.textbox.set_text(to_string(app->year()->start_year) + "-" + to_string(app->year()->end_year));
        interaction.add_button(currentYearButton, go_to_scene2);
    }

    while (window.isOpen() && app->scenes.top() == SceneType::Scene1) {
        if (app->year() != nullptr) {
            window.clear(sf::Color::White);
            currentYearText.draw(window, app->default_font);
            interaction.draw(window, app->default_font);
            window.display();

            auto event = interaction.interact(window);
            app->scenes.interact(event);
        } else {
            window.clear(sf::Color::White);
            noCurrentYearText.draw(window, app->default_font);
            nameText.draw(window, app->default_font);
            interaction2.draw(window, app->default_font);
            window.display();

            auto event = interaction2.interact(window);
            app->scenes.interact(event);
        }
    }
}