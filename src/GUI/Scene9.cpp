#include "Scene9.h"
#include "../App/Utils.h"

std::string to_string(const Gender& gender) {
	switch (gender) {
	case Gender::Male: return "Male";
	case Gender::Female: return "Female";
	case Gender::Other: return "Other";
	}
	return "Unknown";
}

App* app;

void go_back(int dummy) {
	app->scenes.pop();
}

void go_to_scene10(int dummy) {
	app->scenes.push(10);
}

void go_to_scene11(int dummy) {
	app->scenes.push(11);
}

void scene9(sf::RenderWindow& window, App &_app) {
	app = &_app;
	auto student = app->user_iter.ptr<Student>();
	Textbox name = Textbox("Name: " + student->name.last + " " + student->name.first, 18, sf::Color::Black,
		sf::Vector2f(30, 50), sf::Vector2f(100, 50), sf::Color::White);
	name.align_left();


	std::string class_name = student->classroom.ptr<Class>()->name;
	Textbox classroom = Textbox("Class: " + class_name, 18, sf::Color::Black,
		sf::Vector2f(30, 100), sf::Vector2f(100, 50), sf::Color::White);
	classroom.align_left();


	Textbox student_id = Textbox("Student id: " + student->student_id, 18, sf::Color::Black,
		sf::Vector2f(30, 150), sf::Vector2f(100, 50), sf::Color::White);
	student_id.align_left();


	Textbox social_id = Textbox("Social id: " + student->social_id, 18, sf::Color::Black,
		sf::Vector2f(30, 200), sf::Vector2f(100, 50), sf::Color::White);
	social_id.align_left();


	Textbox gender = Textbox("Gender: " + to_string(student->gender), 18, sf::Color::Black,
		sf::Vector2f(30, 250), sf::Vector2f(100, 50), sf::Color::White);
	gender.align_left();


	Textbox birth = Textbox("Birth: " + Utils::dateToStr(student->birth), 18, sf::Color::Black,
		sf::Vector2f(30, 300), sf::Vector2f(100, 50), sf::Color::White);
	birth.align_left();


	Textbox scoreboard_box = Textbox("Scoreboard", 24, sf::Color::Black,
		sf::Vector2f(100, 500), sf::Vector2f(200, 100), sf::Color::Cyan);
	Button_Textbox scoreboard_button = Button_Textbox(scoreboard_box, sf::Color::Yellow);
	scoreboard_button.set_idle_outline(sf::Color::Blue);


	Textbox course_box = Textbox("Course", 24, sf::Color::Black,
		sf::Vector2f(500, 500), sf::Vector2f(200, 100), sf::Color::Cyan);
	Button_Textbox course_button = Button_Textbox(course_box, sf::Color::Yellow);
	course_button.set_idle_outline(sf::Color::Blue);


	sf::Texture texture;
	texture.loadFromFile("assets/images/GoBack.png");
	Button_Sprite back_button = Button_Sprite(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));
	Interaction interaction;
	interaction.add_button(scoreboard_button, go_to_scene10);
	interaction.add_button(course_button, go_to_scene11);
	interaction.add_button(back_button, go_back);


	while (window.isOpen() && app->scenes.top() == 9) {
		window.clear(sf::Color::White);
		name.draw(window);
		classroom.draw(window);
		student_id.draw(window);
		social_id.draw(window);
		gender.draw(window);
		birth.draw(window);
		interaction.draw(window);
		window.display();
		interaction.interact(window);
	}
}
