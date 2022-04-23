#include "scene10.h"

static App* app;
static int num_page, page_cnt;

static void go_back(int dummy) {
	app->scenes.pop();
}

static void previous(int dummy) {
	num_page = std::max(num_page - 1, 0);
}

static void next(int dummy) {
	num_page = std::min(num_page + 1, page_cnt - 1);
}

void scene10(sf::RenderWindow& window, App &_app) {
	app = &_app;
	auto student = app->user_iter.ptr<Student>();

	Textbox course_id = Textbox("Course ID", 16, sf::Color::Black, sf::Vector2f(80, 100), sf::Vector2f(150, 50), sf::Color::White);
	course_id.set_outline(sf::Color::Black);

	Textbox course_name = Textbox("Course Name", 16, sf::Color::Black, sf::Vector2f(230, 100), sf::Vector2f(400, 50), sf::Color::White);
	course_name.set_outline(sf::Color::Black);

	Textbox credits = Textbox("Credits", 16, sf::Color::Black, sf::Vector2f(630, 100), sf::Vector2f(100, 50), sf::Color::White);
	credits.set_outline(sf::Color::Black);

	Textbox midterm = Textbox("Midterm", 16, sf::Color::Black, sf::Vector2f(730, 100), sf::Vector2f(100, 50), sf::Color::White);
	midterm.set_outline(sf::Color::Black);

	Textbox other = Textbox("Other", 16, sf::Color::Black, sf::Vector2f(830, 100), sf::Vector2f(100, 50), sf::Color::White);
	other.set_outline(sf::Color::Black);

	Textbox final = Textbox("Final", 16, sf::Color::Black, sf::Vector2f(930, 100), sf::Vector2f(100, 50), sf::Color::White);
	final.set_outline(sf::Color::Black);

	Textbox total = Textbox("Total", 16, sf::Color::Black, sf::Vector2f(1030, 100), sf::Vector2f(100, 50), sf::Color::White);
	total.set_outline(sf::Color::Black);

	List<Textbox> headers;
	headers.push_back(course_id);
	headers.push_back(course_name);
	headers.push_back(credits);
	headers.push_back(midterm);
	headers.push_back(other);
	headers.push_back(final);
	headers.push_back(total);

	auto scores = student->scores;
	int n = scores.size();
	int page_size = 8;
	int num_row = 0;
	List<Textbox> page;
	List<List<Textbox>> pages;
	scores.for_each([&](const auto& score) {
		++num_row;
		if (num_row > page_size) {
			num_row -= page_size;
			pages.push_back(page);
			page.clear();
		}

		auto course = score->course.ptr<Course>();

		Textbox id = Textbox(course->id, 16, sf::Color::Black, sf::Vector2f(80, 100 + num_row * 50),
			sf::Vector2f(150, 50), sf::Color::White);
		id.set_outline(sf::Color::Black);
		id.align_left();


		Textbox name = Textbox(course->name, 16, sf::Color::Black, sf::Vector2f(230, 100 + num_row * 50),
			sf::Vector2f(400, 50), sf::Color::White);
		name.set_outline(sf::Color::Black);
		name.align_left();


		Textbox credits_cnt = Textbox(std::to_string(course->credits), 16, sf::Color::Black, sf::Vector2f(630, 100 + num_row * 50),
			sf::Vector2f(100, 50), sf::Color::White);
		credits_cnt.set_outline(sf::Color::Black);


		Textbox midterm_score = Textbox(std::to_string(score->midterm), 16, sf::Color::Black, sf::Vector2f(730, 100 + num_row * 50),
			sf::Vector2f(100, 50), sf::Color::White);
		midterm_score.set_outline(sf::Color::Black);


		Textbox other_score = Textbox(std::to_string(score->other), 16, sf::Color::Black, sf::Vector2f(830, 100 + num_row * 50),
			sf::Vector2f(100, 50), sf::Color::White);
		other_score.set_outline(sf::Color::Black);


		Textbox final_score = Textbox(std::to_string(score->final), 16, sf::Color::Black, sf::Vector2f(930, 100 + num_row * 50),
			sf::Vector2f(100, 50), sf::Color::White);
		final_score.set_outline(sf::Color::Black);


		Textbox total_score = Textbox(std::to_string(score->total), 16, sf::Color::Black, sf::Vector2f(1030, 100 + num_row * 50),
			sf::Vector2f(100, 50), sf::Color::White);
		total_score.set_outline(sf::Color::Black);


		page.push_back(id);
		page.push_back(name);
		page.push_back(credits_cnt);
		page.push_back(midterm_score);
		page.push_back(other_score);
		page.push_back(final_score);
		page.push_back(total_score);
	});
	pages.push_back(page);
	num_page = 0;
	page_cnt = (n - 1) / page_size + 1;

	sf::Texture texture;
	texture.loadFromFile("assets/images/GoBack.png");
	Button_Sprite back_button = Button_Sprite(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));

	texture.loadFromFile("assets/images/previous.jpg");
	Button_Sprite previous_button = Button_Sprite(texture, sf::Vector2f(950, 60), sf::Vector2f(40, 40));

	texture.loadFromFile("assets/images/next.jpg");
	Button_Sprite next_button = Button_Sprite(texture, sf::Vector2f(990, 60), sf::Vector2f(40, 40));

	Interaction interaction;
	interaction.add_button(back_button, go_back);
	interaction.add_button(previous_button, previous);
	interaction.add_button(next_button, next);

	while (window.isOpen() && app->scenes.top() == 10) {
		window.clear(sf::Color::White);

		headers.for_each([&](Textbox& textbox) {
			textbox.draw(window);
		});

		if (!pages.empty()) {
			pages[num_page].for_each([&](Textbox& textbox) {
				textbox.draw(window);
			});
		}

		interaction.draw(window);
		interaction.interact(window);
		window.display();
	}
}