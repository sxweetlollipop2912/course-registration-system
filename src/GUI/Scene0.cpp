#include "Scene0.h"

static Input_Textbox *usernameInputBoxP;
static Input_Textbox *passwordInputBoxP;
static App *appGlobal;
static SceneType current_scene = SceneType::Scene0;

static void checkAccount(int dummy) {
    string username = usernameInputBoxP->text;
    string password = passwordInputBoxP->text;
    appGlobal->login(username, password);
}

void scene0(sf::RenderWindow &window, App &app) {
    if (app.isLoggedIn()) app.logout();
    appGlobal = &app;
    Textbox mainBackground("", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4, windowHeight / 3),
                           sf::Vector2f(windowWidth / 2, windowHeight / 3), sf::Color::Black);
    mainBackground.set_outline(sf::Color::Blue, 4);

    Textbox usernameText("Username:", defaultMediumCharSize, sf::Color::White,
                         sf::Vector2f(windowWidth / 4 + 20, windowHeight / 3 + 30), sf::Vector2f(130, 50),
                         sf::Color::Blue);
    Textbox usernameBox("", defaultMediumCharSize, sf::Color::Black,
                        sf::Vector2f(windowWidth / 4 + 170, windowHeight / 3 + 30), sf::Vector2f(410, 50),
                        sf::Color::White);
    Input_Textbox usernameInputBox(usernameBox, 20, sf::Color::Blue);
    usernameInputBoxP = &usernameInputBox;

    Textbox passwordText("Password:", defaultMediumCharSize, sf::Color::White,
                         sf::Vector2f(windowWidth / 4 + 20, windowHeight / 3 + 100), sf::Vector2f(130, 50),
                         sf::Color::Blue);
    Textbox passwordBox("", defaultMediumCharSize, sf::Color::Black,
                        sf::Vector2f(windowWidth / 4 + 170, windowHeight / 3 + 100), sf::Vector2f(410, 50),
                        sf::Color::White);
    Input_Textbox passwordInputBox(passwordBox, 20, sf::Color::Blue);
    passwordInputBox.set_hide_text();
    passwordInputBoxP = &passwordInputBox;


    Textbox loginBox("ENTER", defaultMediumCharSize, sf::Color::White,
                     sf::Vector2f(windowWidth / 2 - 65, windowHeight / 3 + 220), sf::Vector2f(130, 50),
                     sf::Color::Blue);
    Button_Textbox loginButton(loginBox, sf::Color::White);

    Interaction interaction;

    interaction.add_input_textbox(usernameInputBox);
    interaction.add_input_textbox(passwordInputBox);


    interaction.add_button(loginButton, checkAccount);

    while (window.isOpen() && !appGlobal->scenes.empty() && appGlobal->scenes.top() == current_scene &&
           !appGlobal->scenes.refresh) {
        auto event = interaction.interact(window);
        appGlobal->scenes.interact(event);

        if (app.isLoggedIn()) {
            if (app.userType() == UserType::Student) {
                app.scenes.push(SceneType::Scene9);
            } else {
                app.scenes.push(SceneType::Scene1);
            }
            return;
        }
        window.clear(sf::Color::Green);

        mainBackground.draw(window, app.default_font);

        usernameText.draw(window, app.default_font);
        passwordText.draw(window, app.default_font);

        interaction.draw(window, app.default_font);

        window.display();
    }
}