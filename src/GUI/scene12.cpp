#include "Scene12.h"

static Input_Textbox *PasswordInputBox;
static Input_Textbox *ConfirmPasswordInputBox;
static App *app;
static bool password_change = false;

static void go_back(int dummy) {
	app->scenes.pop();
}

static void ChangePassword(int dummy) {
	if (PasswordInputBox->text == ConfirmPasswordInputBox->text) {
		app->user_iter.ptr<Student>()->password = PasswordInputBox->text;
		password_change = true;
	}
}

void scene12(sf::RenderWindow &window, App &_app) {
	app = &_app;
	Textbox mainBackground("", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4, windowHeight / 3),
		sf::Vector2f(windowWidth / 2, windowHeight / 3), sf::Color::Black);
	mainBackground.set_outline(sf::Color::Blue, 4);

	Textbox PasswordText("Password:", defaultMediumCharSize, sf::Color::White,
		sf::Vector2f(windowWidth / 4 + 20, windowHeight / 3 + 30), sf::Vector2f(130, 50),
		sf::Color::Blue);
	Textbox PasswordBox("", defaultMediumCharSize, sf::Color::Black,
		sf::Vector2f(windowWidth / 4 + 170, windowHeight / 3 + 30), sf::Vector2f(410, 50),
		sf::Color::White);
	Input_Textbox _PasswordInputBox(PasswordBox, 20, sf::Color::Blue);
	_PasswordInputBox.set_hide_text();
	PasswordInputBox = &_PasswordInputBox;

	Textbox ConfirmPasswordText("Confirm password:", defaultMediumCharSize, sf::Color::White,
		sf::Vector2f(windowWidth / 4 + 20, windowHeight / 3 + 100), sf::Vector2f(130, 50),
		sf::Color::Blue);
	Textbox ConfirmPasswordBox("", defaultMediumCharSize, sf::Color::Black,
		sf::Vector2f(windowWidth / 4 + 170, windowHeight / 3 + 100), sf::Vector2f(410, 50),
		sf::Color::White);
	Input_Textbox _ConfirmPasswordInputBox(ConfirmPasswordBox, 20, sf::Color::Blue);
	_ConfirmPasswordInputBox.set_hide_text();
	ConfirmPasswordInputBox = &_ConfirmPasswordInputBox;


	Textbox ChangeBox("Change", defaultMediumCharSize, sf::Color::White,
		sf::Vector2f(windowWidth / 2 - 65, windowHeight / 3 + 220), sf::Vector2f(130, 50),
		sf::Color::Blue);
	Button_Textbox ChangeButton(ChangeBox, sf::Color::White);

	sf::Texture texture;
	texture.loadFromFile(PATH::IMAGES + "go_back.png");
	Button_Sprite back_button(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));

	Textbox success_change("Success change password", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 4, windowHeight / 3),
		sf::Vector2f(400, 50), sf::Color::White);
	success_change.set_outline(sf::Color::Black);

	password_change = false;

	while (window.isOpen() && !app->scenes.empty() && app->scenes.top() == SceneType::Scene12) {
		if (!password_change) {
			Interaction interaction;

			interaction.add_input_textbox(_PasswordInputBox);
			interaction.add_input_textbox(_ConfirmPasswordInputBox);

			interaction.add_button(ChangeButton, ChangePassword);
			interaction.add_button(back_button, go_back);

			while (window.isOpen() && !app->scenes.empty() && app->scenes.top() == SceneType::Scene12 && !password_change) {
				window.clear(sf::Color::White);

				mainBackground.draw(window, app->default_font);
				PasswordText.draw(window, app->default_font);
				ConfirmPasswordText.draw(window, app->default_font);

				interaction.draw(window, app->default_font);
				window.display();

				auto event = interaction.interact(window);
				app->scenes.interact(event);
			}
		}
		else {
			Interaction interaction;

			interaction.add_button(back_button, go_back);

			while (window.isOpen() && !app->scenes.empty() && app->scenes.top() == SceneType::Scene12) {
				window.clear(sf::Color::White);
				success_change.draw(window, app->default_font);

				interaction.draw(window, app->default_font);
				window.display();

				auto event = interaction.interact(window);
				app->scenes.interact(event);
			}
		}
	}
}
