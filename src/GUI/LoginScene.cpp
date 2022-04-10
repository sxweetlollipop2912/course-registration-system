#include <iostream>
#include "LoginScene.h"
#include "../Constants.h"
using namespace std;

void login_scene(sf::RenderWindow& app)
{
    sf::Font font;
    font.loadFromFile(PATH::DEFAULT_FONT);
    
    Textbox mainBackground("", font, GUI::DEFAULT_CHAR_SIZE, sf::Color::White, sf::Vector2f(GUI::WINDOW_WIDTH / 4, GUI::WINDOW_HEIGHT / 3), sf::Vector2f(GUI::WINDOW_WIDTH / 2, GUI::WINDOW_HEIGHT / 3), sf::Color::Black);
    mainBackground.set_outline(sf::Color::Blue, 4);

    Textbox usernameText("Username:", font, GUI::DEFAULT_CHAR_SIZE, sf::Color::White, sf::Vector2f(GUI::WINDOW_WIDTH / 4 + 20, GUI::WINDOW_HEIGHT / 3 + 30), sf::Vector2f(130, 50), sf::Color::Blue, sf::Vector2f(5, 10));
    Textbox usernameShow("", font, GUI::DEFAULT_CHAR_SIZE, sf::Color::Black, sf::Vector2f(GUI::WINDOW_WIDTH / 4 + 170, GUI::WINDOW_HEIGHT / 3 + 30), sf::Vector2f(410, 50), sf::Color::White, sf::Vector2f(10, 10));
    
    Textbox passwordText("Password:", font, GUI::DEFAULT_CHAR_SIZE, sf::Color::White, sf::Vector2f(GUI::WINDOW_WIDTH / 4 + 20, GUI::WINDOW_HEIGHT / 3 + 100), sf::Vector2f(130, 50), sf::Color::Blue, sf::Vector2f(5, 10));
    Textbox passwordShow("", font, GUI::DEFAULT_CHAR_SIZE, sf::Color::Black, sf::Vector2f(GUI::WINDOW_WIDTH / 4 + 170, GUI::WINDOW_HEIGHT / 3 + 100), sf::Vector2f(410, 50), sf::Color::White, sf::Vector2f(10, 10));

    Textbox loginBox("ENTER", font, GUI::DEFAULT_CHAR_SIZE, sf::Color::White, sf::Vector2f(GUI::WINDOW_WIDTH / 4 + 110, GUI::WINDOW_HEIGHT / 3 + 220), sf::Vector2f(130, 50), sf::Color::Blue, sf::Vector2f(24, 10));
    Textbox loginBoxHover("ENTER", font, GUI::DEFAULT_CHAR_SIZE, sf::Color::Black, sf::Vector2f(GUI::WINDOW_WIDTH / 4 + 110, GUI::WINDOW_HEIGHT / 3 + 220), sf::Vector2f(130, 50), sf::Color::White, sf::Vector2f(24, 10));
    Button loginButton(loginBox, loginBoxHover);
    
    Textbox exitBox("EXIT", font, GUI::DEFAULT_CHAR_SIZE, sf::Color::White, sf::Vector2f(GUI::WINDOW_WIDTH / 4 + 360, GUI::WINDOW_HEIGHT / 3 + 220), sf::Vector2f(130, 50), sf::Color::Blue, sf::Vector2f(40, 10));
    Textbox exitBoxHover("EXIT", font, GUI::DEFAULT_CHAR_SIZE, sf::Color::Black, sf::Vector2f(GUI::WINDOW_WIDTH / 4 + 360, GUI::WINDOW_HEIGHT / 3 + 220), sf::Vector2f(130, 50), sf::Color::White, sf::Vector2f(40, 10));
    Button exitButton(exitBox, exitBoxHover);

    Interaction interaction;
    interaction.add_button(loginButton);
    interaction.add_button(exitButton);

    string username;
    string password;
    string hidePassword;

    int blankPos = 0;
    usernameShow.set_outline(sf::Color::Blue, 4);
    passwordShow.set_outline(sf::Color::Transparent, 0);

    while (app.isOpen())
	{
        sf::Event event{};

        sf::Vector2i mousePos = sf::Mouse::getPosition(app);
        while (app.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                app.close();
            }
            // keyboard event
            if (event.type == sf::Event::KeyPressed)
            {
                if (blankPos == 0)
                {
                    if (event.key.code < 0)
                    {

                    }
                    else if (event.key.code < 26)
                    {
                        if (username.size() < 20) username.push_back(char(event.key.code + 97));
                    }
                    else if (event.key.code < 36)
                    {
                        if (username.size() < 20) username.push_back(char(event.key.code + 22));
                    }
                    else if (event.key.code == 59)
                    {
                        if (!username.empty()) username.pop_back();
                    }
                    else if (event.key.code == 60)
                    {
                        blankPos = 1;
                        usernameShow.set_outline(sf::Color::Transparent, 0);
                        passwordShow.set_outline(sf::Color::Blue, 4);
                    }
                    usernameShow.set_text(username);
                }
                else
                {
                    if (event.key.code < 0)
                    {

                    }
                    else if (event.key.code < 26)
                    {
                        if (password.size() < 20) password.push_back(char(event.key.code + 97)), hidePassword.push_back('*');
                    }
                    else if (event.key.code < 36)
                    {
                        if (password.size() < 20) password.push_back(char(event.key.code + 22)), hidePassword.push_back('*');
                    }
                    else if (event.key.code == 59)
                    {
                        if (!password.empty()) password.pop_back(), hidePassword.pop_back();
                    }
                    else if (event.key.code == 60)
                    {
                        blankPos = 0;
                        usernameShow.set_outline(sf::Color::Blue, 4);
                        passwordShow.set_outline(sf::Color::Transparent, 0);
                    }
                    passwordShow.set_text(hidePassword);
                }
            }
            // press mouse
            if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (usernameShow.inside(mousePos.x, mousePos.y))
                {
                    blankPos = 0;
                    usernameShow.set_outline(sf::Color::Blue, 4);
                    passwordShow.set_outline(sf::Color::Transparent, 0);
                }
                if (passwordShow.inside(mousePos.x, mousePos.y))
                {
                    blankPos = 1;
                    usernameShow.set_outline(sf::Color::Transparent, 0);
                    passwordShow.set_outline(sf::Color::Blue, 4);
                }
            }
        }
        
        app.clear(sf::Color::Green);

        mainBackground.draw(app);

        usernameText.draw(app);
        usernameShow.draw(app);
        
        passwordText.draw(app);
        passwordShow.draw(app);

        interaction.interact(event);
        interaction.draw(app);

        app.display();
	}
}