#include<iostream>
#include "loginScene.h"
using namespace std;
using namespace GUI;

void closeWindow(int dummy)
{
	exit(0);
}

void login_scene(sf::RenderWindow& window)
{
	sf::Font font;
	font.loadFromFile("Fonts/arial.ttf");

	Textbox mainBackground("", font, defaultCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4, windowHeight / 3), sf::Vector2f(windowWidth / 2, windowHeight / 3), sf::Color::Black);
	mainBackground.set_outline(sf::Color::Blue, 4);

	Textbox usernameText("Username:", font, defaultCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4 + 20, windowHeight / 3 + 30), sf::Vector2f(130, 50), sf::Color::Blue);
	Textbox usernameBox("", font, defaultCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 4 + 170, windowHeight / 3 + 30), sf::Vector2f(410, 50), sf::Color::White);
	Input_Textbox usernameInputBox(usernameBox, 20, sf::Color::Blue);

	Textbox passwordText("Password:", font, defaultCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4 + 20, windowHeight / 3 + 100), sf::Vector2f(130, 50), sf::Color::Blue);
	Textbox passwordBox("", font, defaultCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 4 + 170, windowHeight / 3 + 100), sf::Vector2f(410, 50), sf::Color::White);
	Input_Textbox passwordInputBox(passwordBox, 20, sf::Color::Blue);
	passwordInputBox.set_hide_text();

	Textbox loginBox("ENTER", font, defaultCharSize, sf::Color::White, sf::Vector2f(windowWidth / 2 - 65, windowHeight / 3 + 220), sf::Vector2f(130, 50), sf::Color::Blue);
	Button_Textbox loginButton(loginBox, sf::Color::White);

	Interaction interaction;

	interaction.add_input_textbox(usernameInputBox);
	interaction.add_input_textbox(passwordInputBox);


	interaction.add_button(loginButton, closeWindow);

	while (window.isOpen())
	{
		interaction.interact(window);

		window.clear(sf::Color::Green);

		mainBackground.draw(window);

		usernameText.draw(window);
		passwordText.draw(window);

		interaction.draw(window);

		window.display();
	}
}