#include "Scene4.h"

static App* app;
static sf::RenderWindow* windowP;
static bool inCreate;
Input_Textbox* firstNameInputBoxP;
Input_Textbox* lastNameInputBoxP;
Input_Textbox* studenIdInputBoxP;
Input_Textbox* socialIdInputBoxP;
Input_Textbox* genderIdInputBoxP;
Input_Textbox* dayInputBoxP;
Input_Textbox* monthInputBoxP;
Input_Textbox* yearInputBoxP;

DataIter* clP;

static int to_int(string s)
{
	int sum = 0;
	for (int i = 0; i < (int)s.size(); i++)
	{
		sum = sum * 10 + int(s[i]) - 48;
	}
	return sum;
}

static void draw_student(DataIter& cl)
{
	Textbox studentBox("", defaultMediumCharSize, sf::Color::White, sf::Vector2f(0, 0), sf::Vector2f(350, 50), sf::Color::Black);
	Textbox studentIdBox("", defaultMediumCharSize, sf::Color::White, sf::Vector2f(0, 0), sf::Vector2f(150, 50), sf::Color::Black);


	auto ptrTmp = cl.ptr<Class>();
	List<DataIter> students = ptrTmp->students;
	int cnt = 0;
	students.for_each([&](DataIter& stu)
		{
			auto ptr = stu.ptr<Student>();
			cnt++;
			studentBox.set_text(ptr->name.toStr());
			studentBox.set_box_position(sf::Vector2f(windowWidth / 3 - 175, 100 + cnt * 65));

			studentIdBox.set_text(ptr->student_id);
			studentIdBox.set_box_position(sf::Vector2f(windowWidth / 3 - 330, 100 + cnt * 65));

			studentBox.draw(*windowP, app->default_font);
			studentIdBox.draw(*windowP, app->default_font);
		});
}

static void add_student_function(int dummy)
{
	inCreate = false;
	string firstName = firstNameInputBoxP->text;
	string lastName = lastNameInputBoxP->text;
	string studentId = studenIdInputBoxP->text;
	string socialId = socialIdInputBoxP->text;
	string genderStr = genderIdInputBoxP->text;
	tm birth = Utils::mktm(to_int(dayInputBoxP->text), to_int(monthInputBoxP->text), to_int(yearInputBoxP->text));
	Gender gender;
	if (genderStr == "male") gender = Gender::Male;
	else if (genderStr == "female") gender = Gender::Female;
	else if (genderStr == "other") gender = Gender::Other;
	else gender = Gender::Unknown;

	Student stu(studentId, socialId, FullName(firstName, lastName), gender, birth);
	auto ptr = clP->ptr<Class>();
	app->addStudent(make_shared<Student>(stu), ptr->name);
}

static void add_student()
{
	Interaction interaction;
	Textbox mainBackground("", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 2 - 300, windowHeight / 2 - 350), sf::Vector2f(600, 600), sf::Color::Black);
	mainBackground.set_outline(sf::Color::Blue, 4);

	Textbox firstNameText("First name:", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4 + 20, 125), sf::Vector2f(130, 50), sf::Color::Transparent);
	Textbox firstNameBox("", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 4 + 170, 125), sf::Vector2f(410, 50), sf::Color::White);
	Input_Textbox firstNameInputBox(firstNameBox, 25, sf::Color::Blue);
	interaction.add_input_textbox(firstNameInputBox);

	Textbox lastNameText("Last name:", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4 + 20, 200), sf::Vector2f(130, 50), sf::Color::Transparent);
	Textbox lastNameBox("", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 4 + 170, 200), sf::Vector2f(410, 50), sf::Color::White);
	Input_Textbox lastNameInputBox(lastNameBox, 25, sf::Color::Blue);
	interaction.add_input_textbox(lastNameInputBox);

	Textbox studenIdText("Student id:", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4 + 20, 275), sf::Vector2f(130, 50), sf::Color::Transparent);
	Textbox studenIdBox("", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 4 + 170, 275), sf::Vector2f(410, 50), sf::Color::White);
	Input_Textbox studenIdInputBox(studenIdBox, 25, sf::Color::Blue);
	interaction.add_input_textbox(studenIdInputBox);

	Textbox socialIdText("Social id:", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4 + 20, 350), sf::Vector2f(130, 50), sf::Color::Transparent);
	Textbox socialIdBox("", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 4 + 170, 350), sf::Vector2f(410, 50), sf::Color::White);
	Input_Textbox socialIdInputBox(socialIdBox, 25, sf::Color::Blue);
	interaction.add_input_textbox(socialIdInputBox);

	Textbox genderText("Gender:", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4 + 20, 425), sf::Vector2f(130, 50), sf::Color::Transparent);
	Textbox genderIdBox("", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 4 + 170, 425), sf::Vector2f(410, 50), sf::Color::White);
	Input_Textbox genderIdInputBox(genderIdBox, 25, sf::Color::Blue);
	interaction.add_input_textbox(genderIdInputBox);

	Textbox birthText("Date of birth:", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4 + 20, 500), sf::Vector2f(130, 50), sf::Color::Transparent);
	Textbox dayBox("", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 3 + 80, 500), sf::Vector2f(100, 50), sf::Color::White);
	Textbox monthBox("", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 3 + 230, 500), sf::Vector2f(100, 50), sf::Color::White);
	Textbox yearBox("", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 3 + 380, 500), sf::Vector2f(100, 50), sf::Color::White);
	Input_Textbox dayInputBox(dayBox, 2, sf::Color::Blue);
	Input_Textbox monthInputBox(monthBox, 2, sf::Color::Blue);
	Input_Textbox yearInputBox(yearBox, 4, sf::Color::Blue);

	interaction.add_input_textbox(dayInputBox);
	interaction.add_input_textbox(monthInputBox);
	interaction.add_input_textbox(yearInputBox);

	firstNameInputBoxP = &firstNameInputBox;
	lastNameInputBoxP = &lastNameInputBox;
	studenIdInputBoxP = &studenIdInputBox;
	socialIdInputBoxP = &socialIdInputBox;
	genderIdInputBoxP = &genderIdInputBox;
	dayInputBoxP = &dayInputBox;
	monthInputBoxP = &monthInputBox;
	yearInputBoxP = &yearInputBox;

	Textbox enterBox("ENTER", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 2 - 65, windowHeight / 2 + 150), sf::Vector2f(130, 50), sf::Color::Blue);
	Button_Textbox enterButton(enterBox, sf::Color::White);
	interaction.add_button(enterButton, add_student_function);
	inCreate = true;
	while (windowP->isOpen() && inCreate)
	{
		
		windowP->clear(sf::Color::White);
		mainBackground.draw(*windowP, app->default_font);
		interaction.draw(*windowP, app->default_font);
		firstNameText.draw(*windowP, app->default_font);
		lastNameText.draw(*windowP, app->default_font);
		studenIdText.draw(*windowP, app->default_font);
		socialIdText.draw(*windowP, app->default_font);
		genderText.draw(*windowP, app->default_font);
		birthText.draw(*windowP, app->default_font);
		windowP->display();
		interaction.interact(*windowP);
	}
}

static void import_student_function(int dummy)
{
	inCreate = false;
	auto csvData = CSVIO::tryParse("csv/" + studenIdInputBoxP->text);
	auto ptr = clP->ptr<Class>();
	app->addStudents(csvData, ptr->name);
}

static void import_student()
{
	Interaction interaction;
	Textbox mainBackground("", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 2 - 300, windowHeight / 2 - 250), sf::Vector2f(600, 300), sf::Color::Black);
	mainBackground.set_outline(sf::Color::Blue, 4);

	Textbox studenIdText("file Name:", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4 + 20, 275), sf::Vector2f(130, 50), sf::Color::Transparent);
	Textbox studenIdBox("", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 4 + 170, 275), sf::Vector2f(410, 50), sf::Color::White);
	Input_Textbox studenIdInputBox(studenIdBox, 25, sf::Color::Blue);
	interaction.add_input_textbox(studenIdInputBox);

	Textbox enterBox("ENTER", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 2 - 65, windowHeight / 2 - 50), sf::Vector2f(130, 50), sf::Color::Blue);
	Button_Textbox enterButton(enterBox, sf::Color::White);
	interaction.add_button(enterButton, import_student_function);

	studenIdInputBoxP = &studenIdInputBox;

	inCreate = true;
	while (windowP->isOpen() && inCreate)
	{

		windowP->clear(sf::Color::White);
		mainBackground.draw(*windowP, app->default_font);
		interaction.draw(*windowP, app->default_font);
		studenIdText.draw(*windowP, app->default_font);
		windowP->display();
		interaction.interact(*windowP);
	}
}

static void revome_student_function(int dummy)
{
	inCreate = false;
	auto ptr = clP->ptr<Class>();
	string studentId = studenIdInputBoxP->text;
	app->deleteStudent(ptr->getStudent(studentId));
}

static void revome_student()
{
	Interaction interaction;
	Textbox mainBackground("", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 2 - 300, windowHeight / 2 - 250), sf::Vector2f(600, 300), sf::Color::Black);
	mainBackground.set_outline(sf::Color::Blue, 4);

	Textbox studenIdText("Student id:", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4 + 20, 275), sf::Vector2f(130, 50), sf::Color::Transparent);
	Textbox studenIdBox("", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 4 + 170, 275), sf::Vector2f(410, 50), sf::Color::White);
	Input_Textbox studenIdInputBox(studenIdBox, 25, sf::Color::Blue);
	interaction.add_input_textbox(studenIdInputBox);

	Textbox enterBox("ENTER", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 2 - 65, windowHeight / 2 - 50), sf::Vector2f(130, 50), sf::Color::Blue);
	Button_Textbox enterButton(enterBox, sf::Color::White);
	interaction.add_button(enterButton, revome_student_function);

	studenIdInputBoxP = &studenIdInputBox;

	inCreate = true;
	while (windowP->isOpen() && inCreate)
	{

		windowP->clear(sf::Color::White);
		mainBackground.draw(*windowP, app->default_font);
		interaction.draw(*windowP, app->default_font);
		studenIdText.draw(*windowP, app->default_font);
		windowP->display();
		interaction.interact(*windowP);
	}
}

static void remove_class()
{
	app->deleteClass(*clP);
}

void scene4(sf::RenderWindow& window, App& _app, DataIter &cl)
{
	windowP = &window;
	app = &_app;
	clP = &cl;

	Textbox headText("Student list", 40, sf::Color::Black, sf::Vector2f(windowWidth / 3 - 200, 100), sf::Vector2f(200, 50), sf::Color::Transparent);
	Textbox headText2("Functions", 40, sf::Color::Black, sf::Vector2f(windowWidth / 3 * 2, 100), sf::Vector2f(200, 50), sf::Color::Transparent);

	Textbox addStudentButton("Add student", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 3 * 2 - 50, 100 + 75), sf::Vector2f(300, 50), sf::Color::Green);
	Textbox importStudentButton("Import student", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 3 * 2 - 50, 100 + 150), sf::Vector2f(300, 50), sf::Color::Green);
	Textbox removeStudentButton("Remove student by id", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 3 * 2 - 50, 100 + 225), sf::Vector2f(300, 50), sf::Color::Green);
	Textbox viewScoreButton("View score board", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 3 * 2 - 50, 100 + 300), sf::Vector2f(300, 50), sf::Color::Green);
	Textbox removeClassButton("Remove this class", defaultMediumCharSize, sf::Color::Black, sf::Vector2f(windowWidth / 3 * 2 - 50, 100 + 375), sf::Vector2f(300, 50), sf::Color::Red);

	sf::Texture texture;
	texture.loadFromFile("assets/images/go_back.png");
	Button_Sprite back_button = Button_Sprite(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));
	
	while (window.isOpen())
	{
		sf::Event event;
		sf::Vector2i mousePos = Mouse::getPosition(window);
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
            {
                window.close();
                app->scenes.clear();
            }
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (mousePos.x >= 10 && mousePos.x <= 50 && mousePos.y >= 5 && mousePos.y <= 45)
					{
						return;
					}
					if (addStudentButton.inside(mousePos.x, mousePos.y))
					{
						add_student();
					}
					if (importStudentButton.inside(mousePos.x, mousePos.y))
					{
						import_student();
					}
					if (removeStudentButton.inside(mousePos.x, mousePos.y))
					{
						revome_student();
					}
					if (removeClassButton.inside(mousePos.x, mousePos.y))
					{
						remove_class();
						return;
					}
				}
			}
		}
		window.clear(sf::Color::White);
		if (mousePos.x >= 10 && mousePos.x <= 50 && mousePos.y >= 5 && mousePos.y <= 45)
		{
			back_button.idle = false;
		}
		else back_button.idle = true;
		back_button.draw(window, app->default_font);

		draw_student(cl);
		headText.draw(window, app->default_font);
		headText2.draw(window, app->default_font);

		if (addStudentButton.inside(mousePos.x, mousePos.y)) addStudentButton.set_outline(sf::Color::Blue);
		else addStudentButton.set_outline(sf::Color::Transparent);
		addStudentButton.draw(window, app->default_font);

		if (importStudentButton.inside(mousePos.x, mousePos.y)) importStudentButton.set_outline(sf::Color::Blue);
		else importStudentButton.set_outline(sf::Color::Transparent);
		importStudentButton.draw(window, app->default_font);

		if (removeStudentButton.inside(mousePos.x, mousePos.y)) removeStudentButton.set_outline(sf::Color::Blue);
		else removeStudentButton.set_outline(sf::Color::Transparent);
		removeStudentButton.draw(window, app->default_font);

		if (viewScoreButton.inside(mousePos.x, mousePos.y)) viewScoreButton.set_outline(sf::Color::Blue);
		else viewScoreButton.set_outline(sf::Color::Transparent);
		viewScoreButton.draw(window, app->default_font);

		if (removeClassButton.inside(mousePos.x, mousePos.y)) removeClassButton.set_outline(sf::Color::Blue);
		else removeClassButton.set_outline(sf::Color::Transparent);
		removeClassButton.draw(window, app->default_font);
		window.display();
	}
}