#include <iomanip>

#include "scene10.h"

static string double_to_string(const double &val, int d = 2) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(d) << val;
    return stream.str();
}

static App *app;
static int num_page, page_cnt;
static SceneType current_scene = SceneType::Scene10;

static void go_back(int dummy) {
    app->scenes.pop();
}

static void previous(int dummy) {
    num_page = std::max(num_page - 1, 0);
}

static void next(int dummy) {
    num_page = std::min(num_page + 1, page_cnt - 1);
}

void scene10(sf::RenderWindow &window, App &_app) {
    app = &_app;
    auto student = app->user_iter.ptr<Student>();

    Textbox course_id("Course ID", 16, sf::Color::Black, sf::Vector2f(80, 100), sf::Vector2f(150, 50),
                      sf::Color::White);
    course_id.set_outline(sf::Color::Black);

    Textbox course_name("Course Name", 16, sf::Color::Black, sf::Vector2f(230, 100), sf::Vector2f(400, 50),
                        sf::Color::White);
    course_name.set_outline(sf::Color::Black);

    Textbox credits("Credits", 16, sf::Color::Black, sf::Vector2f(630, 100), sf::Vector2f(100, 50), sf::Color::White);
    credits.set_outline(sf::Color::Black);

    Textbox midterm("Midterm", 16, sf::Color::Black, sf::Vector2f(730, 100), sf::Vector2f(100, 50), sf::Color::White);
    midterm.set_outline(sf::Color::Black);

    Textbox other("Other", 16, sf::Color::Black, sf::Vector2f(830, 100), sf::Vector2f(100, 50), sf::Color::White);
    other.set_outline(sf::Color::Black);

    Textbox final("Final", 16, sf::Color::Black, sf::Vector2f(930, 100), sf::Vector2f(100, 50), sf::Color::White);
    final.set_outline(sf::Color::Black);

    Textbox total("Total", 16, sf::Color::Black, sf::Vector2f(1030, 100), sf::Vector2f(100, 50), sf::Color::White);
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
    double GPA = 0;
    int total_credit = 0;
    int page_size = 8;
    int num_row = 0;
    List<Textbox> page;
    List<List<Textbox>> pages;
    scores.for_each([&](const std::shared_ptr<Score> &score) {
        ++num_row;
        if (num_row > page_size) {
            num_row -= page_size;
            pages.push_back(page);
            page.clear();
        }

        auto course = score->course.ptr<Course>();

        Textbox id(course->id, 16, sf::Color::Black, sf::Vector2f(80, 100 + num_row * 50),
                   sf::Vector2f(150, 50), sf::Color::White);
        id.set_outline(sf::Color::Black);
        id.align_left();


        Textbox name(course->name, 16, sf::Color::Black, sf::Vector2f(230, 100 + num_row * 50),
                     sf::Vector2f(400, 50), sf::Color::White);
        name.set_outline(sf::Color::Black);
        name.align_left();


        Textbox credits_cnt(std::to_string(course->credits), 16, sf::Color::Black,
                            sf::Vector2f(630, 100 + num_row * 50),
                            sf::Vector2f(100, 50), sf::Color::White);
        credits_cnt.set_outline(sf::Color::Black);


        Textbox midterm_score(double_to_string(score->midterm), 16, sf::Color::Black,
                              sf::Vector2f(730, 100 + num_row * 50),
                              sf::Vector2f(100, 50), sf::Color::White);
        midterm_score.set_outline(sf::Color::Black);


        Textbox other_score(double_to_string(score->other), 16, sf::Color::Black, sf::Vector2f(830, 100 + num_row * 50),
                            sf::Vector2f(100, 50), sf::Color::White);
        other_score.set_outline(sf::Color::Black);


        Textbox final_score(double_to_string(score->final), 16, sf::Color::Black, sf::Vector2f(930, 100 + num_row * 50),
                            sf::Vector2f(100, 50), sf::Color::White);
        final_score.set_outline(sf::Color::Black);


        Textbox total_score(double_to_string(score->total), 16, sf::Color::Black,
                            sf::Vector2f(1030, 100 + num_row * 50),
                            sf::Vector2f(100, 50), sf::Color::White);
        total_score.set_outline(sf::Color::Black);


        page.push_back(id);
        page.push_back(name);
        page.push_back(credits_cnt);
        page.push_back(midterm_score);
        page.push_back(other_score);
        page.push_back(final_score);
        page.push_back(total_score);

        total_credit += course->credits;
        GPA += score->total;
    });
    pages.push_back(page);
    num_page = 0;
    page_cnt = (n - 1) / page_size + 1;

    std::string GPA_string;
    if (n > 0) {
        GPA /= n;
        GPA_string = double_to_string(GPA);
    }

    Textbox GPA_box("GPA: " + GPA_string, 16, sf::Color::Black, sf::Vector2f(610, 60),
                    sf::Vector2f(120, 40), sf::Color::White);
    GPA_box.align_left();

    Textbox total_credit_box("Total credit: " + std::to_string(total_credit), 16, sf::Color::Black,
                             sf::Vector2f(760, 60),
                             sf::Vector2f(140, 40), sf::Color::White);
    total_credit_box.align_left();

    headers.push_back(GPA_box);
    headers.push_back(total_credit_box);

    sf::Texture texture;
    texture.loadFromFile("assets/images/go_back.png");
    Button_Sprite back_button(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));

    texture.loadFromFile("assets/images/previous.jpg");
    Button_Sprite previous_button(texture, sf::Vector2f(950, 60), sf::Vector2f(40, 40));

    texture.loadFromFile("assets/images/next.jpg");
    Button_Sprite next_button(texture, sf::Vector2f(990, 60), sf::Vector2f(40, 40));

    Interaction interaction;
    interaction.add_button(back_button, go_back);
    interaction.add_button(previous_button, previous);
    interaction.add_button(next_button, next);

    while (window.isOpen() && !app->scenes.empty() && app->scenes.top() == current_scene && !app->scenes.refresh) {
        window.clear(sf::Color::White);

        headers.for_each([&](Textbox &textbox) {
            textbox.draw(window, app->default_font);
        });

        if (!pages.empty()) {
            pages[num_page].for_each([&](Textbox &textbox) {
                textbox.draw(window, app->default_font);
            });
        }

        std::string page_num_string;
        if (n > 0)
            page_num_string = std::to_string(num_page + 1) + "/" + std::to_string(page_cnt);
        else
            page_num_string = "0/0";
        Textbox page_num(page_num_string, 16, sf::Color::Black, sf::Vector2f(1030, 60), sf::Vector2f(60, 40),
                         sf::Color::White);
        page_num.draw(window, app->default_font);

        interaction.draw(window, app->default_font);

        auto event = interaction.interact(window);
        app->scenes.interact(event);
        window.display();
    }
}