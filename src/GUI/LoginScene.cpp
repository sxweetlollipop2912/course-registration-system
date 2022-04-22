#include "LoginScene.h"

Input_Textbox* usernameInputBoxP;
Input_Textbox* passwordInputBoxP;
App* appGlobal;

void checkAccount(int dummy)
{
	string username = usernameInputBoxP->text;
	string password = passwordInputBoxP->text;
	appGlobal->login(username, password);
}

void login_scene(sf::RenderWindow& window, App &app)
{
	appGlobal = &app;
	Textbox mainBackground("", defaultCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4, windowHeight / 3), sf::Vector2f(windowWidth / 2, windowHeight / 3), sf::Color::Black);
	mainBackground.set_outline(sf::Color::Blue, 4);

	Textbox usernameText("Username:", defaultCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4 + 20, windowHeight / 3 + 30), sf::Vector2f(130, 50), sf::Color::Blue);
	Textbox usernameBox("", defaultCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 4 + 170, windowHeight / 3 + 30), sf::Vector2f(410, 50), sf::Color::White);
	Input_Textbox usernameInputBox(usernameBox, 20, sf::Color::Blue);
	usernameInputBoxP = &usernameInputBox;

	Textbox passwordText("Password:", defaultCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4 + 20, windowHeight / 3 + 100), sf::Vector2f(130, 50), sf::Color::Blue);
	Textbox passwordBox("", defaultCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 4 + 170, windowHeight / 3 + 100), sf::Vector2f(410, 50), sf::Color::White);
	Input_Textbox passwordInputBox(passwordBox, 20, sf::Color::Blue);
	passwordInputBox.set_hide_text();
	passwordInputBoxP = &passwordInputBox;


	Textbox loginBox("ENTER", defaultCharSize, sf::Color::White, sf::Vector2f(windowWidth / 2 - 65, windowHeight / 3 + 220), sf::Vector2f(130, 50), sf::Color::Blue);
	Button_Textbox loginButton(loginBox, sf::Color::White);

	Interaction interaction;

	interaction.add_input_textbox(usernameInputBox);
	interaction.add_input_textbox(passwordInputBox);


	interaction.add_button(loginButton, checkAccount);

	while (window.isOpen())
	{
		interaction.interact(window);
		if (app.isLoggedIn())
		{
			if (app.userType() == UserType::Student)
			{
				app.scenes.push(9);
			}
			else
			{
				app.scenes.push(1);
			}
			return;
		}
		window.clear(sf::Color::Green);

		mainBackground.draw(window);

		usernameText.draw(window);
		passwordText.draw(window);

		interaction.draw(window);

		window.display();
	}
}