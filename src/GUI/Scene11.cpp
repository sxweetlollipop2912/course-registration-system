#include "Scene11.h"

static App *app;
static int num_page, page_cnt, page_size;
static bool data_change;

static void go_back(int dummy) {
	app->scenes.pop();
}

static void previous(int dummy) {
	num_page = std::max(num_page - 1, 0);
	data_change = true;
}

static void next(int dummy) {
	num_page = std::min(num_page + 1, page_cnt - 1);
	data_change = true;
}

static void toggle_enroll_course(int i) {
	int offset = num_page * page_size;
	auto student = app->user_iter;
	auto course = app->semester()->courses[offset + i].ptr<Course>();
	if (course->getStudent(student.ptr<Student>()->student_id).empty())
		data_change = app->enroll(student, course->id);
	else
		data_change = app->disenroll(student, course->id);
}

void scene11(sf::RenderWindow &window, App &_app) {
	app = &_app;
	auto student = app->user_iter.ptr<Student>();
	auto semester = app->semester();

	if (!semester) {
		go_back(0);
		return;
	}

	semester->sortCourse();

	bool enrollable = semester->reg_session.isOngoing();
	auto courses = student->getCoursesInSemester(app->semester()->uid);
	if (enrollable)
		courses = semester->courses;

	Textbox course_id_header("Course ID", 16, sf::Color::Black, sf::Vector2f(0, 100), sf::Vector2f(100, 50),
		sf::Color::White);
	course_id_header.set_outline(sf::Color::Black);

	Textbox course_name_header("Course Name", 16, sf::Color::Black, sf::Vector2f(100, 100), sf::Vector2f(300, 50),
		sf::Color::White);
	course_name_header.set_outline(sf::Color::Black);

	Textbox credits_header("Credits", 16, sf::Color::Black, sf::Vector2f(400, 100), sf::Vector2f(100, 50),
		sf::Color::White);
	credits_header.set_outline(sf::Color::Black);

	Textbox sessions_header("Sessions", 16, sf::Color::Black, sf::Vector2f(500, 100), sf::Vector2f(200, 50),
		sf::Color::White);
	sessions_header.set_outline(sf::Color::Black);

	Textbox teacher_header("Teacher", 16, sf::Color::Black, sf::Vector2f(700, 100), sf::Vector2f(250, 50),
		sf::Color::White);
	teacher_header.set_outline(sf::Color::Black);

	Textbox slot_header("Slot", 16, sf::Color::Black, sf::Vector2f(950, 100), sf::Vector2f(100, 50), sf::Color::White);
	slot_header.set_outline(sf::Color::Black);

	Textbox status_header("Status", 16, sf::Color::Black, sf::Vector2f(1050, 100), sf::Vector2f(150, 50),
		sf::Color::White);
	status_header.set_outline(sf::Color::Black);

	List<Textbox> headers;
	headers.push_back(course_id_header);
	headers.push_back(course_name_header);
	headers.push_back(credits_header);
	headers.push_back(sessions_header);
	headers.push_back(teacher_header);
	headers.push_back(slot_header);
	if (enrollable)
		headers.push_back(status_header);

	int n = courses.size();
	page_size = 5;
	int num_row = 0;
	List<DataIter> page;
	List<List<DataIter>> pages;
	courses.for_each([&](const auto &Iter) {
		++num_row;
		if (num_row > page_size) {
			num_row -= page_size;
			pages.push_back(page);
			page.clear();
		}
		page.push_back(Iter);
	});
	pages.push_back(page);
	num_page = 0;
	page_cnt = (n - 1) / page_size + 1;

	sf::Texture texture;
	texture.loadFromFile("assets/images/go_back.png");
	Button_Sprite back_button(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));

	texture.loadFromFile("assets/images/previous.jpg");
	Button_Sprite previous_button(texture, sf::Vector2f(910, 60), sf::Vector2f(40, 40));

	texture.loadFromFile("assets/images/next.jpg");
	Button_Sprite next_button(texture, sf::Vector2f(950, 60), sf::Vector2f(40, 40));

	while (window.isOpen() && !app->scenes.empty() && app->scenes.top() == SceneType::Scene11) {
		List<Textbox> textboxs;
		std::string page_num_string;
		if (n > 0)
			page_num_string = std::to_string(num_page + 1) + "/" + std::to_string(page_cnt);
		else
			page_num_string = "0/0";
		Textbox page_num(page_num_string, 16, sf::Color::Black, sf::Vector2f(990, 60), sf::Vector2f(60, 40),
			sf::Color::White);
		textboxs.push_back(page_num);

		Interaction interaction;
		interaction.add_button(back_button, go_back);
		interaction.add_button(previous_button, previous);
		interaction.add_button(next_button, next);
		Button_List<Button_Textbox> status_column;

		if (n > 0) {
			int current_y = 150;
			pages[num_page].for_each([&](const DataIter &Iter) {
				auto course = Iter.ptr<Course>();

				Textbox course_id(course->id, 16, sf::Color::Black, sf::Vector2f(0, current_y), sf::Vector2f(100, 100),
					sf::Color::White);
				course_id.set_outline(sf::Color::Black);
				course_id.align_left();
				textboxs.push_back(course_id);


				Textbox course_name(course->name, 16, sf::Color::Black, sf::Vector2f(100, current_y),
					sf::Vector2f(300, 100), sf::Color::White);
				course_name.set_outline(sf::Color::Black);
				course_name.align_left();
				textboxs.push_back(course_name);


				Textbox credits(std::to_string(course->credits),
					16, sf::Color::Black, sf::Vector2f(400, current_y), sf::Vector2f(100, 100),
					sf::Color::White);
				credits.set_outline(sf::Color::Black);
				textboxs.push_back(credits);


				Textbox session1(Utils::sessionToStr(course->sessions[0].start) + "-" +
					Utils::sessionToStr(course->sessions[0].end).substr(4, 5),
					16, sf::Color::Black, sf::Vector2f(500, current_y), sf::Vector2f(200, 50),
					sf::Color::White);
				session1.set_outline(sf::Color::Black);
				textboxs.push_back(session1);


				Textbox session2(Utils::sessionToStr(course->sessions[1].start) + "-" +
					Utils::sessionToStr(course->sessions[1].end).substr(4, 5),
					16, sf::Color::Black, sf::Vector2f(500, current_y + 50), sf::Vector2f(200, 50),
					sf::Color::White);
				session2.set_outline(sf::Color::Black);
				textboxs.push_back(session2);


				Textbox teacher(course->teacher_name.last + " " + course->teacher_name.first,
					16, sf::Color::Black, sf::Vector2f(700, current_y), sf::Vector2f(250, 100),
					sf::Color::White);
				teacher.set_outline(sf::Color::Black);
				teacher.align_left();
				textboxs.push_back(teacher);


				Textbox slot(std::to_string(course->students.size()) + "/" + std::to_string(course->max_students),
					16, sf::Color::Black, sf::Vector2f(950, current_y), sf::Vector2f(100, 100),
					sf::Color::White);
				slot.set_outline(sf::Color::Black);
				textboxs.push_back(slot);


				if (enrollable) {
					Textbox status("", 16, sf::Color::Black, sf::Vector2f(1050, current_y), sf::Vector2f(150, 100),
						sf::Color::White);
					if (student->getCourse(course->id).empty()) {
						status.set_text("Enroll");
						status.set_box_color(sf::Color::Green);
					}
					else {
						status.set_text("Disenroll");
						status.set_box_color(sf::Color::Red);
					}
					status.align_center();
					Button_Textbox status_button(status, sf::Color::Yellow);
					status_button.set_idle_outline(sf::Color::Black);
					status_column.add_button(status_button);

					current_y += 100;

					std::string conflict_message;
					if (course->students.size() == course->max_students)
						conflict_message = "This course is full";
					else {
						auto OverlappingCourses = student->getOverlappingCourses(course->sessions);
						if (!OverlappingCourses.empty()) {
							conflict_message = "Conflict with: ";
							int i = 0;
							OverlappingCourses.for_each([&](const DataIter &Iter) {
								if (i)
									conflict_message += ", ";
								conflict_message += Iter.ptr<Course>()->id;
								i++;
							});
						}
					}
					Textbox conflict(conflict_message,
						16, sf::Color::Black, sf::Vector2f(0, current_y), sf::Vector2f(1200, 50),
						sf::Color::White);
					conflict.set_outline(sf::Color::Black);
					conflict.align_left();
					textboxs.push_back(conflict);

					current_y += 50;
				}
				else
					current_y += 100;
			});
		}
		status_column.trigger = toggle_enroll_course;
		interaction.add_button_list(status_column);

		data_change = false;
		while (window.isOpen() && !app->scenes.empty() && app->scenes.top() == SceneType::Scene11 && !data_change) {
			window.clear(sf::Color::White);

			headers.for_each([&](Textbox &textbox) {
				textbox.draw(window, app->default_font);
			});

			textboxs.for_each([&](Textbox &textbox) {
				textbox.draw(window, app->default_font);
			});

			interaction.draw(window, app->default_font);
			window.display();

			auto event = interaction.interact(window);
			app->scenes.interact(event);
		}
	}
}
