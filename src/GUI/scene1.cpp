#include "scene1.h"

static Input_Textbox* startInputBoxP;
static Input_Textbox* endInputBoxP;
static App* app;
static sf::RenderWindow* windowP;
static Button_Textbox* currentYearButtonP;
static Interaction* interactionP;

static string to_string(int x)
{
	string res = "";
	while (x > 0)
	{
		res.push_back(char(x % 10 + 48));
		x /= 10;
	}
	reverse(res.begin(), res.end());
	return res;
}

static int to_int(string s)
{
	int sum = 0;
	for (int i = 0; i < s.size(); i++)
	{
		sum = sum * 10 + int(s[i]) - 48;
	}
	return sum;
}

static void go_back(int dummy) {
	app->scenes.pop();
}

static void go_to_scene2(int dummy)
{
	app->scenes.push(2);
}

static void create_new_year_function(int dummy)
{
	int start_year = to_int(startInputBoxP->text);
	int end_year = to_int(endInputBoxP->text);
	auto default_year = make_shared<SchoolYear>(start_year, end_year);
	app->addDefaultSchoolYear(default_year);
}

static void create_new_year(int dummy)
{
	Textbox mainBackground("", defaultCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4, windowHeight / 3), sf::Vector2f(windowWidth / 2, windowHeight / 3), sf::Color::Black);
	mainBackground.set_outline(sf::Color::Blue, 4);

	Textbox startText("Start:", defaultCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4 + 20, windowHeight / 3 + 30), sf::Vector2f(130, 50), sf::Color::Transparent);
	Textbox startBox("", defaultCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 4 + 170, windowHeight / 3 + 30), sf::Vector2f(410, 50), sf::Color::White);
	Input_Textbox startInputBox(startBox, 20, sf::Color::Blue);
	startInputBoxP = &startInputBox;

	Textbox endText("End:", defaultCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4 + 20, windowHeight / 3 + 100), sf::Vector2f(130, 50), sf::Color::Transparent);
	Textbox endBox("", defaultCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 4 + 170, windowHeight / 3 + 100), sf::Vector2f(410, 50), sf::Color::White);
	Input_Textbox endInputBox(endBox, 20, sf::Color::Blue);
	endInputBoxP = &endInputBox;


	Textbox enterBox("ENTER", defaultCharSize, sf::Color::White, sf::Vector2f(windowWidth / 2 - 65, windowHeight / 3 + 220), sf::Vector2f(130, 50), sf::Color::Blue);
	Button_Textbox enterButton(enterBox, sf::Color::White);

	Interaction interaction;

	interaction.add_input_textbox(startInputBox);
	interaction.add_input_textbox(endInputBox);


	interaction.add_button(enterButton, create_new_year_function);
	while (windowP->isOpen() && app->year() == NULL)
	{
		windowP->clear(sf::Color::White);
		mainBackground.draw(*windowP);
		interaction.draw(*windowP);
		startText.draw(*windowP);
		endText.draw(*windowP);
		windowP->display();
		interaction.interact(*windowP);
	}
	currentYearButtonP->textbox.set_text(to_string(app->year()->start_year) + "-" + to_string(app->year()->end_year));
	interactionP->add_button(*currentYearButtonP, go_to_scene2);

}

void scene1(sf::RenderWindow& window, App& _app)
{
	windowP = &window;
	app = &_app;
	Interaction interaction, interaction2;
	Textbox currentYearText("Current year", defaultCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 2 - 100, windowHeight / 2 - 100), sf::Vector2f(200, 50), sf::Color::Transparent);
	Textbox currentYear("", defaultCharSize, sf::Color::White, sf::Vector2f(windowWidth / 2 - 100, windowHeight / 2 - 25), sf::Vector2f(200, 50), sf::Color::Black);
	Button_Textbox currentYearButton(currentYear, sf::Color::Blue);
	currentYearButtonP = &currentYearButton;

	Textbox noCurrentYearText("No current year!", defaultCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 2 - 100, windowHeight / 2 - 100), sf::Vector2f(200, 50), sf::Color::Transparent);
	Textbox createYearBox("Create new year", defaultCharSize, sf::Color::White, sf::Vector2f(windowWidth / 2 - 100, windowHeight / 2 - 25), sf::Vector2f(200, 50), sf::Color::Black);
	Button_Textbox createYearButton(createYearBox, sf::Color::Blue);
	interaction2.add_button(createYearButton, create_new_year);


	sf::Texture texture;
	texture.loadFromFile("assets/images/GoBack.png");
	Button_Sprite back_button = Button_Sprite(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));
	interaction.add_button(back_button, go_back);
	interactionP = &interaction;

	if (app->year() != NULL)
	{
		currentYearButton.textbox.set_text(to_string(app->year()->start_year) + "-" + to_string(app->year()->end_year));
		interaction.add_button(currentYearButton, go_to_scene2);
	}
	
	while (window.isOpen() && app->scenes.top() == 1)
	{
		if (app->year() != NULL)
		{
			window.clear(sf::Color::White);
			currentYearText.draw(window);
			interaction.draw(window);
			window.display();
			interaction.interact(window);
		}
		else
		{
			window.clear(sf::Color::White);
			noCurrentYearText.draw(window);
			interaction2.draw(window);
			window.display();
			interaction2.interact(window);
		}
		
	}
	

}