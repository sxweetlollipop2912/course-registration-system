#include "scene3.h"

static App *app;
static sf::RenderWindow *windowP;
static bool inCreate;
static Input_Textbox *classNameInputBoxP;

static void go_back() {
    app->scenes.pop();
}

static void draw_class(sf::Vector2i mousePos) {
    Textbox classBox("", defaultMediumCharSize, sf::Color::White, sf::Vector2f(0, 0), sf::Vector2f(200, 50),
                     sf::Color::Black);
    List<DataIter> classes = app->getAllClasses();

    int cnt = 0;
    classes.for_each([&](DataIter &cl) {
        auto ptr = cl.ptr<Class>();
        cnt++;
        classBox.set_text(ptr->name);
        classBox.set_box_position(sf::Vector2f(windowWidth / 2 - 100, 200 + cnt * 75));
        if (classBox.inside(mousePos.x, mousePos.y)) {
            classBox.set_outline(sf::Color::Blue);
        } else classBox.set_outline(sf::Color::Transparent);
        classBox.draw(*windowP, app->default_font);
    });
}

static void clickClass(sf::Vector2i mousePos) {
    Textbox classBox("", defaultMediumCharSize, sf::Color::White, sf::Vector2f(0, 0), sf::Vector2f(200, 50),
                     sf::Color::Black);
    List<DataIter> classes = app->getAllClasses();

    int cnt = 0;
    classes.for_each([&](DataIter &cl) {
        auto ptr = cl.ptr<Class>();
        cnt++;
        classBox.set_text(ptr->name);
        classBox.set_box_position(sf::Vector2f(windowWidth / 2 - 100, 200 + cnt * 75));
        if (classBox.inside(mousePos.x, mousePos.y)) {
            //app->scenes.push(4);
            app->scenes.arg = cl;
            scene4(*windowP, *app);
            return;
        }
    });
}

static void create_class_function(int dummy) {
    inCreate = false;
    if (classNameInputBoxP->text.size() > 0) app->addClass(make_shared<Class>(classNameInputBoxP->text));
}

static void create_class() {
    Interaction interaction;
    Textbox mainBackground("", defaultMediumCharSize, sf::Color::White, sf::Vector2f(windowWidth / 4, windowHeight / 3),
                           sf::Vector2f(windowWidth / 2, windowHeight / 3), sf::Color::Black);
    mainBackground.set_outline(sf::Color::Blue, 4);

    Textbox classNameText("Class name:", defaultMediumCharSize, sf::Color::White,
                          sf::Vector2f(windowWidth / 4 + 20, windowHeight / 3 + 30), sf::Vector2f(130, 50),
                          sf::Color::Transparent);
    Textbox classNameBox("", defaultMediumCharSize, sf::Color::Black,
                         sf::Vector2f(windowWidth / 4 + 170, windowHeight / 3 + 30), sf::Vector2f(410, 50),
                         sf::Color::White);
    Input_Textbox classNameInputBox(classNameBox, 10, sf::Color::Blue);
    classNameInputBoxP = &classNameInputBox;

    Textbox enterBox("ENTER", defaultMediumCharSize, sf::Color::White,
                     sf::Vector2f(windowWidth / 2 - 65, windowHeight / 3 + 235), sf::Vector2f(130, 50),
                     sf::Color::Blue);
    Button_Textbox enterButton(enterBox, sf::Color::White);

    interaction.add_input_textbox(classNameInputBox);
    interaction.add_button(enterButton, create_class_function);
    inCreate = true;
    while (windowP->isOpen() && inCreate) {
        windowP->clear(sf::Color::White);
        mainBackground.draw(*windowP, app->default_font);
        classNameText.draw(*windowP, app->default_font);
        interaction.draw(*windowP, app->default_font);
        windowP->display();
        interaction.interact(*windowP);
    }
}

void scene3(sf::RenderWindow &window, App &_app) {
    windowP = &window;
    app = &_app;

    Textbox classText("Classes", 40, sf::Color::Black, sf::Vector2f(windowWidth / 2 - 100, 200), sf::Vector2f(200, 50),
                      sf::Color::Transparent);
    Textbox createClassBox("Create class", defaultMediumCharSize, sf::Color::Black,
                           sf::Vector2f(windowWidth / 2 - 100, 200 - 75), sf::Vector2f(200, 50), sf::Color::Green);

    sf::Texture texture;
    texture.loadFromFile("assets/images/go_back.png");
    Button_Sprite back_button = Button_Sprite(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));

    while (window.isOpen() && app->scenes.top() == SceneType::Scene3) {
        sf::Event event;
        sf::Vector2i mousePos = Mouse::getPosition(window);

        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
                app->scenes.clear();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (createClassBox.inside(mousePos.x, mousePos.y)) {
                        create_class();
                    }
                    if (mousePos.x >= 10 && mousePos.x <= 50 && mousePos.y >= 5 && mousePos.y <= 45) {
                        go_back();
                        return;
                    }
                    clickClass(mousePos);

                }
            }
        }
        window.clear(sf::Color::White);
        classText.draw(window, app->default_font);

        if (mousePos.x >= 10 && mousePos.x <= 50 && mousePos.y >= 5 && mousePos.y <= 45) {
            back_button.idle = false;
        } else back_button.idle = true;
        back_button.draw(window, app->default_font);

        if (createClassBox.inside(mousePos.x, mousePos.y)) createClassBox.set_outline(sf::Color::Blue);
        else createClassBox.set_outline(sf::Color::Transparent);
        createClassBox.draw(window, app->default_font);

        draw_class(mousePos);
        window.display();
    }
}