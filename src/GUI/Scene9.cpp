#include "Scene9.h"
#include "../App/Utils.h"
#include "../Constants.h"

static std::string to_string(const Gender &gender) {
    switch (gender) {
        case Gender::Male:
            return "Male";
        case Gender::Female:
            return "Female";
        case Gender::Other:
            return "Other";
    }
    return "Unknown";
}

static App *app;
static SceneType current_scene = SceneType::Scene9;

static void go_back(int dummy) {
    app->scenes.pop();
}

static void go_to_scene10(int dummy) {
    app->scenes.push(SceneType::Scene10);
}

static void go_to_scene11(int dummy) {
    app->scenes.push(SceneType::Scene11);
}

void scene9(sf::RenderWindow &window, App &_app) {
    app = &_app;
    auto student = app->user_iter.ptr<Student>();
    Textbox name("Name: " + student->name.last + " " + student->name.first, 18, sf::Color::Black,
                 sf::Vector2f(30, 50), sf::Vector2f(100, 50), sf::Color::White);
    name.align_left();


    std::string class_name = student->classroom.ptr<Class>()->name;
    Textbox classroom("Class: " + class_name, 18, sf::Color::Black,
                      sf::Vector2f(30, 100), sf::Vector2f(100, 50), sf::Color::White);
    classroom.align_left();


    Textbox student_id("Student id: " + student->student_id, 18, sf::Color::Black,
                       sf::Vector2f(30, 150), sf::Vector2f(100, 50), sf::Color::White);
    student_id.align_left();


    Textbox social_id("Social id: " + student->social_id, 18, sf::Color::Black,
                      sf::Vector2f(30, 200), sf::Vector2f(100, 50), sf::Color::White);
    social_id.align_left();


    Textbox gender("Gender: " + to_string(student->gender), 18, sf::Color::Black,
                   sf::Vector2f(30, 250), sf::Vector2f(100, 50), sf::Color::White);
    gender.align_left();


    Textbox birth("Birth: " + Utils::dateToStr(student->birth), 18, sf::Color::Black,
                  sf::Vector2f(30, 300), sf::Vector2f(100, 50), sf::Color::White);
    birth.align_left();


    Textbox scoreboard_box("Scoreboard", 24, sf::Color::Black,
                           sf::Vector2f(100, 500), sf::Vector2f(200, 100), sf::Color::Cyan);
    Button_Textbox scoreboard_button(scoreboard_box, sf::Color::Yellow);
    scoreboard_button.set_idle_outline(sf::Color::Blue);


    Textbox course_box("Course", 24, sf::Color::Black,
                       sf::Vector2f(500, 500), sf::Vector2f(200, 100), sf::Color::Cyan);
    Button_Textbox course_button(course_box, sf::Color::Yellow);
    course_button.set_idle_outline(sf::Color::Blue);


    sf::Texture texture;
    texture.loadFromFile(PATH::IMAGES + "go_back.png");
    Button_Sprite back_button(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));
    Interaction interaction;
    interaction.add_button(scoreboard_button, go_to_scene10);
    interaction.add_button(course_button, go_to_scene11);
    interaction.add_button(back_button, go_back);


    while (window.isOpen() && !app->scenes.empty() && app->scenes.top() == current_scene && !app->scenes.refresh) {
        window.clear(sf::Color::White);
        name.draw(window, app->default_font);
        classroom.draw(window, app->default_font);
        student_id.draw(window, app->default_font);
        social_id.draw(window, app->default_font);
        gender.draw(window, app->default_font);
        birth.draw(window, app->default_font);
        interaction.draw(window, app->default_font);
        window.display();

        auto event = interaction.interact(window);
        app->scenes.interact(event);
    }
}
